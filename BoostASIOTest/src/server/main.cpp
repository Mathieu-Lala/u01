#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>

namespace net = ::boost::asio;

int main()
{
    net::io_service ioservice;
    net::ip::tcp::endpoint tcp_endpoint{net::ip::tcp::v4(), 8080};
    net::ip::tcp::acceptor tcp_acceptor{ioservice, tcp_endpoint};
    net::ip::tcp::socket tcp_socket{ioservice};
    std::string data;

    tcp_acceptor.listen();
    tcp_acceptor.async_accept(tcp_socket, [&tcp_socket](const boost::system::error_code &ec){
        if (!ec)
            return;
        std::time_t now = std::time(nullptr);
        std::string data = std::ctime(&now);
        net::async_write(tcp_socket, net::buffer(data),
            [&tcp_socket](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                if (!ec)
                    tcp_socket.shutdown(net::ip::tcp::socket::shutdown_send);
            });
    });
    ioservice.run();

    return 0;
}
