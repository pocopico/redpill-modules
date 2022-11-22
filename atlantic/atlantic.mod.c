#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=crc-itu-t";

MODULE_ALIAS("pci:v00001D6Ad00000001sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad0000D100sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad0000D107sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad0000D108sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad0000D109sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000000B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000007B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000008B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000009B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000011B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000012B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000080B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000087B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000088B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000089B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000091B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000092B1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000004C0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000000C0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000014C0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000034C0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000012C0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000011C0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000094C0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D6Ad000093C0sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "AA3F2DA7FC077CA3F003A5E");
