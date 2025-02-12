export module Kiss.Kernel:memory;

import Kiss.Base;
import :panic;

extern "C" char __free_ram[], __free_ram_end[];

namespace Kiss::Kernel {

constexpr usize PAGE_SIZE = 4096;
static auto next_paddr = reinterpret_cast<paddr>(__free_ram);
export paddr allocPages(u32 n) {

    paddr addr = next_paddr;
    next_paddr += n * PAGE_SIZE;

    if (next_paddr > reinterpret_cast<paddr>(__free_ram_end))
        panic("out of memory"s);

    memset(reinterpret_cast<void*>(addr), 0, n * PAGE_SIZE);
    return addr;
}
}