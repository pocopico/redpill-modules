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
"depends=";

MODULE_ALIAS("pci:v00001077d00002100sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002200sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002300sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002312sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002322sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00006312sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00006322sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002422sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002432sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00008432sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00005422sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00005432sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002532sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002031sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00008001sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00008021sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00008031sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d0000F001sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00008044sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002071sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002271sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001077d00002261sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "80C44015B9E5FC0369F61BF");
