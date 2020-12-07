#include "boost/asio.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include "../include/lib.h"
#include <vector>

using namespace boost::asio;
using std::cout;
using std::endl;

using boost::asio::ip::tcp;

class Session
        : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(tcp::socket socket, uint batch_size)
            : socket_(std::move(socket)),
            _batch_size(batch_size)
    {
    }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                [this, self](boost::system::error_code ec, std::size_t length)
                                {
                                    if (!ec)
                                    {
                                        std::string str_data = std::string{data_, length};

                                        typedef std::vector< std::string > split_vector_type;
                                        split_vector_type commands; // #2: Search for tokens
                                        split(commands, str_data, boost::algorithm::is_any_of("\n") );

//
//                                        std::vector<std::string> testSeries1 = {"cmd1", "cmd2", "cmd3", "cmd4", "cmd5"};
//                                        std::vector<std::string> testSeries2 = {"cmd1", "cmd2", "{", "cmd3", "cmd4", "}",
//                                                                                "{",  "cmd5", "cmd6", "{", "cmd7", "cmd8", "}", "cmd9", "}",
//                                                                                "{",  "cmd10", "cmd11"};
//
//                                        auto newEnd = std::remove_if(commands.begin(), commands.end(), [](std::string & s) {
//                                            return s=="";
//                                        });
//                                        commands.erase(newEnd, commands.end());
//
//                                        uint context1 = connect(_batch_size);
//                                        uint context2 = connect(_batch_size);
//
//                                        for (auto & command : testSeries1)
//                                            recieve(command.c_str(), command.size(), context1);
//                                        for (auto & command : testSeries2)
//                                            recieve(command.c_str(), command.size(), context2);
//                                        disconnect(context1);
//                                        disconnect(context2);

                                        uint context = connect(_batch_size);
//
                                        for (auto & command : commands)
                                            recieve(command.c_str(), command.size(), context);
                                        disconnect(context);

                                    }
                                });
    }

//    void do_write(std::size_t length)
//    {
//        auto self(shared_from_this());
//        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
//                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
//                                 {
//                                     if (!ec)
//                                     {
//                                         do_read();
//                                     }
//                                 });
//    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    uint _batch_size;
};

class Server
{
public:
    explicit Server(short port, uint batch_size)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
              _batch_size(batch_size)
    {
    }

    void run() {
        do_accept();
        io_context.run();
    }

private:
    boost::asio::io_context io_context;

    void do_accept()
    {
        acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket)
                {
                    if (!ec)
                    {
                        std::make_shared<Session>(std::move(socket), _batch_size)->start();
                    }

                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
    uint _batch_size;
};




int main(int argc, char *argv[])
{
    uint batch_size = 3;
    if (argc == 2)
    {
        std::istringstream iss( argv[1] );
        if (!(iss >> batch_size))
        {
            cout << "Bad batch size" << endl;
            return -1;
        }
    }

    Server server(9000, batch_size);
    server.run();
    return 0;
}
