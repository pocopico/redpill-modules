cmd_/opt/downloads/redpill-modules/dm9601/dm9601.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /opt/downloads/redpill-modules/dm9601/dm9601.ko /opt/downloads/redpill-modules/dm9601/dm9601.o /opt/downloads/redpill-modules/dm9601/dm9601.mod.o
