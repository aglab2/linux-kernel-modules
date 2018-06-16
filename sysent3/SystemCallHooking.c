#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/unistd.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>

#include "SystemCallHooking.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Denis Kopyrin");
MODULE_DESCRIPTION("Hello World Module");


/* sys_call_table address */
unsigned long *sys_call_table = SYS_CALL_TABLE_ADDR;

/* real_access address */
asmlinkage long (*real_access)(const char __user *filename, int mode);

/* hook sys_access which is */
asmlinkage long custom_access(const char __user *filename, int mode)
{
	printk("%s is launched\n", current->comm);
	printk("path: %s, mode: %d\n", filename, mode);
	return real_access(filename, mode);
}

int make_rw(unsigned long address) 
{
	unsigned int level;
	pte_t *pte = lookup_address(address, &level);
	if (!pte)
	{
		printk("no pte\n");
		return -1;
	}

	if(pte->pte &~ _PAGE_RW)
		pte->pte |= _PAGE_RW;
	return 0;
}


int make_ro(unsigned long address)
{
	unsigned int level;
	pte_t *pte = lookup_address(address, &level);
	if (!pte)
	{
		printk("no pte\n");
		return -1;
	}
	pte->pte = pte->pte & ~_PAGE_RW;
	return 0;
}


static int __init test_init(void) 
{
	printk("addr 0x%lx\n", (unsigned long) sys_call_table);

	/* hook access system call*/
	int ret = make_rw((unsigned long)sys_call_table);
	if (ret)
		return ret;

	real_access = (long (*)(const char __user *filename, int mode))
	*(sys_call_table + __NR_access);
	*(sys_call_table + __NR_access) = (unsigned long)custom_access;
	make_ro((unsigned long)sys_call_table);

	return 0;
}

static void __exit test_exit(void)
{
	/* resume what it should be */
	make_rw((unsigned long)sys_call_table);
	*(sys_call_table + __NR_access) = (unsigned long)real_access;
	make_ro((unsigned long)sys_call_table);
}


module_init(test_init);
module_exit(test_exit);
