export module Kiss.Kernel:process;

import Kiss.Base;
import :panic;

namespace Kiss::Kernel {

constexpr u32 PROCS_MAX = 8;

extern "C" void _switchContext(u32* prev_sp, u32* next_sp);

export enum ProcessState : u32 {
    PROC_UNUSED = 0,
    PROC_RUNNABLE = 1
};

export struct Process {

    i32 pid;
    ProcessState state;
    vaddr sp;
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
        panic("no free process slots"s);

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

    // Initialize fields.
    proc->pid = i + 1;
    proc->state = PROC_RUNNABLE;
    proc->sp = reinterpret_cast<vaddr>(sp);
    return proc;
}

static Process* currentProc;
static Process* idleProc;

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

    __asm__ __volatile__(
        "csrw sscratch, %[sscratch]\n"
        :
        : [sscratch] "r" (reinterpret_cast<u32>(&next->stack[sizeof(next->stack)]))
    );


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