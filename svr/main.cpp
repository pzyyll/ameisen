#include <iostream>
#include <string>
#include <vector>
//#include <boost/smart_ptr.hpp>
//#include <boost/type_traits.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
//#include <boost/functional.hpp>
//#include <boost/bind.hpp>
//#include <boost/thread.hpp>
#include <utility>
#include <memory>

#include "am_memory.h"

using namespace std;
using namespace am;

class Server {
public:
    Server(boost::asio::io_service &io_svr,
           const boost::asio::ip::tcp::endpoint &ep)
        : acceptor_(io_svr, ep), socket_(io_svr) {
        DoAccept();
    }

    void DoAccept() {
        acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
            if (!ec) {
                cout << "accept" << endl;
                char data[1000] = {0};
                boost::asio::read(socket_, boost::asio::buffer(data, sizeof(data)));
                cout << data << endl;
            }
            cout << "===" << endl;
            DoAccept();
        });
    }

private:
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};

int main() {
    boost::asio::io_service net_io;
    boost::asio::io_service::work work(net_io);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 3399);

    Server svr(net_io, endpoint);
    net_io.run();

    cout << "endl" << endl;
    return 0;
}
