#ifndef __COMMANDS_H_INCLUDED__
#define __COMMANDS_H_INCLUDED__

/* DEFCMD(NAME, CODE, NARGS, CODE) */

DEFCMD(HLT, 0x00, 0,
        {
            return CPU_SUCCESS;
        }
      )

DEFCMD(PUSH, 0x01, 1,
        {
            cpuNumber_t num = NAN;

            if (0 != cpu->code[cpu->pc].opcode.mem)
                {
                    long long addr = 0;

                    if (0 != cpu->code[cpu->pc].opcode.imm)
                    {
                        addr += cpu->code[cpu->pc].data.address;
                    }
                    if (0 != cpu->code[cpu->pc].opcode.reg)
                    {
                        addr += (long long) cpu->reg[cpu->code[cpu->pc].opcode.regNo];
                    }

                    num = cpu->RAM[addr];
                    sleep(1);
                }

                else
                {
                    if (0 != cpu->code[cpu->pc].opcode.imm)
                    {
                        num = cpu->code[cpu->pc].data.number;
                    }
                    else if (0 != cpu->code[cpu->pc].opcode.reg)
                    {
                        num = cpu->reg[cpu->code[cpu->pc].opcode.regNo];
                    }
                }

                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, cpu->code[cpu->pc].data.number), CPU_STACKERR);
                break;
            }
        }
      )

#endif /* __COMMANDS_H_INCLUDED__ */
