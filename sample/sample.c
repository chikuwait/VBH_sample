#include <linux/module.h>
#include "hypervisor_introspection.h"
#include "vmx_common.h"

int handle_vmcall(hv_event_e type, unsigned char *data, int size, int *allow)
{
	if (type != vmcall) {
		pr_err("Invalid event(vmmcall) %d\n", type);
		return -EINVAL;
	}
	pr_info("Handled vmcall!\n");
	return 0;
}

static int __init vbh_sample_init(void)
{
	struct hvi_event_callback vmcall_event;
	int status;

	vmcall_event.event = vmcall;
	vmcall_event.callback = handle_vmcall;

	status = hvi_register_event_callback(&vmcall_event, 1);
	if (status){
		pr_err(" failed hvi_register_event_callback, status : %d\n",status);
	}

	asm_make_vmcall(1, NULL);
	return 0;
}

static void __exit vbh_sample_uninit(void)
{
	asm_make_vmcall(1, NULL);
	hvi_unregister_event_callback(vmcall);
}

module_init(vbh_sample_init);
module_exit(vbh_sample_uninit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yuki Nakata");
