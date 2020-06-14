#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace net = ::boost::asio;

int main()
{
    net::io_service io_service{};

    net::ip::tcp::resolver resolver{io_service};
    net::ip::tcp::resolver::query query{"127.0.0.1", "8080"};
    auto endpoint_iterator = resolver.resolve(query);

    net::ip::tcp::socket socket{io_service};
    boost::asio::connect(socket, endpoint_iterator);

    while (true) {
        boost::array<char, 128> buf;
        boost::system::error_code error;

        auto len = socket.read_some(boost::asio::buffer(buf), error);

        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        std::cout.write(buf.data(), len);
    }

    return 0;
}
