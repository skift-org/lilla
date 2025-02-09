#include "libc.h"

extern "C" void* memset(void* buf, char c, Kiss::usize n) {
    auto* p = static_cast<Kiss::u8*>(buf);
    while (n--)
        *p++ = c;
    return buf;
}
