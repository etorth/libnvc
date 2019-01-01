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
#include <type_traits>
#include "socket.hpp"
#include "objdef.hpp"
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
            std::map<int64_t, std::function<void(libnvc::object)>>       m_onresp;
            std::map<int64_t, std::function<void(int64_t, std::string)>> m_onresperr;

        public:
            api_client(libnvc::socket *);

        public:
            // seems I can't use the default dtor
            // because of the incomplete type stream_decoder and unique_ptr
            ~api_client();

        public:
            int64_t seqid()
            {
                return m_seqid;
            }

            int64_t seqid(int64_t advanced)
            {
                return (m_seqid += advanced);
            }

        private:
            static int64_t msgid(size_t req_id, int64_t seq_id)
            {
                // put seq_id at hight bits
                // this helps to make the key/value pairs sorted by sent time
                return ((seq_id & 0x0000ffffffffffff) << 16) | ((int64_t)(req_id) & 0x000000000000ffff);
            }

            static std::tuple<size_t, int64_t> msgid_decomp(int64_t msg_id)
            {
                return {(size_t)(msg_id & 0x000000000000ffff), msg_id >> 16};
            }

        public:
            template<size_t reqid> inline void forward(typename libnvc::req<reqid>::parms_t parms, std::function<void(typename libnvc::req<reqid>::res_t)> on_resp, std::function<void(int64_t, std::string)> on_resperr = [](int64_t, std::string){})
            {
                int64_t seq_id = seqid(1);
                int64_t msg_id = msgid(reqid, seq_id);

                auto pkres = parms.pack(msg_id);
                m_socket->send(pkres.data(), pkres.length());

                if(on_resp){
                    if(m_onresp.find(msg_id) != m_onresp.end()){
                        throw std::runtime_error(((std::string("response handler already resgistered: req = ") + libnvc::idstr(reqid)) + ", seqid = ") + std::to_string(seq_id));
                    }

                    m_onresp[msg_id] = [this, on_resp](libnvc::object result)
                    {
                        if constexpr (std::is_void_v<typename libnvc::req<reqid>::res_t>){
                            on_resp();
                        }else{
                            on_resp(std::get<typename libnvc::req<reqid>::res_t>(result));
                        }
                    };
                }

                if(on_resperr){
                    if(m_onresperr.find(msg_id) != m_onresperr.end()){
                        throw std::runtime_error(((std::string("response error handler already resgistered: req = ") + libnvc::idstr(reqid)) + ", seqid = ") + std::to_string(seq_id));
                    }
                    m_onresperr[msg_id] = on_resperr;
                }
            }

            template<size_t reqid> inline void forward(typename libnvc::req<reqid>::parms_t parms, std::function<void()> on_resp, std::function<void(int64_t, std::string)> on_resperr = [](int64_t, std::string){})
            {
                static_assert(std::is_void_v<typename libnvc::req<reqid>::res_t>);

                int64_t seq_id = seqid(1);
                int64_t msg_id = msgid(reqid, seq_id);

                auto pkres = parms.pack(msg_id);
                m_socket->send(pkres.data(), pkres.length());

                if(on_resp){
                    if(m_onresp.find(msg_id) != m_onresp.end()){
                        throw std::runtime_error(((std::string("response handler already resgistered: req = ") + libnvc::idstr(reqid)) + ", seqid = ") + std::to_string(seq_id));
                    }

                    m_onresp[msg_id] = [this, on_resp](libnvc::object)
                    {
                        on_resp();
                    };
                }

                if(on_resperr){
                    if(m_onresperr.find(msg_id) != m_onresperr.end()){
                        throw std::runtime_error(((std::string("response error handler already resgistered: req = ") + libnvc::idstr(reqid)) + ", seqid = ") + std::to_string(seq_id));
                    }
                    m_onresperr[msg_id] = on_resperr;
                }
            }

        public:
            void poll();
    };
}
