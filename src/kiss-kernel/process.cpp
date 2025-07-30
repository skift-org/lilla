module;

#include <karm-logger/logger.h>

export module Kiss.Kernel:process;

import Kiss.SBI;
import :riscv32;
import :memory;

namespace Kiss::Kernel {

extern "C" char __kernel_start[], __kernel_end[];

constexpr u32 PROCS_MAX = 8;

extern "C" void _switchContext(u32* prev_sp, u32* next_sp);

export enum ProcessState : u32 {
    PROC_UNUSED = 0,
    PROC_RUNNABLE = 1
};

export struct Process {

    i32 pid;
    ProcessState state;
    usize sp;
    u32* pageTable;
    u32 stack[8192];
};

static Process procs[PROCS_MAX]; // All process control structures.

Process* createProcess(u32 pc) {
    // Find an unused process control structure.
    Process* proc = nullptr;
    u32 i;
    for (i = 0; i < PROCS_MAX; i++) {
        if (procs[i].state == PROC_UNUSED) {
            proc = &procs[i];
            break;
        }
    }

    if (!proc)
        panic("no free process slots");

    // Stack callee-saved registers. These register values will be restored in
    // the first context switch in switch_context.
    u32* sp = &proc->stack[sizeof(proc->stack)];
    *--sp = 0;  // s11
    *--sp = 0;  // s10
    *--sp = 0;  // s9
    *--sp = 0;  // s8
    *--sp = 0;  // s7
    *--sp = 0;  // s6
    *--sp = 0;  // s5
    *--sp = 0;  // s4
    *--sp = 0;  // s3
    *--sp = 0;  // s2
    *--sp = 0;  // s1
    *--sp = 0;  // s0
    *--sp = pc; // ra

    auto pageTable = reinterpret_cast<u32*>(allocPages(1));
    yap("Alloc page {:#x} for process {}\n", reinterpret_cast<u32>(pageTable), i + 1);
    for (usize pAddr = reinterpret_cast<usize>(__kernel_start);
         pAddr < reinterpret_cast<usize>(__kernel_end); pAddr += PAGE_SIZE)
        mapPage(pageTable, pAddr, pAddr, Page::R | Page::W | Page::X);

    // Initialize fields.
    proc->pid = i + 1;
    proc->state = PROC_RUNNABLE;
    proc->sp = reinterpret_cast<usize>(sp);
    proc->pageTable = pageTable;
    return proc;
}

static Process* currentProc = nullptr;
static Process* idleProc = nullptr;

export void yield() {
    // Search for a runnable process
    Process* next = idleProc;
    for (u32 i = 0; i < PROCS_MAX; i++) {
        Process* proc = &procs[(currentProc->pid + i) % PROCS_MAX];
        if (proc->state == PROC_RUNNABLE && proc->pid > 0) {
            next = proc;
            break;
        }
    }

    // If there's no runnable process other than the current one, return and continue processing
    if (next == currentProc)
        return;
    Riscv32::sfenceVma();
    csrw(Riscv32::Csr::SATP, SATP_SV32 | reinterpret_cast<u32>(next->pageTable) / PAGE_SIZE);
    Riscv32::sfenceVma();
    csrw(Riscv32::Csr::SSCRATCH, reinterpret_cast<u32>(&next->stack[sizeof(next->stack)]));

    // Context switch
    Process* prev = currentProc;
    currentProc = next;
    _switchContext(&prev->sp, &next->sp);
}

export void initProcess() {
    idleProc = createProcess(reinterpret_cast<u32>(nullptr));
    idleProc->pid = -1; // idle
    currentProc = idleProc;
}

} // namespace Kiss::Kernel