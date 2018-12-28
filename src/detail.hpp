/*
 * =====================================================================================
 *
 *       Filename: detail.hpp
 *        Created: 12/27/2018 03:01:51
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

namespace libnvc::detail
{
    template<typename T> T mp_read(mpack_reader_t *)
    {
        static_assert(false, "Reach undefined mp_read<T>(mpack_reader_t *)");
    }

    template<> bool mpack_read<bool>(mpack_reader_t *reader)
    {
        return mpack_expect_bool(reader);
    }

    template<> int64_t mpack_read<int64_t>(mpack_reader_t* reader)
    {
        return mpack_expect_i64(reader);
    }

    template<> double mpack_read<double>(mpack_reader_t* reader)
    {
        return mpack_expect_double(reader);
    }

    template<> std::string mpack_read<std::string>(mpack_reader_t* reader)
    {
        // TODO: we may get very long string
        // remember to take care of mpack_error_too_big
        char buf[1024];
        mpack_expect_utf8_cstr(reader, buf, sizeof(buf));
        return std::string(buf);
    }

    template<> libnvc::object mpack_read<libnvc::object>(mpack_reader_t* reader)
    {
        return mpack_read_object(reader);
    }

    template<> std::array<int32_t, 2> mpack_read<std::array<int32_t, 2>(mpack_reader_t* reader)
    {
        int32_t x = mpack_expect_i32(reader);
        int32_t y = mpack_expect_i32(reader);
        return {x, y};
    }

    template<> std::unordered_map<std::string, libnvc_object> mpack_read<std::unordered_map<std::string, Object>>(mpack_reader_t* reader)
    {
            const size_t numElements = mpack_expect_map(reader);

            std::unordered_map<std::string, Object> options{numElements};

            char buffer[512];
            for(int i = 0; i < numElements; ++i) {
                mpack_expect_cstr(reader, buffer, sizeof(buffer));
                options[std::string(buffer)] = mpack_read_object(reader);
            }

            mpack_done_map(reader);
            return options;
        }

    template<typename T>
        Containers::Array<T> mpack_read_array(mpack_reader_t* reader) {
            const size_t numElements = mpack_expect_array(reader);

            Containers::Array<T> data{Containers::DefaultInit, numElements};
            for(int i = 0; i < numElements; ++i) {
                data[i] = mpack_read<T>(reader);
            }

            mpack_done_array(reader);
            return data;
        }
    template<>
        Containers::Array<Long> mpack_read<Containers::Array<Long>>(mpack_reader_t* reader) {
            return mpack_read_array<Long>(reader);
        }
    template<>
        Containers::Array<Object> mpack_read<Containers::Array<Object>>(mpack_reader_t* reader) {
            return mpack_read_array<Object>(reader);
        }
    template<>
        Containers::Array<std::string> mpack_read<Containers::Array<std::string>>(mpack_reader_t* reader) {
            return mpack_read_array<std::string>(reader);
        }
    template<>
        Containers::Array<std::unordered_map<std::string, Object>> mpack_read<Containers::Array<std::unordered_map<std::string, Object>>>(mpack_reader_t* reader) {
            return mpack_read_array<std::unordered_map<std::string, Object>>(reader);
        }
}
