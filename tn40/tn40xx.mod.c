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

MODULE_ALIAS("pci:v00001FC9d00004010sv00001FC9sd00004010bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004020sv00001FC9sd00003015bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004022sv00001FC9sd00003015bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004022sv00001186sd00004D00bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004022sv00001043sd00008709bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004022sv00001432sd00008103bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004024sv00001FC9sd00003015bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004027sv00001FC9sd00003015bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004027sv00001432sd00008104bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004527sv00001FC9sd00003015bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004026sv00001FC9sd00003015bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004025sv00001186sd00002900bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004025sv00001FC9sd00003015bc*sc*i*");
MODULE_ALIAS("pci:v00001FC9d00004025sv00001432sd00008102bc*sc*i*");

MODULE_INFO(srcversion, "01F5599A0B9EB2A857E00C5");
