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
#include "strfunc.hpp"

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
            void spawn(int, char *argv[])
            {
                reproc::stop_actions stop_actions
                {
                    {reproc::stop::terminate, reproc::milliseconds(5000)},
                    {reproc::stop::kill,      reproc::milliseconds(2000)},
                    {},
                };

                reproc::options options;
                options.stop_actions = stop_actions;
                std::error_code ec = m_process.start(argv, options);

                if(ec == std::errc::no_such_file_or_directory){
                    throw std::runtime_error("neovim not found, make sure it's in PATH");
                }else if(ec){
                    throw std::runtime_error(ec.message());
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
            size_t send(const char *, size_t)
            {
                return 0;
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

void libnvc::reproc_device::spawn(int argc, char *argv[])
{
    m_impl->spawn(argc, argv);
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
