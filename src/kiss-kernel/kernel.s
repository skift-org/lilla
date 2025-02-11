.global __stack_top

.section .text.boot
.global _kissEntry
.type _kissEntry, @function

.global _kstart
_kstart:
    mv ra, zero
    mv fp, zero

    la sp, __stack_top
    jal _kissEntry
