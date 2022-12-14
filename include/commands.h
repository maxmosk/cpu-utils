/* DEFCMD(NAME, N, NARGS, CODE) */

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
                cpuAddress_t addr = 0;

                if (0 != cpu->code[cpu->pc].opcode.imm)
                {
                    addr += cpu->code[cpu->pc].data.address;
                }
                if (0 != cpu->code[cpu->pc].opcode.reg)
                {
                    addr += (cpuAddress_t) cpu->reg[cpu->code[cpu->pc].opcode.regNo];
                }

                num = cpu->RAM[addr];
                usleep(100);
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

            CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, num), CPU_STACKERR);
        }
      )


DEFCMD(IN, 0x02, 0,
        {
            cpuNumber_t num = NAN;
            CPU_CHECK(1 == scanf("%lf", &num), CPU_ERROR);
            CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, num), CPU_STACKERR);
        }
      )


DEFCMD(OUT, 0x03, 0,
        {
            cpuNumber_t num = NAN;
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &num), CPU_STACKERR);
            CPU_CHECK(0 != printf("%lg\n", num), CPU_ERROR);
        }
      )


DEFCMD(ADD, 0x04, 0,
        {       
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, a + b), CPU_STACKERR);
        }
      )


DEFCMD(SUB, 0x05, 0,
        {
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, b - a), CPU_STACKERR);
        }
      )


DEFCMD(MUL, 0x06, 0,
        {
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, b * a), CPU_STACKERR);
        }
      )


DEFCMD(DIV, 0x07, 0,
        {
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            CPU_CHECK(fabs(a) >= ACCURACY, CPU_ZERODIV);
            CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, b / a), CPU_STACKERR);
        }
      )


DEFCMD(DUP, 0x08, 0,
        {
            cpuNumber_t num = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &num), CPU_STACKERR);
            for (int i = 0; i < 2; i++)
            {
                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, num), CPU_STACKERR);
            }
        }
      )


DEFJMP(JMP, 0x09, 1,
        {
            cpu->pc = cpu->code[cpu->pc].data.address;
            cpu->pc--;
        })


DEFCMD(POP, 0x0A, 1,
        {
            cpuNumber_t *dst = NULL;
            
            if (0 != cpu->code[cpu->pc].opcode.mem)
            {
                cpuAddress_t addr = 0;

                if (0 != cpu->code[cpu->pc].opcode.imm)
                {
                    addr += cpu->code[cpu->pc].data.address;
                }
                if (0 != cpu->code[cpu->pc].opcode.reg)
                {
                    addr += (cpuAddress_t) cpu->reg[cpu->code[cpu->pc].opcode.regNo];
                }

                dst = &cpu->RAM[addr];
                usleep(100);
            }

            else if (0 != cpu->code[cpu->pc].opcode.reg)
            {
                dst = &cpu->reg[cpu->code[cpu->pc].opcode.regNo];
            }
             
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, dst), CPU_STACKERR);
        }
      )


DEFJMP(JE, 0x0B, 1,
        {
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            if (fabs(b - a) < ACCURACY)
            {
                cpu->pc = cpu->code[cpu->pc].data.address;
                cpu->pc--;
            }
        })


DEFJMP(JNE, 0x0C, 1,
        {
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            if (fabs(b - a) >= ACCURACY)
            {
                cpu->pc = cpu->code[cpu->pc].data.address;
                cpu->pc--;
            }
        })


DEFJMP(JA, 0x0D, 1,
        {
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            if ((fabs(b - a) >= ACCURACY) && (b > a))
            {
                cpu->pc = cpu->code[cpu->pc].data.address;
                cpu->pc--;
            }
        })


DEFJMP(JAE, 0x0E, 1,
        {
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            if ((fabs(b - a) < ACCURACY) || (b > a))
            {
                cpu->pc = cpu->code[cpu->pc].data.address;
                cpu->pc--;
            }
        })


DEFJMP(JB, 0x0F, 1,
        {
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            if ((fabs(b - a) >= ACCURACY) && (b < a))
            {
                cpu->pc = cpu->code[cpu->pc].data.address;
                cpu->pc--;
            }
        })


DEFJMP(JBE, 0x10, 1,
        {
            cpuNumber_t a = NAN;
            cpuNumber_t b = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_STACKERR);
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_STACKERR);
            if (fabs(b - a) < ACCURACY || (b < a))
            {
                cpu->pc = cpu->code[cpu->pc].data.address;
                cpu->pc--;
            }
        })


DEFJMP(CALL, 0x11, 1,
        {
            cpuData_t retAddr = {.address = cpu->pc};
            CPU_CHECK(STACK_ERROR != stackPush(&cpu->callstack, retAddr.number), CPU_STACKERR);
            cpu->pc = cpu->code[cpu->pc].data.address;
            cpu->pc--;
        })


DEFCMD(RET, 0x12, 0,
        {
            cpuData_t retAddr = {.address = SIZE_MAX};
            CPU_CHECK(STACK_ERROR != stackPop(&cpu->callstack, &retAddr.number), CPU_STACKERR);
            cpu->pc = retAddr.address;
        })


DEFCMD(MEOW, 0x13, 0,
        {
            printf("MEOW!\n");
        }
      )

DEFCMD(SQRT, 0x14, 0,
        {
            cpuNumber_t num = NAN;

            CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &num), CPU_STACKERR);

            num = sqrt(num);
            CPU_CHECK(isfinite(num), CPU_FLOATERR);
            CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, num), CPU_STACKERR);
        }
      )


#define SCREEN_X 80
#define SCREEN_Y 24
DEFCMD(PRINT, 0x15, 0,
        {
            static char vbuf[SCREEN_Y * (SCREEN_X + 1)];

            for (int y = 0; y < SCREEN_Y; y++)
            {
                for (int x = 0; x < SCREEN_X; x++)
                {
                    vbuf[y * (SCREEN_X + 1) + x] = (char) cpu->RAM[y * SCREEN_X + x];
                }
                vbuf[(y + 1) * (SCREEN_X + 1) - 1] = '\n';
            }

            fprintf(stdout, "\033[2J\n");
            fwrite(vbuf, sizeof *vbuf, sizeof vbuf / sizeof *vbuf, stdout);
        }
      )
#undef SCREEN_X
#undef SCREEN_Y


DEFCMD(DUMP, 0x1F, 0,
        {
            cpuDump(cpu);
        })

