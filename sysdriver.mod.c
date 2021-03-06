#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x9341a943, "module_layout" },
	{ 0xeaf8d0d7, "i2c_del_driver" },
	{ 0x2905eee4, "i2c_register_driver" },
	{ 0x1d7b4b8e, "kmalloc_caches" },
	{ 0xaeca4a52, "_dev_err" },
	{ 0x87a97a56, "sysfs_create_group" },
	{ 0xb0ffbbb, "kobject_create_and_add" },
	{ 0x6fb3fe8a, "device_property_read_u32_array" },
	{ 0xbef12856, "kmem_cache_alloc_trace" },
	{ 0x38f98b33, "i2c_transfer" },
	{ 0x9d669763, "memcpy" },
	{ 0x5f754e5a, "memset" },
	{ 0x91715312, "sprintf" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x7c32d0f0, "printk" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0xa46f2f1b, "kstrtouint" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xe31477c0, "kobject_put" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:24c32");

MODULE_INFO(srcversion, "A03814A21D674C1ED87E6CC");
