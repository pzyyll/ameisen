//
// Created by czl17 on 2017-12-26.
//

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

    boost::asio::ip::tcp::socket &Session::Socket() {
        return socket_;
    }

    unsigned int Session::SessionId() const {
        return session_id_;
    }

    unsigned int Session::GenSessionId() {
        //@TODO : multi-thread safe
        static unsigned int cnt = 0;
        return cnt++;
    }


} //namespace am
