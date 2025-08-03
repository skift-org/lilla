export module Lilla.Kernel:riscv32;

import Karm.Core;

using namespace Karm;

namespace Lilla::Kernel::Riscv32 {

export enum struct Csr : usize {
#define CSR(NUM, _, NAME) NAME = NUM,
#include "defs/csr.inc"

#undef CSR
};

export usize csrr(Csr csr) {
    usize tmp;
    switch (csr) {
#define CSR(NUM, name, NAME)                                 \
    case Csr::NAME:                                          \
        __asm__ __volatile__("csrr %0, " #name : "=r"(tmp)); \
        break;
#include "defs/csr.inc"

#undef CSR
    };
    return tmp;
}

export void csrw(Csr csr, usize val) {
    switch (csr) {
#define CSR(NUM, name, NAME)                                   \
    case Csr::NAME:                                            \
        __asm__ __volatile__("csrw " #name ", %0" ::"r"(val)); \
        break;
#include "defs/csr.inc"
#undef CSR
    };
}

export void unimp() { __asm__ __volatile__("unimp"); }

export void wfi() { __asm__ __volatile__("wfi"); }

export void di() { __asm__ __volatile__("csrci mstatus, 8"); }

export void ei() { __asm__ __volatile__("csrsi mstatus, 8"); }

export void sfenceVma() { __asm__ __volatile__("sfence.vma"); }

} // namespace Lilla::Kernel::Asm
