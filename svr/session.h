//
// Created by czl17 on 2017-12-26.
//

#ifndef AM_SVR_SESSION_H
#define AM_SVR_SESSION_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "buffer.h"

namespace am {

    class Session : public boost::enable_shared_from_this<Session> {
    public:
        enum {
            kMaxBufferLenth = 1024000,
        };
        explicit Session(boost::asio::io_service &io_service);
        explicit Session(boost::asio::ip::tcp::socket socket);
        ~Session();

        /**
         * @todo start async_read
         */
        void Start();

        /**
         * @todo send msg
         */
        void AsyncSend(const std::string &data);

        boost::asio::ip::tcp::socket &Socket();

        unsigned int SessionId() const;

    private:
        /**
         * @todo
         */
        void DoRead();

        static unsigned int GenSessionId();

    private:
        boost::asio::ip::tcp::socket socket_;
        unsigned int session_id_;

        // @todo add rw buffer
        Buffer read_buf_, write_buf_;
    };
} //namespace am


#endif //AM_SVR_SESSION_H
