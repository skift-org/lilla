.global __stack_top

.section .text.start
.global _iniMain
.type _iniMain, @function
.global _start
_start:
    la sp, __stack_top
    call _iniMain
    call exit
