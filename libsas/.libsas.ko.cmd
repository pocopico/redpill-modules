cmd_/opt/downloads/redpill-modules/libsas/libsas.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /opt/downloads/redpill-modules/libsas/libsas.ko /opt/downloads/redpill-modules/libsas/libsas.o /opt/downloads/redpill-modules/libsas/libsas.mod.o
