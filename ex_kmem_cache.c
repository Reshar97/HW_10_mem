#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#define OBJ_SIZE 256 
#define NUM_OBJECTS 1024 

static struct kmem_cache *my_cache;

static void *ptrs[NUM_OBJECTS];

static int __init ex_kmem_cache_init(void) {
    int i;
    ktime_t start, end;
    s64 actual_ns;

    my_cache = kmem_cache_create("my_cache", OBJ_SIZE, 0, 0, NULL);
    if (!my_cache) {
        pr_err("kmem_cache_create: FAIL\n");
        return -ENOMEM;
    }

    pr_info("kmem_cache: start allocation of %u objects (%u bytes each)\n", NUM_OBJECTS, OBJ_SIZE);

    start = ktime_get();
    for (i = 0; i < NUM_OBJECTS; i++) {
        ptrs[i] = kmem_cache_alloc(my_cache, GFP_KERNEL);
        if (!ptrs[i]) {
            pr_err("kmem_cache_alloc: FAIL at index %d\n", i);
            while (--i >= 0) {
                kmem_cache_free(my_cache, ptrs[i]);
            }
            kmem_cache_destroy(my_cache);
            return -ENOMEM;
        }
        *(char *)ptrs[i] = 'A'; 
    }
    end = ktime_get();

    actual_ns = ktime_to_ns(ktime_sub(end, start));
    pr_info("kmem_cache: SUCCESS, time for %u objects: %lld us, type: physical slab objects\n", NUM_OBJECTS, actual_ns / 1000);

    for (i = 0; i < NUM_OBJECTS; i++) {
        kmem_cache_free(my_cache, ptrs[i]);
    }

    kmem_cache_destroy(my_cache);
    return 0;
}

static void __exit ex_kmem_cache_exit(void) {
    pr_info("kmem_cache: module unloaded\n");
}

module_init(ex_kmem_cache_init);
module_exit(ex_kmem_cache_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anton Zubin");
MODULE_DESCRIPTION("Demonstration of kmem_cache");

