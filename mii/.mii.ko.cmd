cmd_/opt/downloads/redpill-modules/mii/mii.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /opt/downloads/redpill-modules/mii/mii.ko /opt/downloads/redpill-modules/mii/mii.o /opt/downloads/redpill-modules/mii/mii.mod.o
