cmd_/opt/downloads/redpill-modules/bnxt/bnxt_en.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /opt/downloads/redpill-modules/bnxt/bnxt_en.ko /opt/downloads/redpill-modules/bnxt/bnxt_en.o /opt/downloads/redpill-modules/bnxt/bnxt_en.mod.o
