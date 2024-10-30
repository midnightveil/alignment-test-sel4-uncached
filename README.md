# Alignment Fault w/seL4 Microkit on Uncached Memory

Build:
```
$ make MICROKIT_SDK=/path/to/sdk
```

Built with the `arm-gnu-toolchain-12.2.rel1-x86_64-aarch64-none-elf` listed on the Microkit README.

Running on the iMX8MQ_EVK board, the following behaviour:

```
Booting all finished, dropped to user space
MON|INFO: Microkit Bootstrap
MON|INFO: bootinfo untyped list matches expected list
MON|INFO: Number of bootstrap invocations: 0x00000009
MON|INFO: Number of system invocations:    0x00000028
MON|INFO: completed bootstrap invocations
MON|INFO: completed system invocations
writing to cached
writing to uncached
MON|ERROR: received message 0x00000006  badge: 0x0000000000000001  tcb cap: 0x8000000000000008
MON|ERROR: faulting PD: hello
Registers:
pc : 0x0000000000200074
sp: 0x000000ffffffffa0
spsr : 0x0000000060000040
x0 : 0x0000000000200408                   // All zeroed registers have been elided for readability
x1 : 0x0000000030010000
x2 : 0x00000000000a2121
x4 : 0x6f77206f6c6c6568
x5 : 0x0a21212121646c72
x7 : 0xfffffffffffffff4
x8 : 0x0000000000200404
x29 : 0x000000ffffffffa0
x30 : 0x0000000000200058
x19 : 0x0000000000201000
x20 : 0x0000000000201010
tpidr_el0 : 0x0000000000000000
tpidrro_el0 : 0x0000000000000000
MON|ERROR: VMFault: ip=0x0000000000200074  fault_addr=0x000000003001000d  fsr=0x0000000093c28061  (data fault)
MON|ERROR:   ec: 0x00000024  Data Abort from a lower Exception level   il: 1   iss: 0x01c28061
MON|ERROR:   dfsc = alignment fault (0x00000021) -- write not read
<<seL4(CPU 0) [receiveIPC/142 T0x80fffe7400 "rootserver" @8a0007d8]: Reply object already has unexecuted reply!>>
```

The disassembly of the init() function:

```asm
0000000000200010 <init>:
; init():
; /alignment-test/hello.c:16
  200010: a9be7bfd      stp     x29, x30, [sp, #-0x20]!
; /alignment-test/hello.c:17
  200014: 90000000      adrp    x0, 0x200000 <_text>
  200018: 910f6000      add     x0, x0, #0x3d8
; /alignment-test/hello.c:16
  20001c: 910003fd      mov     x29, sp
  200020: a90153f3      stp     x19, x20, [sp, #0x10]
; /alignment-test/hello.c:17
  200024: 9400002b      bl      0x2000d0 <microkit_dbg_puts>
; /alignment-test/hello.c:18
  200028: b0000001      adrp    x1, 0x201000 <data_string>
  20002c: 91004034      add     x20, x1, #0x10
  200030: b0000013      adrp    x19, 0x201000 <data_string>
  200034: f9400262      ldr     x2, [x19]
; /alignment-test/hello.c:19
  200038: 90000000      adrp    x0, 0x200000 <_text>
  20003c: 910fc000      add     x0, x0, #0x3f0
; /alignment-test/hello.c:18
  200040: f9400821      ldr     x1, [x1, #0x10]
  200044: a9401444      ldp     x4, x5, [x2]
  200048: a9001424      stp     x4, x5, [x1]
  20004c: f840d042      ldur    x2, [x2, #0xd]
  200050: f800d022      stur    x2, [x1, #0xd]
; /alignment-test/hello.c:19
  200054: 9400001f      bl      0x2000d0 <microkit_dbg_puts>
; /alignment-test/hello.c:20
  200058: f9400262      ldr     x2, [x19]
; /alignment-test/hello.c:22
  20005c: 90000000      adrp    x0, 0x200000 <_text>
  200060: 91102000      add     x0, x0, #0x408
; /alignment-test/hello.c:20
  200064: f9400681      ldr     x1, [x20, #0x8]
  200068: a9401444      ldp     x4, x5, [x2]
  20006c: a9001424      stp     x4, x5, [x1]
  200070: f840d042      ldur    x2, [x2, #0xd]
  200074: f800d022      stur    x2, [x1, #0xd]
; /alignment-test/hello.c:23
  200078: a94153f3      ldp     x19, x20, [sp, #0x10]
  20007c: a8c27bfd      ldp     x29, x30, [sp], #0x20
; /alignment-test/hello.c:22
  200080: 14000014      b       0x2000d0 <microkit_dbg_puts>
  200084: d503201f      nop
  200088: d503201f      nop
  20008c: d503201f      nop
```
