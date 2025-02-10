export module Kiss.Base:loc;

import :base;
import :string;

namespace Kiss {
export struct Loc {
    String file{};
    String func{};
    usize line{};
    usize column{};

    static constexpr Loc current(
        String file = String{__builtin_FILE()},
        String func = String{__builtin_FUNCTION()},
        usize line = __builtin_LINE(),
        usize column = __builtin_COLUMN()
    ) {
        return {file, func, line, column};
    }
};
}
