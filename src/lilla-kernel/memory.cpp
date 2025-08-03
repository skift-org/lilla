module;

#include <karm-logger/logger.h>

export module Lilla.Kernel:memory;

import Karm.Core;

using namespace Karm;

[[gnu::aligned(4096)]] static u8 ram[64 * 1024 * 1024] = {}; // 64mib

namespace Lilla::Kernel {

constexpr u32 SATP_SV32 = 1u << 31;
constexpr usize PAGE_SIZE = 4096;

enum Page : u32 {
    V = 1 << 0,
    R = 1 << 1,
    W = 1 << 2,
    X = 1 << 3,
    U = 1 << 4,
};

static auto nextPAddr = reinterpret_cast<usize>(ram);

export usize allocPages(u32 n) {
    usize addr = nextPAddr;
    nextPAddr += n * PAGE_SIZE;

    if (nextPAddr > reinterpret_cast<usize>(ram + sizeof(ram)))
        logFatal("out of memory"s);

    std::memset(reinterpret_cast<void*>(addr), 0, n * PAGE_SIZE);
    return addr;
}

bool isAligned(u32 addr, u32 align) {
    return (addr % align) == 0;
}

export void mapPage(u32* table1, u32 vAddr, usize pAddr, u32 flags) {
    if (!isAligned(vAddr, PAGE_SIZE))
        logFatal("unaligned vaddr {:#x}"s, vAddr);

    if (!isAligned(pAddr, PAGE_SIZE))
        logFatal("unaligned paddr {:#x}"s, pAddr);

    u32 vpn1 = (vAddr >> 22) & 0x3ff;
    if ((table1[vpn1] & Page::V) == 0) {
        // Create the non-existent 2nd level page table.
        u32 ptPAddr = allocPages(1);
        table1[vpn1] = ((ptPAddr / PAGE_SIZE) << 10) | Page::V;
    }

    // Set the 2nd level page table entry to map the physical page.
    u32 vpn0 = (vAddr >> 12) & 0x3ff;
    auto table0 = reinterpret_cast<u32*>((table1[vpn1] >> 10) * PAGE_SIZE);
    table0[vpn0] = ((pAddr / PAGE_SIZE) << 10) | flags | Page::V;
}
} // namespace Lilla::Kernel
