#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <functional>
#include <thread>
#include <map>
#include <boost/smart_ptr.hpp>
//#include <boost/type_traits.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
//#include <boost/functional.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "am_memory.h"
#include "session.h"

using namespace std;
using namespace am;

std::map<int, boost::shared_ptr<Session>> sess_map;

void WriteHanlderm(const boost::system::error_code &ec, std::size_t bytes, boost::shared_ptr<Session> sess_ptr) {
    if (ec) {
        cerr << ec.message() << endl;
        sess_map.erase(sess_ptr->SessionId());
        sess_ptr->Socket().close();
    }
}

class Server {
public:
    Server(boost::asio::io_service &io_svr,
           const boost::asio::ip::tcp::endpoint &ep)
        : acceptor_(io_svr, ep), peer_socket_(io_svr) {
        DoAccept();
    }

    void DoAccept() {
        acceptor_.async_accept(peer_socket_, peer_endpoint_, boost::bind(HandleAccept, this, _1));
        //acceptor_.async_accept(peer_socket_, peer_endpoint_, [this](boost::system::error_code ec) {
        //    if (!ec) {
        //        cout << "accept_begin" << endl;
        //        cout << peer_endpoint_.address().to_string() << ":" << peer_endpoint_.port() << endl;
        //        cout << "accept_end" << endl;
        //        for (auto &x: sockets) {
        //            string str("hello.");
        //            x.send(boost::asio::buffer(str.c_str(), str.size()));
        //        }
        //        sockets.emplace_back(std::move(peer_socket_));
        //    }
        //    cout << "===" << endl;
        //    DoAccept();
        //});
    }

    void HandleAccept(boost::system::error_code ec) {
        boost::mutex::scoped_lock lock(mutex_);
        if (!ec) {
            cout << "accept_begin" << endl;
            boost::shared_ptr<Session> sess_ptr = MakeShared<Session>(std::move(peer_socket_));
            sess_map[sess_ptr->SessionId()] = sess_ptr;
            cout << peer_endpoint_.address().to_string() << ":" << peer_endpoint_.port() << endl;
            cout << std::this_thread::get_id() << endl;

            for (auto &x: sess_map) {
                string str("hello\n");
                x.second->Socket().async_send(boost::asio::buffer(str.c_str(), str.length()), boost::bind(WriteHanlderm, _1, _2, x.second));
            }
            cout << "accept_end" << endl;
        }
        cout << "===" << endl;
        DoAccept();
    }


private:
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket peer_socket_;
    boost::asio::ip::tcp::endpoint peer_endpoint_;
    boost::mutex mutex_;
};

int main() {
    try {
        struct sigaction sa;
        std::memset(&sa, 0, sizeof(sa));
        sa.sa_handler = SIG_IGN;
        assert(sigaction(SIGPIPE, &sa, NULL) == 0);

        boost::asio::io_service net_io;
        boost::asio::io_service::work work(net_io);
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 3399);

        Server svr(net_io, endpoint);
        for (int i = 0; i < 2; ++i) {
            MakeShared<boost::thread>(boost::bind(&boost::asio::io_service::run, &net_io))->detach();
        }
        net_io.run();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    cout << "endl" << endl;
    return 0;
}
