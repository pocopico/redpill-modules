cmd_/opt/downloads/redpill-modules/mpt3sas/mpt3sas.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /opt/downloads/redpill-modules/mpt3sas/mpt3sas.ko /opt/downloads/redpill-modules/mpt3sas/mpt3sas.o /opt/downloads/redpill-modules/mpt3sas/mpt3sas.mod.o
