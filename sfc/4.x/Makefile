sfc-y			+= efx.o nic.o farch.o falcon.o siena.o ef10.o tx.o \
			   rx.o selftest.o ethtool.o qt202x_phy.o mdio_10g.o \
			   tenxpress.o txc43128_phy.o falcon_boards.o \
			   mcdi.o mcdi_port.o mcdi_mon.o ptp.o
sfc-$(CONFIG_SFC_MTD)	+= mtd.o
sfc-$(CONFIG_SFC_SRIOV)	+= sriov.o siena_sriov.o ef10_sriov.o

obj-m	+= sfc.o
