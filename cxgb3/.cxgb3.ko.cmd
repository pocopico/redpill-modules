cmd_/opt/downloads/redpill-modules/cxgb3/cxgb3.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /opt/downloads/redpill-modules/cxgb3/cxgb3.ko /opt/downloads/redpill-modules/cxgb3/cxgb3.o /opt/downloads/redpill-modules/cxgb3/cxgb3.mod.o
