/*
 * =====================================================================================
 *
 *       Filename: reqdef.jinja2.cpp
 *        Created: 12/30/2018 04:48:23
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

#include <string>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include "mpack.h"
#include "typedef.hpp"
#include "strfunc.hpp"
#include "mpinterf.hpp"

// we can have pack_req<libnvc::req<reqid>::parms_t>(...) purely in template
// then get rid of this .cpp and jinja templates, this requires:
//
//     1. a helper class to isolate mpack_writer_t
//     2. make libnvc::req<reqid>::parms_t as std::tuple
//
// an implementation looks like
//
// template<> struct req<reqid>
// {
//     using parms_t = std::tuple<int, std::string>;
//     ...
// };
//
// template<typename T> std::string pack_req(typename T::parms_t parms)
// {
//     mpinterf::writer_helper helper;
//     helper.start_array(4);
//     {
//         helper.write(REQ);
//         helper.write(msgid);
//         helerp.write("req_name");
//         helper.start_array(std::tuple_size<typename T::parms_t>());
//         {
//             if(std::is_same_v<typename T::parms_t, std::tuple<libnvc::nil_t>){
//                 helper.write(nullptr);
//             }else{
//                 std::invoke(helper.write, parms);
//             }
//         }
//         helper.done_array();
//     }
//     helper.done_array();
//     return helper.pack_data();
// }

{% for req in nvim_reqs %}
std::string libnvc::req<libnvc::reqid("{{req.name}}")>::parms_t::pack(int64_t msgid) const
{
    char  *data = nullptr;
    size_t size = 0;

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &data, &size);

    mpack_start_array(&writer, 4);
    {
        mp_write(&writer, libnvc::REQ);
        mp_write(&writer, msgid);
        mp_write(&writer, "{{req.name}}");
        mpack_start_array(&writer, {{req.args|length}});
        {
{% for arg in req.args %}
            mp_write(&writer, {{arg.name}});
{% else %}
            mp_write(&writer, nullptr);
{% endfor %}
        }
        mpack_finish_array(&writer);
    }
    mpack_finish_array(&writer);

    if(mpack_writer_destroy(&writer) != mpack_ok){
        throw std::runtime_error("failed to pack req::{{req.name}}");
    }

    std::string buf{data, data + size};
    MPACK_FREE(data);

    return buf;
}
{% endfor %}
