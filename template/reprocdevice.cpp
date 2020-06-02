/*
 * =====================================================================================
 *
 *       Filename: reprocdevice.cpp
 *        Created: 11/29/2019 20:38:14
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

#include <mutex>
#include <string>
#include <future>
#include <system_error>
#include <reproc++/reproc.hpp>
#include <reproc++/sink.hpp>
#include "libnvc.hpp"
#include "fflerror.hpp"

namespace libnvc
{
    class reproc_device::reproc_device_impl
    {
        private:
            reproc::process m_process;

        private:
            std::mutex m_lock;
            std::string m_output;

        private:
            std::future<std::error_code> m_drain_async;

        public:
            reproc_device_impl() = default;

        public:
            void spawn(char *argv[])
            {
                reproc::stop_actions stop_actions
                {
                    {reproc::stop::terminate, reproc::milliseconds(5000)},
                    {reproc::stop::kill,      reproc::milliseconds(2000)},
                    {},
                };

                reproc::options options;
                options.stop_actions = stop_actions;

                const auto ec = [argv, &options, this]()
                {
                    if(argv){
                        return m_process.start(argv, options);
                    }else{
                        std::vector<const char *> spawn_args
                        {
                            "nvim", "--embed", nullptr,
                        };
                        return m_process.start(spawn_args.data(), options);
                    }
                }();

                if(ec == std::errc::no_such_file_or_directory){
                    throw fflerror("neovim not found, make sure it's in PATH");
                }
                else if(ec){
                    throw fflerror(ec.message());
                }

                m_drain_async = std::async(std::launch::async, [this]()
                {
                    reproc::sink::thread_safe::string sink(m_output, m_output, m_lock);
                    return m_process.drain(sink);
                });
            }

            void kill()
            {
            }

        public:
            size_t send(const char *buf, size_t size)
            {
                const auto ec = m_process.write(reinterpret_cast<const uint8_t *>(buf), size);
                if(ec){
                    throw fflerror(ec.message());
                }
                return size;
            }

            size_t recv(char *buf, size_t size)
            {
                std::lock_guard<std::mutex> lockGuard(m_lock);
                const auto read_size = std::min<size_t>(size, m_output.size());
                std::memcpy(buf, m_output.data(), read_size);
                m_output.erase(m_output.begin(), m_output.begin() + read_size);
                return read_size;
            }
    };
}

libnvc::reproc_device::reproc_device()
    : m_impl(std::make_unique<reproc_device_impl>())
{}

libnvc::reproc_device::~reproc_device()
{}

void libnvc::reproc_device::spawn(char *argv[])
{
    m_impl->spawn(argv);
}

void libnvc::reproc_device::kill()
{
    m_impl->kill();
}

size_t libnvc::reproc_device::send(const char *buf, size_t size)
{
    return m_impl->send(buf, size);
}

size_t libnvc::reproc_device::recv(char *buf, size_t size)
{
    return m_impl->recv(buf, size);
}
