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

MODULE_ALIAS("pci:v00001C00d00002273sv00001C00sd00002273bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00003253sv00004348sd00003253bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00005053sv00004348sd00005053bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00003453sv00004348sd00003453bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00007053sv00004348sd00003253bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00005046sv00004348sd00005046bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00007173sv00004348sd00003473bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00007073sv00004348sd00003473bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00003873sv00004348sd00003873bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00003853sv00004348sd00003853bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00005334sv00004348sd00005053bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00005334sv00004348sd00005334bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00005338sv00004348sd00005338bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00005838sv00004348sd00005838bc*sc*i*");
MODULE_ALIAS("pci:v00001C00d00003253sv00001C00sd00003253bc*sc*i*");
MODULE_ALIAS("pci:v00001C00d00003250sv00001C00sd00003250bc*sc*i*");
MODULE_ALIAS("pci:v00001C00d00003470sv00001C00sd00003470bc*sc*i*");
MODULE_ALIAS("pci:v00001C00d00003450sv00001C00sd00003450bc*sc*i*");
MODULE_ALIAS("pci:v00001C00d00003853sv00001C00sd00003853bc*sc*i*");
MODULE_ALIAS("pci:v00001C00d00004353sv00001C00sd00004353bc*sc*i*");
MODULE_ALIAS("pci:v00004348d00005049sv00004348sd00005049bc*sc*i*");
