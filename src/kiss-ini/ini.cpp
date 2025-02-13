export module Kiss.Ini:main;

extern "C" char __bss[], __bss_end[];

namespace Kiss::Ini {

    int main() {
        for (;;);
    }

} // namespace Kiss::Kernel


export extern "C" [[noreturn]] void exit() {
    __builtin_unreachable();
}
export extern "C" void _iniMain() {
    Kiss::Ini::main();
}
