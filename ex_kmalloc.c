#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#define ALLOC_SIZE (1024 * 1024) // 1 MB

static int __init kmalloc_init(void) {
    void *ptr;
    ktime_t start, end;
    s64 actual_ns;

    pr_info("kmalloc: start allocation of %d bytes\n", ALLOC_SIZE);

    start = ktime_get();
    ptr = kmalloc(ALLOC_SIZE, GFP_KERNEL);
    end = ktime_get();

    if (!ptr) {
        pr_err("kmalloc: FAIL (could not allocate %d bytes)\n", ALLOC_SIZE);
        return -ENOMEM;
    }

    actual_ns = ktime_to_ns(ktime_sub(end, start));
    pr_info("kmalloc: SUCCESS, time: %lld us, addr: %p, type: physical\n", actual_ns / 1000, ptr);

    kfree(ptr);
    return 0;
}

static void __exit kmalloc_exit(void) {
    pr_info("kmalloc: module unloaded\n");
}

module_init(kmalloc_init);
module_exit(kmalloc_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anton Zubin");
MODULE_DESCRIPTION("Demonstration of kmalloc");
