/*
 * =====================================================================================
 *
 *       Filename: apiclient.hpp
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
#include <memory>
#include <cstdint>
#include <cinttypes>
#include <functional>
#include "socket.hpp"
#include "typedef.hpp"

namespace libnvc
{
    class api_client
    {
        private:
            class stream_decoder;

        private:
            std::unique_ptr<stream_decoder> m_decoder;

        private:
            int64_t m_seqid;

        private:
            libnvc::socket *m_socket;

        private:
            std::map<int64_t, std::function<void(libnvc::object)>> m_onresp;

        public:
            api_client(libnvc::socket *);

        public:
            // seems I can't use the default dtor
            // because of the incomplete type stream_decoder and unique_ptr
            ~api_client();

        public:
            int64_t seqid(int64_t advanced)
            {
                return (m_seqid += advanced);
            }

        private:
            int64_t msgid(size_t req_id, int64_t seq_id)
            {
                return (((int64_t)(req_id) & 0x000000000000ffff) << 48) | (seq_id & 0x0000ffffffffffff);
            }

            std::tuple<size_t, int64_t> msgid_decomp(int64_t msg_id)
            {
                return {(size_t)(msg_id >> 48), msg_id & 0x0000ffffffffffff};
            }

        public:
            template<size_t reqid> inline void forward(typename libnvc::req<reqid>::parms_t parms, std::function<void(typename libnvc::req<reqid>::res_t)> on_resp)
            {
                int64_t seq_id = seqid(1);
                int64_t msg_id = msgid(reqid, seq_id);

                auto bytestream = parms.pack(msg_id);
                m_socket->send(bytestream.data(), bytestream.length());

                if(!on_resp){
                    return;
                }

                if(m_onresp.find(msg_id) != m_onresp.end()){
                    throw std::runtime_error(((std::string(": req already has response hander: req = ") + libnvc::idstr(reqid)) + ", seqid = ") + std::to_string(seq_id));
                }

                m_onresp[msg_id] = [this, on_resp](libnvc::object result)
                {
                    on_resp(std::get<typename libnvc::req<reqid>::res_t>(result));
                };
            }

        public:
            void poll();
    };
}
