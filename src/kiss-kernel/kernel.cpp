export module Kiss.Kernel:main;

import Kiss.Base;
import Kiss.SBI;
import :panic;
import :exception;
import :memory;
import :process;

extern "C" char __bss[], __bss_end[];

namespace Kiss::Kernel {
struct Process;

void delay(void) {
    for (int i = 0; i < 30000000; i++)
        __asm__ __volatile__("nop"); // do nothing
}

Process* procA;
Process* procB;

void procAEntry() {
    SBI::consolePrintf("starting process A\n"s);
    while (true) {
        SBI::consolePutchar('A');
        yield();
        delay();
    }
}

void procBEntry() {
    SBI::consolePrintf("starting process B\n"s);
    while (true) {
        SBI::consolePutchar('B');
        yield();
        delay();
    }
}

void entry() {
    SBI::consolePrintf("💋 Kiss Kernel v0.0.1\n"s);

    csrw(
        Riscv32::Csr::STVEC,
        reinterpret_cast<usize>(_kexception)
    );

    initProcess();

    procA = createProcess(reinterpret_cast<u32>(procAEntry));
    procB = createProcess(reinterpret_cast<u32>(procBEntry));
    yield();

    panic("unreachaeble"s);
}

} // namespace Kiss::Kernel

extern "C" void _kissEntry() {
    memset(__bss, 0, reinterpret_cast<Kiss::usize>(__bss_end) - reinterpret_cast<Kiss::usize>(__bss));
    Kiss::Kernel::entry();
}
