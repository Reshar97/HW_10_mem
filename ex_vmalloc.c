#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/ktime.h>

#define ALLOC_SIZE (2048 * 2048)

static int __init ex_vmalloc_init(void) {
    void *ptr;
    ktime_t start, end;
    s64 actual_ns;

    pr_info("vmalloc: start allocation of %d bytes\n", ALLOC_SIZE);

    start = ktime_get();
    ptr = vmalloc(ALLOC_SIZE);
    end = ktime_get();

    actual_ns = ktime_to_ns(ktime_sub(end, start));

    if (!ptr) {
        pr_err("vmalloc: FAIL (could not allocate %d bytes)\n", ALLOC_SIZE);
        return -ENOMEM;
    }

    pr_info("vmalloc: SUCCESS, time: %lld us, addr: %p\n", actual_ns / 1000, ptr);

    vfree(ptr);
    return 0;
}

static void __exit ex_vmalloc_exit(void) {
    pr_info("vmalloc: module unloaded\n");
}

module_init(ex_vmalloc_init);
module_exit(ex_vmalloc_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anton Zubin");
MODULE_DESCRIPTION("Example of vmalloc");
