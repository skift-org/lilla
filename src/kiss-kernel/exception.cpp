export module Kiss.Kernel:exception;

import :panic;
import :riscv32;
import Kiss.Base;

namespace Kiss::Kernel {

struct [[gnu::packed]] TrapFrame {
    u32 ra;
    u32 gp;
    u32 tp;
    u32 t0;
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    u32 t5;
    u32 t6;
    u32 a0;
    u32 a1;
    u32 a2;
    u32 a3;
    u32 a4;
    u32 a5;
    u32 a6;
    u32 a7;
    u32 s0;
    u32 s1;
    u32 s2;
    u32 s3;
    u32 s4;
    u32 s5;
    u32 s6;
    u32 s7;
    u32 s8;
    u32 s9;
    u32 s10;
    u32 s11;
    u32 sp;
};

export extern "C" void _kexception();

export extern "C" void _kissHandleTrap(TrapFrame* f) {
    u32 scause = csrr(Asm::Csr::SCAUSE);
    u32 stval = csrr(Asm::Csr::STVAL);
    u32 userPc = csrr(Asm::Csr::SEPC);

    panic("unexpected trap scause=0x%h, stval=0x%h, sepc=0x%h\n"s, scause, stval, userPc, reinterpret_cast<paddr>(f));
}

} // namespace Kiss::Kernel
