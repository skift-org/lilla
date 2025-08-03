module;

#include <karm-logger/logger.h>

export module Lilla.Kernel:main;

import Karm.Core;
import Lilla.SBI;
import :exception;
import :memory;
import :process;

extern "C" char __bss_start[], __bss_end[];

namespace Lilla::Kernel {

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
    yap("🪻  Lilla Kernel v0.0.1");

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

} // namespace Lilla::Kernel

void __panicHandler(PanicKind kind, char const* buf) {
    if (kind == PanicKind::PANIC) {
        Lilla::SBI::consolePuts("panic: "s);
        Lilla::SBI::consolePuts(buf);
        Lilla::SBI::consolePuts("\n");
        Lilla::Kernel::stop();
        __builtin_unreachable();
    } else {
        Lilla::SBI::consolePuts("debug: "s);
        Lilla::SBI::consolePuts(buf);
        Lilla::SBI::consolePuts("\n");
    }
}

extern "C" void _lillaEntry() {
    std::memset(__bss_start, 0, reinterpret_cast<usize>(__bss_end) - reinterpret_cast<usize>(__bss_start));
    Karm::registerPanicHandler(__panicHandler);
    Lilla::Kernel::entry();
}
