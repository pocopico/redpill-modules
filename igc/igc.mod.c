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

MODULE_ALIAS("pci:v00008086d000015F2sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000015F3sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000015F8sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000015F7sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00003100sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00003101sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00003102sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00005502sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00005503sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00000D9Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000125Bsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000125Csv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000125Dsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000125Esv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000125Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000015FDsv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "39CDE37211DE39BC049CC71");
