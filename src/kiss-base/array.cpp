export module Kiss.Base:array;

import :base;

namespace Kiss {

export template <typename T, usize N>
struct Array {
    T data[N];

    [[nodiscard]] auto* begin(this auto& self) {
        return self.data;
    }

    [[nodiscard]] auto* end(this auto& self) {
        return self.data + N;
    }

    [[nodiscard]] auto& operator[](this auto& self, usize index)  {
        return self.data[index];
    }
};


template <typename T, typename... Ts>
Array(T, Ts...) -> Array<T, sizeof...(Ts) + 1>;

} // namespace Kiss
