#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/mm.h> 
#include <linux/ktime.h>

#define PAGE_ORDER 4 // Выделяем 2^4 страниц. 16 * 4096 байт = 64 КБ

static int __init ex_get_page_init(void) {
     unsigned long addr;
     ktime_t start, end;
     s64 actual_ns;
     
     pr_info("get_page: start allocation of order %d (%lu pages)\n", PAGE_ORDER, (1UL << PAGE_ORDER));

     start = ktime_get();
     addr = __get_free_pages(GFP_KERNEL, PAGE_ORDER); 
     end = ktime_get();
     
     if (!addr) {
         pr_err("get_page: FAIL (could not allocate pages)\n");
         return -ENOMEM;
     }
     
     actual_ns = ktime_to_ns(ktime_sub(end, start));
     
     pr_info("get_page: SUCCESS, time: %lld us, addr: %p, type: physical pages\n", actual_ns / 1000, (void *)addr);
     
     free_pages(addr, PAGE_ORDER); 
     
     return 0;
}

static void __exit ex_get_page_exit(void) {
     pr_info("get_page: module unloaded\n");
}

module_init(ex_get_page_init);
module_exit(ex_get_page_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anton Zubin");
MODULE_DESCRIPTION("Demonstration of get_free_pages");

