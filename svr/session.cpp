//
// Created by czl17 on 2017-12-26.
//

#include <iostream>
#include <boost/bind.hpp>
//#include <boost/asio/error.hpp>
#include "session.h"

namespace am {

    Session::Session(boost::asio::io_service &io_service)
        : socket_(io_service),
          session_id_(GenSessionId()),
          read_buf_(kMaxBufferLenth),
          write_buf_(kMaxBufferLenth) {

    }

    Session::Session(boost::asio::ip::tcp::socket socket)
        : socket_(std::move(socket)),
          session_id_(GenSessionId()),
          read_buf_(kMaxBufferLenth),
          write_buf_(kMaxBufferLenth) {

    }

    Session::~Session() {

    }

    int Session::Do(const char *data, std::size_t len) {

    }

    void Session::Close() {
        Stop();
    }

    void Session::Start() {
        DoRead();
    }

    void Session::Stop() {
        socket_.close();
        //socket_.shutdown(boost::asio::socket_base::shutdown_both);
    }

    boost::asio::ip::tcp::socket &Session::Socket() {
        return socket_;
    }

    unsigned int Session::SessionId() const {
        return session_id_;
    }

    void Session::DoRead() {
        socket_.async_read_some(boost::asio::buffer(read_buf_.TailPos(), read_buf_.RemainSize()),
                                boost::bind(&Session::HandlerRead, shared_from_this(), _1, _2));
    }

    void Session::HandlerRead(const boost::system::error_code &ec, std::size_t size) {
        if (ec == boost::asio::error::would_block || ec == boost::asio::error::try_again) {
            DoRead();
            return;
        }

        if (ec) {
            std::cerr << ec.message() << std::endl;
            Close();
            return;
        }

        boost::mutex::scoped_lock lock(rmutex_);
        read_buf_.TailAdvancing(size);
        int rs = Do(read_buf_.FrontPos(), read_buf_.UsedSize());
        if (rs < 0) {
            std::cerr << "Recv pkg info err." << std::endl;
            Close();
            return;
        }
        read_buf_.FrontAdvancing(static_cast<unsigned>(rs));
        read_buf_.MemoryMove2Left();
        DoRead();
    }

    unsigned int Session::GenSessionId() {
        //@TODO : multi-thread safe
        static unsigned int cnt = 0;
        return cnt++;
    }

    void Session::AsyncSend(const char *data, std::size_t len) {
        //socket_.async_write_some()
        boost::mutex::scoped_lock lock(wmutex_);
        write_buf_.Append(data, len);
        lock.unlock();
        boost::asio::async_write(socket_, boost::asio::buffer(write_buf_.FrontPos(), write_buf_.UsedSize()),
                                 boost::bind(&Session::HandlerWrite, shared_from_this(), _1, _2));
    }

    void Session::HandlerWrite(const boost::system::error_code &ec, std::size_t size) {
        if (ec == boost::asio::error::would_block || ec == boost::asio::error::try_again) {
            return;
        }

        if (ec) {
            std::cerr << ec.message() << std::endl;
            Close();
            return;
        }

        boost::mutex::scoped_lock lock(wmutex_);
        write_buf_.FrontAdvancing(size);
        write_buf_.MemoryMove2Left();
    }
} //namespace am
