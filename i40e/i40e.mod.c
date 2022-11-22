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
"depends=vxlan";

MODULE_ALIAS("pci:v00008086d00001572sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001574sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001580sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001581sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001583sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001584sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001585sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000101Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001586sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001589sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000015FFsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000104Esv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000104Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001587sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001588sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00000CF8sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00000D58sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000037CEsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000037CFsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000037D0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000037D1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000037D2sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000037D3sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000158Asv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000158Bsv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "F5219D0679A0A07A0DB2E6A");
