// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: save_registers.cpp
// Description: pushAllRegisters Implementations
//      Author: Ziming Li
//     Created: 2018-08-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#if OS_WIN
#   if ARCH_CPU_X86
extern "C" __declspec(naked) void pushAllRegisters(void *, void *, void *)
{
    __asm {
        push ebx
        push ebp
        push esi
        push edi
        mov ecx, [esp + 28]
        push esp
        push [esp + 28]
        push [esp + 28]
        call ecx
        add esp, 28
        ret
    }
}
#   endif // ARCH_CPU_X86
#endif // OS_WIN
