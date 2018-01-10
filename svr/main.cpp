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
#include "session_mng.h"

using namespace std;
using namespace am;

class EchoSession : public Session {
public:
    explicit EchoSession(boost::asio::io_service &io_svr) : Session(io_svr) { }
    explicit EchoSession(boost::asio::ip::tcp::socket sock) : Session(std::move(sock)) { }

    int Do(const char *data, std::size_t len) override {
        string str(data, len);
        cout << "recv: " << str << endl;
        cout << this_thread::get_id() << endl;
        AsyncSend(data, len);
        return len;
    }

    void Close() override {
        Session::Close();
        SessionMngSingleton::get_mutable_instance().DelSession(SessionId());
    }
};

class Server {
public:
    Server(boost::asio::io_service &io_svr,
           const boost::asio::ip::tcp::endpoint &ep)
        : acceptor_(io_svr, ep), peer_socket_(io_svr) {
        DoAccept();
    }

    void DoAccept() {
        acceptor_.async_accept(peer_socket_, peer_endpoint_, boost::bind(HandleAccept, this, _1));
    }

    void HandleAccept(boost::system::error_code ec) {
        boost::mutex::scoped_lock lock(mutex_);
        if (!ec) {
            cout << "accept_begin" << endl;
            boost::shared_ptr<Session> sess_ptr = MakeShared<EchoSession>(std::move(peer_socket_));
            SessionMngSingleton::get_mutable_instance().AddSession(sess_ptr);
            cout << "connect num: " << SessionMngSingleton::get_mutable_instance().Size() << endl;
            cout << peer_endpoint_.address().to_string() << ":" << peer_endpoint_.port() << endl;
            cout << std::this_thread::get_id() << endl;

            sess_ptr->Start();

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
