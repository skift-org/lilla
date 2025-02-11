export module Kiss.Kernel:main;

import Kiss.Base;
import Kiss.SBI;
import :panic;
import :exception;

extern "C" char __bss[], __bss_end[];

namespace Kiss::Kernel {

void entry() {
    SBI::consolePrintf("💋 Kiss Kernel v0.0.1\n"s);

    csrw(
        Asm::Csr::STVEC,
        reinterpret_cast<usize>(_kexception)
    );

    Asm::unimp();

    panic("Yipee"s);
}

} // namespace Kiss::Kernel

extern "C" void _kissEntry() {
    memset(__bss, 0, reinterpret_cast<Kiss::usize>(__bss_end) - reinterpret_cast<Kiss::usize>(__bss));
    Kiss::Kernel::entry();
}
