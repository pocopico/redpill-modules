cmd_/opt/downloads/redpill-modules/cxgb/cxgb.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /opt/downloads/redpill-modules/cxgb/cxgb.ko /opt/downloads/redpill-modules/cxgb/cxgb.o /opt/downloads/redpill-modules/cxgb/cxgb.mod.o
