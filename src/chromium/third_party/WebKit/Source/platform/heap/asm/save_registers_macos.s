// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: SaveRegisters_macos.s
// Description: pushAllRegisters Implementation
//      Author: Ziming Li
//     Created: 2019-08-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

.global _pushAllRegisters

_pushAllRegisters:
    pushq $0
    push %rbx
    push %rbp
    push %r12
    push %r13
    push %r14
    push %r15
    mov %rdx, %r8
    mov %rsp, %rdx
    call *%r8
    add $56, %rsp
    ret
