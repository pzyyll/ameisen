//
// Created by czl17 on 2018-1-8.
//

#ifndef AM_SVR_SESSION_MNG_H
#define AM_SVR_SESSION_MNG_H

#include <unordered_map>

#include <boost/serialization/singleton.hpp>

#include "session.h"

namespace am {

    class SessionMng {
    public:
        typedef std::unordered_map<int, Session::Pointer> SocketMap;
        typedef SocketMap::iterator SocketMapItr;

        SessionMng();
        ~SessionMng();

        int AddSession(Session::Pointer session_ptr);

        void DelSession(int key_sess_id);

        void DelSession(Session::Pointer ptr);

        Session::Pointer GetSession(int key_sess_id);

        std::size_t Size() const;

    private:
        SocketMap socket_map_;
    };

    typedef boost::serialization::singleton<SessionMng> SessionMngSingleton;

} //namespace am


#endif //AM_SVR_SESSION_MNG_H
