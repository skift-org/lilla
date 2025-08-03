export module Lilla.Ini:main;

extern "C" char __bss[], __bss_end[];

namespace Lilla::Ini {

    int main() {
        for (;;);
    }

} // namespace Lilla::Kernel


export extern "C" [[noreturn]] void exit() {
    __builtin_unreachable();
}
export extern "C" void _iniMain() {
    Lilla::Ini::main();
}
