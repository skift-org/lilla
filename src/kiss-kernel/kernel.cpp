import Kiss.Base;

extern char __bss[], __bss_end[], __stack_top[];

namespace Kiss::Kernel {

extern "C" void _kissEntry(void) {
    memset(__bss, 0, (usize) __bss_end - (usize) __bss);
    for (;;);
}

} // namespace Kiss::Kernel
