cmd_/opt/downloads/redpill-modules/mptsas/mptbase.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /opt/downloads/redpill-modules/mptsas/mptbase.ko /opt/downloads/redpill-modules/mptsas/mptbase.o /opt/downloads/redpill-modules/mptsas/mptbase.mod.o
