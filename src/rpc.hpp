/*
 * =====================================================================================
 *
 *       Filename: rpc.hpp
 *        Created: 12/26/2018 06:09:51
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

namespace libnvc
{
    enum
    {
        REQUEST  = 0,
        RESPONSE = 1,
        NOTIFY   = 2
    };

    class rpc
    {
        public:
            template<typename... U> msgpack::type::variant do_call(const std::string &method, const U&... u)
            {
                msgpack::sbuffer sbuf;
                msgpack::packer<msgpack::sbuffer> pk(&sbuf);

                pk.pack_array(4) << (uint64_t)(REQUEST) << msgid(1) << method;

                pk.pack_array(sizeof...(u));


            }

        private:
            uint64_t m_msgid;

    };
}




namespace detail {
    using Packer = msgpack::packer<msgpack::sbuffer>;

    template<class T>
    Packer& pack(Packer& pk, const T& t) {
          return pk << t;
    }
    
    template<class T1, class T2, class...T3>
    Packer& pack(Packer& pk, const T1 &t1, const T2 &t2, const T3 &...t3) {
          return pack(pack(pk, t1), t2, t3...);
    }

    static Packer& pack(Packer& pk) {
          return pk;
    }

} // namespace detail

template<typename T, typename...U> void call(const std::string &method, T& res, const U&...u)
{
    msgpack::type::variant v = do_call(method, u...);
    libnvc::log(LIBNVC_LOG_INFO, str_fflprintf("T call").c_str());

    return 
    std::cout << "T call" << std::endl;
    
    res = boost::get<T>(v);
}

template<typename...U>
void call(const std::string &method, Integer& res, const U& ...u) {
    msgpack::type::variant v = do_call(method, u...);
    std::cout << "Integer call" << std::endl;
    
    // int64_t is only for negative integer.
    if(v.is_int64_t())       res = v.as_int64_t();
    else if(v.is_uint64_t()) res = v.as_uint64_t();
    else std::cout << "invalid response type" << std::endl; //TODO: add error handler
}

template<typename...U>
void call(const std::string &method, msgpack::type::variant& res, const U& ...u) {
    msgpack::type::variant v = do_call(method, u...);
    std::cout << "msgpack::type::variant call" << std::endl;
    res = v;
}

template<typename...U>
void call(const std::string &method, nullptr_t res, const U&...u) {
    do_call(method, u...);
    std::cout << "void call" << std::endl;
}



template<typename...U> msgpack::type::variant do_call(const std::string &method, const U&...u)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk(&sbuf);

    pk.pack_array(4) << (uint64_t)(REQUEST) << msgid(1) << method;

    pk.pack_array(sizeof...(u));
    detail::pack(pk, u...);

    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());

    msgpack::object deserialized = oh.get();

    std::cout << "sbuf = " << deserialized << std::endl;

    socket_.write(sbuf.data(), sbuf.size(), 5);

    msgpack::unpacker unpacker;
    unpacker.reserve_buffer(32*1024ul);

    size_t rlen = socket_.read(unpacker.buffer(), unpacker.buffer_capacity(), 5);
    msgpack::unpacked result;
    unpacker.buffer_consumed(rlen);

    /*
       while(unpacker.next(result)) {
       const msgpack::object &obj = result.get();
       std::cout << "res = " << obj << std::endl;
       result.zone().reset();
       }
       */

    //TODO: full-state response handler should be implemented
    unpacker.next(result);
    const msgpack::object &obj = result.get();
    std::cout << "res = " << obj << std::endl;
    msgpack::type::tuple<int64_t, int64_t, msgpack::type::variant, msgpack::type::variant> dst;
    obj.convert(dst);
    return dst.get<3>();
}
}

int32_t msgid(int advance)
{
    m_msgid += advance;
    return m_msgid;
}

int32_t msgid()
{
    return m_msgid;
}

class databuf
{
    private:
        char  *m_data;
        size_t m_size;

    public:
        mpack_databuf()
            : m_data(nullptr)
            , m_size(0)
        {}

    public:
        ~mpack_databuf()
        {
            if(m_data){
                MPACK_FREE(m_data);
            }
        }

    public:
        const char *data() const
        {
            return m_data;
        }

        size_t size() const
        {
            return m_size;
        }

    public:
        char * &data_ref()
        {
            return m_data;
        }

        size_t size_ref()
        {
            return m_size;
        }
};

template<typename... U> int32_t rpc::dispatch(const std::string& method, U... args)
{
    mpack_writer_t writer;

    detail::databuf buf;
    mpack_writer_init_growable(&writer, &(buf.data_ref()), &(buf.size_ref()));

    const int32_t type = REQUEST;
    const int32_t id   = msgid(1);

    mpack_start_array(&writer, 4);
    {
        mpack_write_i32 (&writer, type);
        mpack_write_i32 (&writer, id);
        mpack_write_cstr(&writer, method.c_str());

        mpack_start_array(&writer, sizeof...(U));
        {
            mpack_write(&writer, args...);
        }
        mpack_finish_array(&writer);
    }
    mpack_finish_array(&writer);

    if(mpack_writer_destroy(&writer) != mpack_ok){
        throw std::runtime_error(str_fflprintf(": Request packing failed"));
    }

    send(buf.data(), buf.size());
    return id;
}

void libnvc::type::poll_result rpc::poll()
{
    mpack_reader_t reader;
    mpack-reader_init_data(&reader, m_socket.buf_data(), m_socket.buf_size());
}

template<typename T> size_t rpc::poll_notify(int32_t msgid, int32_t timeout)
{
    perf_timer timer;
}

template<typename T> size_t rpc::poll_response(int32_t msgid, int32_t timeout)
{
    perf_timer timer;
}
