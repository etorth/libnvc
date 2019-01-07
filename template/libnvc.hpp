/*
 * =====================================================================================
 *
 *       Filename: libnvc.hpp
 *        Created: 12/27/2018 04:52:17
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
#include <variant>
#include <cstdint>
#include <cstddef>
#include <functional>
#include <type_traits>

namespace libnvc
{
    constexpr int LOG_INFO    = 0;
    constexpr int LOG_WARNING = 1;
    constexpr int LOG_FATAL   = 2;
    constexpr int LOG_DEBUG   = 3;

    void log(int, const char *);
    void set_log(std::function<void(int, const char *)>);
}

namespace libnvc::ctf
{
    constexpr size_t strlen(const char *str)
    {
        size_t index = 0;
        while(str[index] != '\0'){
            ++index;
        }
        return index;
    }

    constexpr int strcmp(const char *str1, const char *str2)
    {
        for(size_t index = 0;; ++index){
            if(str1[index] < str2[index]){
                return -1;
            }

            if(str1[index] > str2[index]){
                return 1;
            }

            if(str1[index] == '\0'){
                return 0;
            }
        }
    }

    constexpr int strncmp(const char *str1, const char *str2, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            if(str1[index] < str2[index]){
                return -1;
            }

            if(str1[index] > str2[index]){
                return 1;
            }

            if(str1[index] == '\0'){
                return 0;
            }
        }
        return 0;
    }

    constexpr int chmemcmp(const char *mem1, const char *mem2, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            if(mem1[index] < mem2[index]){
                return -1;
            }

            if(mem1[index] > mem2[index]){
                return +1;
            }
        }
        return 0;
    }

    constexpr void chmemset(char *dst, char value, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            dst[index] = value;
        }
    }

    constexpr char *strcat(char *dst, const char *src)
    {
        size_t index  = 0;
        size_t length = libnvc::ctf::strlen(dst);

        for(index = 0; src[index] != '\0'; ++index){
            dst[length + index] = src[index];
        }

        dst[length + index] = '\0';
        return dst;
    }
}

namespace libnvc
{
    constexpr const char * g_nvim_api_reserved_string_tbl[]
    {
        // put the zero index as reserved
        // only allow the first entry as nullptr
        nullptr,

        // requests
{% for req in nvim_reqs %}
        "req::{{req.name}}",
{% endfor %}

        // notifications
        "notif::test",
    };

    constexpr const char *idstr(size_t id)
    {
        if(id == 0 || id >= std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value){
            return nullptr;
        }
        return g_nvim_api_reserved_string_tbl[id];
    }

    constexpr const char *reqstr(size_t id)
    {
        if(id == 0 || id >= std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value){
            return nullptr;
        }

        if(auto str = g_nvim_api_reserved_string_tbl[id]; libnvc::ctf::strncmp(str, "req::", 5) == 0){
            return str + 5;
        }
        return nullptr;
    }

    constexpr size_t strid(const char *str)
    {
        if(str == nullptr){
            return 0;
        }

        for(size_t index = 1; index < std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value; ++index){
            if(libnvc::ctf::strcmp(str, g_nvim_api_reserved_string_tbl[index]) == 0){
                return index;
            }
        }
        return 0;
    }

    constexpr size_t strid(const char *prefix, const char *name)
    {
        char buf[512] = {'\0'};
        libnvc::ctf::strcat(buf, prefix);
        libnvc::ctf::strcat(buf, "::"  );
        libnvc::ctf::strcat(buf, name  );
        return libnvc::strid(buf);
    }

    constexpr size_t reqid(const char *req_name)
    {
        return strid("req", req_name);
    }

    constexpr size_t notifid(const char *notif_name)
    {
        return strid("notif", notif_name);
    }

    constexpr size_t min_reqid()
    {
        for(size_t id = 1; libnvc::idstr(id) != nullptr; ++id){
            if(libnvc::ctf::strncmp(libnvc::idstr(id), "req::", 5) == 0){
                return id;
            }
        }
        return 0;
    }

    constexpr size_t max_reqid()
    {
        for(size_t id = std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value - 1; libnvc::idstr(id) != nullptr; --id){
            if(libnvc::ctf::strncmp(libnvc::idstr(id), "req::", 5) == 0){
                return id;
            }
        }
        return 0;
    }

    constexpr size_t min_notifid()
    {
        for(size_t id = 1; libnvc::idstr(id) != nullptr; ++id){
            if(libnvc::ctf::strncmp(libnvc::idstr(id), "notif::", 7) == 0){
                return id;
            }
        }
        return 0;
    }

    constexpr size_t max_notifid()
    {
        for(size_t id = std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value - 1; libnvc::idstr(id) != nullptr; --id){
            if(libnvc::ctf::strncmp(libnvc::idstr(id), "notif::", 7) == 0){
                return id;
            }
        }
        return 0;
    }
}

namespace libnvc
{
    class io_device
    {
        public:
            io_device() = default;

        public:
            virtual ~io_device() = default;

        public:
            virtual size_t send(const char *, size_t) = 0; // blocking send, return after done all bytes
            virtual size_t recv(      char *, size_t) = 0;

        public:
            size_t send(std::string_view s)
            {
                return send(s.data(), s.length());
            }
    };
}

namespace libnvc
{
    class socket: public libnvc::io_device
    {
        public:
            socket() = default;

        public:
            virtual ~socket() = default;

        public:
            virtual bool connect(const char *, int) = 0;
            virtual void disconnect()               = 0;
    };

    class asio_socket: public libnvc::socket
    {
        private:
            class asio_socket_impl;

        private:
            std::unique_ptr<asio_socket_impl> m_impl;

        public:
            asio_socket();

        public:
            virtual ~asio_socket();

        public:
            virtual bool connect(const char *, int);
            virtual void disconnect();

        public:
            virtual size_t send(const char *, size_t);
            virtual size_t recv(      char *, size_t);
    };
}

namespace libnvc
{
    constexpr int64_t REQ   = 0;
    constexpr int64_t RESP  = 1;
    constexpr int64_t NOTIF = 2;
}

namespace libnvc
{
    struct _void_type
    {
        int unused = 0;
    };
    using void_type = struct _void_type;


    // TODO: need a better implementation
    // using helper class object_wrapper since  we can't define recursive type in cpp

    // be careful of std::vector<object> vs std::vector<object_wrapper>
    // this can easily give bug
    class object_wrapper;
    using object = std::variant<
          bool,
          int64_t,
          double,
          std::string,
          std::vector<libnvc::object_wrapper>,
          std::map<std::string, libnvc::object_wrapper>
    >;

    class object_wrapper
    {
        public:
            object_wrapper() = default;

        public:
            template<typename T> object_wrapper(T t)
            {
                m_ptr = std::make_shared<libnvc::object>(t);
            }

        private:
            // use shared_ptr not unique_ptr
            // the sematics of object_wrapper is a wrapper of an existing object
            std::shared_ptr<libnvc::object> m_ptr;

            // don't define operator object ()
            // since which returns a copy of the included object

        public:
            libnvc::object &ref()
            {
                return *(m_ptr.get());
            }

            const libnvc::object &ref() const
            {
                return *(m_ptr.get());
            }
    };

    using resp_variant = std::variant<libnvc::void_type,
{% for result_type in nvim_reqs|map(attribute='return_type')|unique %}
    {% if result_type != 'void' %}
        {{result_type}}{% if not loop.last %},{% endif %} 
    {% endif %}
{% endfor %}
    >;
}

namespace libnvc
{
    inline int64_t msgid(size_t req_id, int64_t seq_id)
    {
        // put seq_id at high bits
        // this helps to make the key/value pairs sorted by sent time
        return ((seq_id & 0x0000ffffffffffff) << 16) | ((int64_t)(req_id) & 0x000000000000ffff);
    }

    inline std::tuple<size_t, int64_t> msgid_decomp(int64_t msg_id)
    {
        return {(size_t)(msg_id & 0x000000000000ffff), msg_id >> 16};
    }
}

namespace libnvc::mpinterf
{
    class writer
    {
        private:
            constexpr static size_t m_writer_size = []()
            {
#ifdef LIBNVC_MPACK_WRITER_SIZE
                return LIBNVC_MPACK_WRITER_SIZE;
#else
                return 128;
#endif
            }();

            constexpr static size_t m_writer_align = []()
            {
#ifdef LIBNVC_MPACK_WRITER_ALIGN
                return LIBNVC_MPACK_WRITER_ALIGN;
#else
                return 16;
#endif
            }();

        private:
            std::aligned_storage_t<m_writer_size, m_writer_align> m_storage;

        private:
            // need this helper variable to record if writer alive
            // there is no official way to track if the writer called mpack_writer_destroy()
            bool m_writer_alive;

        private:
            char  *m_data;
            size_t m_size;

        public:
            writer();

        public:
            ~writer();

        private:
            void *storage()
            {
                return &m_storage;
            }

        private:
            void flush();

        private:
            void clear();
            void reset();

        private:
            std::string_view pack();

        private:
            template<typename T, typename... U> void write(T t, U... u)
            {
                write(t);
                if(sizeof...(u) != 0){
                    write(u...);
                }
            }

        private:
            void write();
            void write(double);
            void write(int64_t);
            void write(const char *);
            void write(const std::string&);
            void write(const libnvc::object&);
            void write(const std::array<int64_t, 2> &);
            void write(const std::map<std::string, libnvc::object>&);

        private:
            void start_array(size_t);
            void finish_array();

        public:
            template<typename... Args> std::string_view pack_req(int64_t msgid, Args... args)
            {
                auto [req_id, seq_id] = libnvc::msgid_decomp(msgid);
                if(seq_id <= 0){
                }

                reset();
                start_array(4);
                {
                    write(libnvc::REQ);
                    write(msgid);
                    write(libnvc::reqstr(req_id));
                    start_array(sizeof...(args));
                    {
                        write(args...);
                    }
                    finish_array();

                }
                finish_array();
                return pack();
            }
    };
}

namespace libnvc
{
    template<size_t> struct req;

{% for req in nvim_reqs %}
    template<> struct req<libnvc::reqid("{{req.name}}")>
    {
        using parms_tuple = std::tuple<
{% for arg in req.args %}
            {{arg.type_out}}{% if not loop.last %},{% endif %} 
{% endfor %}
        >;

        using resp_type = {{req.return_type}};

        constexpr int since() const
        {
            return {{req.since}};
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::{{req.name}}");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
{% endfor %}
}

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
            libnvc::io_device *m_iodev;

        private:
            std::map<int64_t, std::function<void(libnvc::resp_variant)>> m_onresp;
            std::map<int64_t, std::function<void(int64_t, std::string)>> m_onresperr;

        public:
            api_client(libnvc::io_device *);

        public:
            // seems I can't use the default dtor
            // because of the incomplete type stream_decoder and unique_ptr
            ~api_client();

        private:
            const char *build_signature() const
            {
                return "{{build_signature}}";
            }

        public:
            int64_t seqid() const
            {
                return m_seqid;
            }

        private:
            void add_seqid(int64_t add)
            {
                m_seqid += add;
            }

        private:

        private:
            template<size_t reqid, typename on_resp_t> inline void regcb_resp(on_resp_t on_resp)
            {
                auto msg_id = libnvc::msgid(reqid, seqid());
                if(true /* on_resp */){
                    if(m_onresp.find(msg_id) != m_onresp.end()){
                        throw std::runtime_error(((std::string("response handler already resgistered: req = ") + libnvc::idstr(reqid)) + ", seqid = ") + std::to_string(seqid()));
                    }

                    m_onresp[msg_id] = [this, on_resp](libnvc::resp_variant result)
                    {
                        if constexpr (std::is_void_v<typename libnvc::req<reqid>::resp_type>){
                            on_resp();
                        }else{
                            on_resp(std::get<typename libnvc::req<reqid>::resp_type>(result));
                        }
                    };
                }
            }

            template<size_t reqid, typename on_resperr_t> inline void regcb_resperr(on_resperr_t on_resperr)
            {
                auto msg_id = libnvc::msgid(reqid, seqid());
                if(true /* on_resperr */){
                    if(m_onresperr.find(msg_id) != m_onresperr.end()){
                        throw std::runtime_error(((std::string("response error handler already resgistered: req = ") + libnvc::idstr(reqid)) + ", seqid = ") + std::to_string(seqid()));
                    }
                    m_onresperr[msg_id] = on_resperr;
                }
            }

        public:
            template<size_t reqid, typename on_resp_t> inline int64_t forward(const typename libnvc::req<reqid>::parms_tuple &parms, on_resp_t on_resp)
            {
                add_seqid(1);
                auto msgid = libnvc::msgid(reqid, seqid());

                libnvc::mpinterf::writer packer;
                auto fn_pack = [&packer](int64_t msgid, auto &&... args)
                {
                    return packer.pack_req(msgid, std::forward<decltype(args)>(args)...);
                };

                m_iodev->send(std::apply(fn_pack, std::tuple_cat(std::make_tuple(msgid), parms)));
                regcb_resp<reqid, on_resp_t>(on_resp);
                return msgid;
            }

            template<size_t reqid, typename on_resp_t, typename on_resperr_t> inline int64_t forward(const typename libnvc::req<reqid>::parms_tuple &parms, on_resp_t on_resp, on_resperr_t on_resperr)
            {
                add_seqid(1);
                auto msgid = libnvc::msgid(reqid, seqid());

                libnvc::mpinterf::writer packer;
                auto fn_pack = [&packer](int64_t msgid, auto &&... args)
                {
                    return packer.pack_req(msgid, std::forward<decltype(args)>(args)...);
                };

                m_iodev->send(std::apply(fn_pack, std::tuple_cat(std::make_tuple(msgid), parms)));
                regcb_resp<reqid, on_resp_t>(on_resp);
                regcb_resperr<reqid, on_resperr_t>(on_resperr);
                return msgid;
            }

        public:
            void poll()
            {
                while(poll_one()) continue;
            }

        public:
            int64_t poll_one();

        public:
            void poll_all()
            {
                while(!m_onresp.empty() || !m_onresperr.empty()){
                    poll();
                }
            }

        public:
            void poll_wait(int64_t msgid)
            {
                while(true){
                    if(msgid == poll_one()){
                        break;
                    }
                }
            }

        public:
            // blocking API
            // this may be necessary for requests like nvim_ui_attach()
            // when got error reported from nvim server these function throw [errc, errmsg]

{% for req in nvim_reqs %}
            {{req.return_type}} {{req.name}}({% for arg in req.args %}{{arg.type}} {{arg.name}}{% if not loop.last %}, {% endif %}{% endfor %})
            {
{% if req.return_type != 'void' %}
                {{req.return_type}} r;
                auto on_resp = [&r]({{req.return_type}} res)
                {
                    r = res;
                };
{% else %}
                auto on_resp = []()
                {
                };
{% endif %}
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request\"{{req.name}}\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("{{req.name}}");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple({% for arg in req.args %}{{arg.name}}{% if not loop.last %}, {% endif %}{% endfor %}), on_resp, on_resperr);
                poll_wait(msgid);
{% if req.return_type != 'void' %}
                return r;
{% else %}
                return;
{% endif %}
            }

{% endfor %}

        public:
            // ui notifications
            // notif doesn't have return value, do nothing by default
            // it's impossible for jinja2 to generate proper code for each notification
{% for notif in nvim_notifs %}
            virtual void on_{{notif.name}}({% for arg in notif.args%}{{arg.type}}{% if not loop.last %}, {% endif %}{% endfor %})
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: {{notif.name}}");
            }
{% endfor %}

    };
}

namespace libnvc
{
    class nvim_box: public api_client
    {
    };
}
