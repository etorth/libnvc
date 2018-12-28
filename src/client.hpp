/*
 * =====================================================================================
 *
 *       Filename: client.hpp
 *        Created: 12/26/2018 01:57:33
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#pragma once
#include <map>
#include <cstdint>
#include <cinttypes>
#include "libnvc.h"

namespace libnvc
{
    class client_base
    {
        private:
            class client_impl;

        private:
            std::unique_ptr<client_impl> m_impl;

        private:
            std::map<int64_t, std::function<libnvc::req<0>::res_t>> m_onresp;

        private:
            struct libnvc_api m_libnvc_api;

        public:
            client(struct libnvc_api);

        public:
            ~client() = default;

        public:
            void log(int, const char *);

        public:
            virtual bool connect()    = 0;
            virtual bool disconnect() = 0;

        public:
            virtual size_t send(const uint8_t *, size_t) = 0;
            virtual size_t recv(const uint8_t *, size_t) = 0;

        public:
            void poll();

        public:
            template<int reqid> void forward(libnvc::req<reqid>::parms_t parms, std::function<void(libncv::req<reqid>::res_t)> on_resp)
            {
                int64_t seq_id = seqid(1);
                int64_t msg_id = msgid(reqid, seq_id).id();

                parms.pack(msg_id);
                m_socket.send(parms.data(), parms.length());

                if(!on_resp){
                    return;
                }

                if(m_onresp.find(msg_id) != m_onresp.end()){
                    throw std::runtime_error(str_printf(": req already has response hander: req = %s, seqid = %" PRIi64, id2cstr(reqid), seq_id));
                }

                m_onresp[msg_id] = [this, on_resp](libncv::req<0>::res_t result)
                {
                    on_resp(std::get<libnvc::req<reqid>::res_t>(result));
                };
            }
    };

    // an implementation for convience
    // don't expose the implementation detail to user for less dependency

    class default_client: public libnvc::client_base
    {
        private:
            class socket_impl;

        private:
            std::unique_ptr<socket_impl> m_socket;

        private:
            const std::string m_address;

        public:
            default_client(const char *address, libnvc::api api)
                : client_base(api)
                , m_socket()
                , m_address(address)
            {}

        public:
            bool connect();

        public:
            size_t send();
            size_t recv();
    };
}
