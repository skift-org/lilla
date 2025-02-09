#include <stdint.h>
#include <stddef.h>
typedef uint8_t u8;
typedef uint32_t u32;
typedef size_t usize;

extern char __bss[], __bss_end[], __stack_top[];

void* memset(void* buf, char c, usize n) {
    u8* p = static_cast<u8*>(buf);
    while (n--)
        *p++ = c;
    return buf;
}

extern "C" void kernel_main(void) {
    memset(__bss, 0, (usize) __bss_end - (usize) __bss);

    for (;;);
}

//TODO: Virer cette merde
__attribute__((section(".text.boot")))
__attribute__((naked))
extern "C" void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" // Set the stack pointer
        "j kernel_main\n"       // Jump to the kernel main function
        :
        : [stack_top] "r" (__stack_top) // Pass the stack top address as %[stack_top]
    );
}