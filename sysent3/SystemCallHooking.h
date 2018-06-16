#ifndef HOOK_FUNCTION_PTR
#define HOOK_FUNCTION_PTR

/* copy from fs/exec.c */
struct user_arg_ptr {
	union {
		const char __user *const __user *native;
	} ptr;
};


#define SYS_CALL_TABLE_ADDR \
(unsigned long*)0xffffffffa18001a0

#endif
