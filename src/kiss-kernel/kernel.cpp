import Kiss.Base;
import Kiss.SBI;

extern char __bss[], __bss_end[], __stack_top[];

namespace Kiss::Kernel {

extern "C" void _kissEntry(void) {
    SBI::consolePuti(-4269, 16);

    for (;;) {
        __asm__ __volatile__("wfi");
    }
}

} // namespace Kiss::Kernel
