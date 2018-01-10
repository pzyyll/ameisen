//
// Created by czl17 on 2018-1-8.
//

#include "session_mng.h"

#include <iostream>

namespace am {
    SessionMng::SessionMng() {

    }

    SessionMng::~SessionMng() {

    }

    int SessionMng::AddSession(Session::Pointer session_ptr) {
        auto itr = socket_map_.find(session_ptr->SessionId());
        if (itr != socket_map_.end()) {
            std::cout << "Add failer, exist." << std::endl;
            return -1;
        }

        socket_map_.insert(std::make_pair(session_ptr->SessionId(), session_ptr));

        return 0;
    }

    void SessionMng::DelSession(int key_sess_id) {
        socket_map_.erase(key_sess_id);
    }

    void SessionMng::DelSession(Session::Pointer ptr) {
        DelSession(ptr->SessionId());
    }

    Session::Pointer SessionMng::GetSession(int key_sess_id) {
        auto itr = socket_map_.find(key_sess_id);
        if (itr != socket_map_.end())
            return itr->second;
        return am::Session::Pointer();
    }

    std::size_t SessionMng::Size() const {
        return socket_map_.size();
    }

} //namespace am

