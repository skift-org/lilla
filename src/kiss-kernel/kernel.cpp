module;

#include <karm-base/base.h>
#include <karm-logger/logger.h>

export module Kiss.Kernel:main;

import Kiss.SBI;
import :exception;
import :memory;
import :process;

extern "C" char __bss_start[], __bss_end[];

namespace Kiss::Kernel {

struct Process;

void delay() {
    for (int i = 0; i < 30000000; i++)
        __asm__ __volatile__("nop"); // do nothing
}

Process* procA;
Process* procB;

void procAEntry() {
    yap("starting process A");
    while (true) {
        SBI::consolePutchar('A');
        yield();
        delay();
    }
}

void procBEntry() {
    yap("starting process B");
    while (true) {
        SBI::consolePutchar('B');
        yield();
        delay();
    }
}

void stop() {
    for (;;)
        Riscv32::wfi();
}

void entry() {
    yap("💋 Kiss Kernel v0.0.1");

    csrw(
        Riscv32::Csr::STVEC,
        reinterpret_cast<usize>(_kexception)
    );

    initProcess();

    procA = createProcess(reinterpret_cast<u32>(procAEntry));
    procB = createProcess(reinterpret_cast<u32>(procBEntry));
    yield();

    unreachable();
}

} // namespace Kiss::Kernel

void __panicHandler(PanicKind kind, char const* buf) {
    if (kind == PanicKind::PANIC) {
        Kiss::SBI::consolePuts("panic: "s);
        Kiss::SBI::consolePuts(buf);
        Kiss::SBI::consolePuts("\n");
        Kiss::Kernel::stop();
        __builtin_unreachable();
    } else {
        Kiss::SBI::consolePuts("debug: "s);
        Kiss::SBI::consolePuts(buf);
        Kiss::SBI::consolePuts("\n");
    }
}

extern "C" void _kissEntry() {
    std::memset(__bss_start, 0, reinterpret_cast<usize>(__bss_end) - reinterpret_cast<usize>(__bss_start));
    Karm::registerPanicHandler(__panicHandler);
    Kiss::Kernel::entry();
}
