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
    constexpr String()
        : str(nullptr), len(0) {}

    constexpr explicit String(char const* str)
        : str(str), len(stringLength(str)) {}

    constexpr explicit String(char const* str, usize len)
        : str(str), len(len) {}

    constexpr bool operator==(const String& other) const {
        if (len != other.len)
            return false;
        auto* p1 = this->begin();
        auto* p2 = other.begin();
        while (p1 != this->end() && p2 != other.end()) {
            if (*p1 != *p2)
                return false;
            ++p1;
            ++p2;
        }
        return true;
    }

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
