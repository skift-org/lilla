export module Kiss.Kernel:panic;

import Kiss.Base;
import Kiss.SBI;
import :riscv32;

namespace Kiss::Kernel {

struct Format {
    String fmt;
    Loc loc;

    Format(String fmt, Loc loc = Loc::current()) : fmt(fmt), loc(loc) {}
};

export template <typename... Args>
[[noreturn]] void panic(Format msg = "An error occured"s, Args const&... args) {
    SBI::consolePrintf("\n\nPANIC !\n%s:%d:%d: (%s) "s, msg.loc.file, msg.loc.line, msg.loc.column, msg.loc.func);
    SBI::consolePrintf(msg.fmt, args...);
    for (;;) {
        Asm::wfi();
    }
}

}