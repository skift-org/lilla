export module Kiss.Base:string;

import :base;

namespace Kiss {

constexpr usize stringLength(char const* s) {
    usize len = 0;
    while (s[len] != '\0') {
        ++len;
    }
    return len;
}

export struct String {
private:
    char const* str;
    usize len;

public:
    constexpr explicit String(char const* str)
        : str(str), len(stringLength(str)) {}

    constexpr explicit String(char const* str, usize len)
        : str(str), len(len) {}

    [[nodiscard]] const char* begin() const {
        return str;
    }

    [[nodiscard]] const char* end() const {
        return str + len;
    }

};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuser-defined-literals"
export constexpr String operator""s(char const* str, usize len) {
    return String(str, len);
}
#pragma clang diagnostic pop

} // namespace Kiss
