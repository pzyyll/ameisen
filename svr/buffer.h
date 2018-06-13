//
// Created by czl17 on 2017-12-27.
//

#ifndef AM_SVR_BUFFER_H
#define AM_SVR_BUFFER_H

#include <cstring>
#include <vector>

namespace am {
    struct Buffer {
        Buffer() : fpos(0), tpos(0) {
        }

        explicit Buffer(const unsigned long len)
                : fpos(0), tpos(0) {
            storage.reserve(len);
        }

        ~Buffer() {
        }

        unsigned long MaxSize() { return storage.capacity(); }
        unsigned long UsedSize() { return (unsigned long)(tpos - fpos); }
        unsigned long RemainSize() { return (unsigned long)(MaxSize() - tpos); }
        int Resize(unsigned long size) {
            if (size > MaxSize())
                storage.reserve(size);
            return 0;
        }
        void FrontAdvancing(unsigned long step_size) {
            if (fpos + step_size > tpos)
                return;
            fpos += step_size;
            if (fpos == tpos)
                fpos = tpos = 0;
        }
        void TailAdvancing(unsigned long step_size) {
            if (tpos + step_size > MaxSize())
                return;
            tpos += step_size;
        }
        char *FrontPos() { return (&storage[0] + fpos); }
        char *TailPos() { return (&storage[0] + tpos); }
        void MemoryMove2Left() {
            if (0 == fpos)
                return;
            memmove(&storage[0], &storage[0] + fpos, tpos - fpos);
            tpos -= fpos;
            fpos = 0;
        }

        std::size_t Append(const char *data, std::size_t len) {
            if (len > RemainSize())
                return static_cast<std::size_t>(-1);
            memcpy(TailPos(), data, len);
            TailAdvancing(len);
            return len;
        }

        unsigned long fpos, tpos;
        std::vector<char> storage;
    };
} //namespace am


#endif //AM_SVR_BUFFER_H
