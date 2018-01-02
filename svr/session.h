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
        virtual ~Session();

        /**
         * 接收数据后实际执行的动作
         * @param data
         * @return 解析掉的数据, 包尚未接收完整返回 0, 包错误及其它出错情况返回 -1.
         */
        virtual std::size_t Do(const char *data, std::size_t len);

        void Start();

        /**
         * @todo stop
         */
        void Stop();

        void AsyncSend(const char *data, std::size_t len);

        boost::asio::ip::tcp::socket &Socket();

        unsigned int SessionId() const;

    private:
        void DoRead();

        void HandlerRead(const boost::system::error_code &ec, std::size_t size);

        void HandlerWrite(const boost::system::error_code &ec, std::size_t size);

        static unsigned int GenSessionId();

    private:
        boost::asio::ip::tcp::socket socket_;
        unsigned int session_id_;

        Buffer read_buf_, write_buf_;
    };
} //namespace am


#endif //AM_SVR_SESSION_H
