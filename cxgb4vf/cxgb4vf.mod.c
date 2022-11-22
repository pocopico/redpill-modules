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

MODULE_ALIAS("pci:v00001425d00004800sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004801sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004802sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004803sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004804sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004805sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004806sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004807sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004808sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004809sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000480Asv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000480Bsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000480Csv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000480Dsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000480Esv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004880sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004881sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004882sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004883sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004884sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004885sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004886sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004887sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00004888sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005800sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005801sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005802sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005803sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005804sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005805sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005806sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005807sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005808sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005809sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000580Asv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000580Bsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000580Csv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000580Dsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000580Esv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005810sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005811sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005812sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005813sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005814sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005815sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005816sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005817sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005880sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005881sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005882sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005883sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005884sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005885sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005886sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005887sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005888sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005889sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005890sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005891sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005892sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005893sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005894sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005895sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005896sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00005897sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006801sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006802sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006803sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006804sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006805sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006806sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006807sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006809sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d0000680Dsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006810sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006811sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006814sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001425d00006815sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "218F942D86A16624003F0EF");
