#include <websocketpp/config/bare_client.hpp>
#include <websocketpp/client.hpp>

#include <websocketpp/common/connection_hdl.hpp> // connection_hdl
#include <websocketpp/common/functional.hpp>     // bind

#include <websocketpp/concurrency/none.hpp> // 

#include <iostream>
#include <string>

#include <websocketpp/logger/syslog.hpp>
//#include <websocketpp/extensions/permessage_deflate/enabled.hpp>
struct custom_client_config : public websocketpp::config::bare_client {
    // Replace default stream logger with a syslog logger
    typedef websocketpp::log::syslog<concurrency_type, websocketpp::log::elevel> elog_type;
    typedef websocketpp::log::syslog<concurrency_type, websocketpp::log::alevel> alog_type;
 
    // Reduce read buffer size to optimize for small messages
    static const size_t connection_read_buffer_size = 1024;
    static bool const enable_multithreading = false;
 
 /*
    // enable permessage_compress extension
    struct permessage_deflate_config {};
 
    typedef websocketpp::extensions::permessage_deflate::enabled
        <permessage_deflate_config> permessage_deflate_type;
        */
};
 
typedef websocketpp::client<custom_client_config> client;
//typedef websocketpp::client<websocketpp::config::core_client> client;

class connection_metadata
{
public:
    typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;

    connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri)
        : m_id(id), m_hdl(hdl), m_status("Connecting"), m_uri(uri), m_server("N/A")
    {
    }

    void on_open(client *c, websocketpp::connection_hdl hdl)
    {
        std::cout << "DDD connection_metadata::on_open" << std::endl;
        m_status = "Open";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
    }

    void on_fail(client *c, websocketpp::connection_hdl hdl)
    {
        std::cout << "DDD connection_metadata::on_fail" << std::endl;
        m_status = "Failed";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        m_server = con->get_response_header("Server");
        m_error_reason = con->get_ec().message();

        std::cout << "DDD    Error reason: " << m_error_reason << std::endl;
    }

    // TODO what does this do?
    friend std::ostream &operator<<(std::ostream &out, connection_metadata const &data);

private:
    int m_id = 0;
    websocketpp::connection_hdl m_hdl;
    std::string m_status = "";
    std::string m_uri = "";
    std::string m_server = "";
    std::string m_error_reason = "";
};

std::ostream &operator<<(std::ostream &out, connection_metadata const &data)
{
    out << "> URI: " << data.m_uri << "\n"
        << "> Status: " << data.m_status << "\n"
        << "> Remote Server: " << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
        << "> Error/close reason: " << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason);

    return out;
}

class websocket_endpoint
{
public:
    void endpoint_on_open(websocketpp::connection_hdl hdl)
    {
        std::cout << "DDD endpoint_on_open()" << std::endl;
        //m_connections.insert(hdl);
    }

    void endpoint_on_close(websocketpp::connection_hdl hdl)
    {
        std::cout << "DDD endpoint_on_close()" << std::endl;
        //m_connections.erase(hdl);
    }

    void endpoint_on_fail(websocketpp::connection_hdl hdl)
    {
        std::cout << "DDD endpoint_on_fail()" << std::endl;
        //m_connections.erase(hdl);
    }


    void endpoint_on_init(websocketpp::connection_hdl hdl, client::message_ptr msg)
    {
        std::cout << "DDD endpoint_on_init()" << std::endl;
    }
    void endpoint_on_message(websocketpp::connection_hdl hdl, client::message_ptr msg)
    {
        std::cout << "DDD endpoint_on_message()" << std::endl;
    }
    bool endpoint_on_ping(websocketpp::connection_hdl hdl, std::string pingMsg)
    {
        std::cout << "DDD endpoint_on_ping()" << std::endl;
        return(true);
    }
    void endpoint_on_pong(websocketpp::connection_hdl hdl, std::string pongMsg)
    {
        std::cout << "DDD endpoint_on_pong()" << std::endl;
    }


    websocket_endpoint()
    {
        m_client_endpoint.clear_access_channels(websocketpp::log::alevel::all);
        m_client_endpoint.set_access_channels(websocketpp::log::alevel::connect);
        m_client_endpoint.set_access_channels(websocketpp::log::alevel::disconnect);
        m_client_endpoint.set_access_channels(websocketpp::log::alevel::app);

        //m_client_endpoint.clear_error_channels(websocketpp::log::elevel::all);
        //m_thread.reset(new websocketpp::lib::thread(&client::run, &m_client_endpoint));

        std::cout << "DDD set up handler for the end point." << std::endl;
        m_client_endpoint.set_close_handler(websocketpp::lib::bind(&websocket_endpoint::endpoint_on_close, this, websocketpp::lib::placeholders::_1));
        m_client_endpoint.set_fail_handler(websocketpp::lib::bind(&websocket_endpoint::endpoint_on_fail, this, websocketpp::lib::placeholders::_1));
        //m_client_endpoint.set_http_handler(websocketpp::lib::bind(&websocket_endpoint::endpoint_on_http, this, websocketpp::lib::placeholders::_1));
        m_client_endpoint.set_message_handler(websocketpp::lib::bind(&websocket_endpoint::endpoint_on_message, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
        m_client_endpoint.set_open_handler(websocketpp::lib::bind(&websocket_endpoint::endpoint_on_open, this, websocketpp::lib::placeholders::_1));
        m_client_endpoint.set_ping_handler(websocketpp::lib::bind(&websocket_endpoint::endpoint_on_ping, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
        m_client_endpoint.set_pong_handler(websocketpp::lib::bind(&websocket_endpoint::endpoint_on_pong, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
        //m_client_endpoint.set_shutdown_handler(websocketpp::lib::bind(&websocket_endpoint::endpoint_on_pong, this, websocketpp::lib::placeholders::_1));
        //m_client_endpoint.set_init_handler(websocketpp::lib::bind(&websocket_endpoint::endpoint_on_init, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
    }

    int connect(std::string const &uri)
    {
        websocketpp::lib::error_code ec;

        client::connection_ptr webSocketConnection = m_client_endpoint.get_connection(uri, ec);
        //webSocketConnection->

        //con->set_open_handler(websocketpp::lib::bind(&websocket_endpoint::on_open, this, websocketpp::lib::placeholders::_1));
        //con->set_fail_handler(websocketpp::lib::bind(&websocket_endpoint::on_fail, this, websocketpp::lib::placeholders::_1));

        if (ec)
        {
            std::cout << "> Connect initialization error: " << ec.message() << std::endl;
            return -1;
        }

        int new_id = m_next_id++;
        connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(new_id, webSocketConnection->get_handle(), uri);
        m_connection_list[new_id] = metadata_ptr;

        webSocketConnection->set_open_handler(websocketpp::lib::bind(
            &connection_metadata::on_open,
            metadata_ptr,
            &m_client_endpoint,
            websocketpp::lib::placeholders::_1));
        webSocketConnection->set_fail_handler(websocketpp::lib::bind(
            &connection_metadata::on_fail,
            metadata_ptr,
            &m_client_endpoint,
            websocketpp::lib::placeholders::_1));

        m_client_endpoint.connect(webSocketConnection);

        return new_id;
    }

    connection_metadata::ptr get_metadata(int id) const
    {
        con_list::const_iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end())
        {
            return connection_metadata::ptr();
        }
        else
        {
            return metadata_it->second;
        }
    }

private:
    typedef std::map<int, connection_metadata::ptr> con_list;

    client m_client_endpoint;
    // websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    con_list m_connection_list;
    int m_next_id = 0;
};

int main()
{
    bool done = false;
    std::string input;
    websocket_endpoint endpoint;

    while (!done)
    {
        std::cout << "Enter Command: ";
        std::getline(std::cin, input);

        if (input == "quit")
        {
            done = true;
        }
        else if (input == "help")
        {
            std::cout
                << "\nCommand List:\n"
                << "connect <ws uri>\n"
                << "show <connection id>\n"
                << "help: Display this help text\n"
                << "quit: Exit the program\n"
                << std::endl;
        }
        else if (input.substr(0, 7) == "connect")
        {
            // TODO how do I register an output stream? I assume this is setting up a TCP connection
            int id = endpoint.connect(input.substr(8));
            if (id != -1)
            {
                std::cout << "> Created connection with id " << id << std::endl;
            }
        }
        else if (input.substr(0, 4) == "show")
        {
            int id = atoi(input.substr(5).c_str());

            connection_metadata::ptr metadata = endpoint.get_metadata(id);
            if (metadata)
            {
                std::cout << *metadata << std::endl;
            }
            else
            {
                std::cout << "> Unknown connection id " << id << std::endl;
            }
        }
        else
        {
            std::cout << "Unrecognized Command" << std::endl;
        }
    }

    return 0;
}