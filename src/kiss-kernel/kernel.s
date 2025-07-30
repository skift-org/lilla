__stack_top:
    .skip 0x20000
__stack_bottom:

.section .text.boot
.global _kissEntry
.type _kissEntry, @function

.global _kstart
_kstart:
    mv ra, zero
    mv fp, zero

    la sp, __stack_bottom
    jal _kissEntry
