
#ifndef WEBSOCKETPP_COMMON_BARE_HPP
#define WEBSOCKETPP_COMMON_BARE_HPP

// TODO figure out what this is suppose to be used for

#include <chrono>

namespace websocketpp
{
    namespace lib
    {

        namespace bare
        {

            template <typename T>
            bool is_neg(T duration)
            {
                return duration.count() < 0;
            }

            typedef std::error_code error_code;

            // If boost believes it has std::chrono available it will use it
            // so we should also use it for things that relate to boost, even
            // if the library would otherwise use boost::chrono.
            inline std::chrono::milliseconds milliseconds(long duration)
            {
                return std::chrono::milliseconds(duration);
            }

            // TODO figure out what this should actually be
            typedef int *steady_timer;

            // TODO figure out what the asio io_service actually do, specifically the 'work'
            class io_context
            {
                class work;
            };
            typedef io_context io_service;
            // See: https://www.boost.org/doc/libs/1_67_0/boost/asio/io_context.hpp
            class io_context::work
            {
            public:
                work() {}
                void reset() {}
            };

            // https://www.boost.org/doc/libs/1_55_0/boost/asio/socket_base.hpp
            class socket_base
            {
                public:
                // TODO what should this number actualy bye
                int max_connections = 1;
            };

            // TODO replace this with something useful
            // See: https://www.boost.org/doc/libs/1_60_0/boost/asio/ip/tcp.hpp
            namespace ip
            {
                namespace tcp
                {
                    typedef int acceptor;
                    /// The type of a TCP endpoint.
                    // typedef basic_endpoint<tcp> endpoint;
                    typedef int endpoint;

                    /// The TCP resolver type.
                    //typedef basic_resolver<tcp> resolver;
                    typedef int resolver;

                } // namespace tcp
            }     // namespace ip

        } // namespace bare
    }     // namespace lib
} // namespace websocketpp

#endif // WEBSOCKETPP_COMMON_BARE_HPP