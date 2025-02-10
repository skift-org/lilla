export module Kiss.Base:libc;

import :base;

export extern "C" void* memset(void* buf, char c, Kiss::usize n) {
    auto* p = static_cast<Kiss::u8*>(buf);
    while (n--)
        *p++ = c;
    return buf;
}

export extern "C" void* memcpy(void* dest, void const* src, Kiss::usize n) {
    auto* d = static_cast<Kiss::u8*>(dest);
    auto* s = static_cast<Kiss::u8 const*>(src);
    while (n--)
        *d++ = *s++;
    return dest;
}
