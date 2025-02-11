export module Kiss.Kernel:main;

import Kiss.Base;
import Kiss.SBI;
import :panic;
import :exception;

extern "C" char __bss[], __bss_end[], __stack_top[];

extern "C" void _kissEntry(void) {
    //memset(__bss, 0, reinterpret_cast<Kiss::usize>(__bss_end) - reinterpret_cast<Kiss::usize>(__bss));
    Kiss::SBI::consolePrintf("Bruh2\n"s);
    csrw(
        Kiss::Kernel::Asm::Csr::STVEC,
        reinterpret_cast<Kiss::usize>(Kiss::Kernel::_kexception)
    );
    Kiss::Kernel::Asm::unimp();

    Kiss::Kernel::panic("Yipee"s);
}
