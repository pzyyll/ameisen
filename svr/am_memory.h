//
// Created by czl17 on 2017-12-26.
//

#ifndef AM_SVR_AM_MEMORY_H
#define AM_SVR_AM_MEMORY_H

#include <utility>
#include <boost/smart_ptr.hpp>
#include <boost/type_traits.hpp>

#include "am_malloc.h"

namespace am {

template <typename Type, typename... Args>
boost::shared_ptr<Type> MakeShared(Args &&... args) {
    void *ptr = AM_malloc(sizeof(Type));
    return boost::shared_ptr<Type>(new(ptr) Type(std::forward<Args>(args)...), [](Type *ptr) {
        if (!ptr)
            return;
        if (boost::is_class<Type>::value)
            ptr->~Type();
        AM_free(ptr);
    });
};

}

#endif //AM_SVR_AM_MEMORY_H
