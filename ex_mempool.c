#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mempool.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#define POOL_SIZE 128 
#define ALLOC_COUNT 512 
#define ELEMENT_SIZE 256 // Размер одного элемента пула

static mempool_t *my_mempool;

static void *ptrs[ALLOC_COUNT];

static int __init ex_mempool_init(void) {
     int i;
     ktime_t start, end;
     s64 actual_ns;

     my_mempool = mempool_create(POOL_SIZE, mempool_kmalloc, mempool_kfree, (void *)(uintptr_t)ELEMENT_SIZE);
     if (!my_mempool) {
         pr_err("mempool_create: FAIL\n");
         return -ENOMEM;
     }

     pr_info("mempool: start allocation of %u elements\n", ALLOC_COUNT);

     start = ktime_get();
     for (i = 0; i < ALLOC_COUNT; i++) {
         ptrs[i] = mempool_alloc(my_mempool, GFP_KERNEL);
         if (!ptrs[i]) {
             pr_err("mempool_alloc: FAIL at index %d\n", i);
             while (--i >= 0) {
                 mempool_free(ptrs[i], my_mempool);
             }
             mempool_destroy(my_mempool);
             return -ENOMEM;
         }
         *(char *)ptrs[i] = 'B';
     }
     end = ktime_get();

     actual_ns = ktime_to_ns(ktime_sub(end, start));
     pr_info("mempool: SUCCESS, time for %u elements: %lld us, type: preallocated pool & physical\n", ALLOC_COUNT, actual_ns / 1000);

     for (i = 0; i < ALLOC_COUNT; i++) {
         mempool_free(ptrs[i], my_mempool);
     }

     mempool_destroy(my_mempool);
     
     return 0;
}

static void __exit ex_mempool_exit(void) {
     pr_info("mempool: module unloaded\n");
}

module_init(ex_mempool_init);
module_exit(ex_mempool_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anton Zubin");
MODULE_DESCRIPTION("Demonstration of mempool");

