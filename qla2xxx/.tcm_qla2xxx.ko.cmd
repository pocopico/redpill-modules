cmd_/opt/downloads/redpill-modules/qla2xxx/tcm_qla2xxx.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /opt/downloads/redpill-modules/qla2xxx/tcm_qla2xxx.ko /opt/downloads/redpill-modules/qla2xxx/tcm_qla2xxx.o /opt/downloads/redpill-modules/qla2xxx/tcm_qla2xxx.mod.o
