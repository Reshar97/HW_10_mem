#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#define ALLOC_SIZE (1024 * 1024)

static int __init ex_kmalloc_init(void) {
    void *ptr;
    ktime_t start, end;
    s64 diff_ns;

    printk(KERN_INFO "kmalloc: Попытка выделить %d байт\n", ALLOC_SIZE);

    start = ktime_get(); // Точное время "до"
    ptr = kmalloc(ALLOC_SIZE, GFP_KERNEL);
    end = ktime_get(); // Точное время "после"

    if (!ptr) {
        printk(KERN_ERR "kmalloc: Ошибка выделения памяти\n");
        return -ENOMEM;
    }

    // Вычисляем разницу в наносекундах
    diff_ns = ktime_to_ns(ktime_sub(end, start));
    printk(KERN_INFO "kmalloc: %d байт выделено за %lld us (микросекунд)\n", ALLOC_SIZE, diff_ns / 1000);

    kfree(ptr);
    return 0;
}

static void __exit ex_kmalloc_exit(void) {
    printk(KERN_INFO "kmalloc: Модуль выгружен\n");
}

module_init(ex_kmalloc_init);
module_exit(ex_kmalloc_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anton Zubin");
MODULE_DESCRIPTION("Пример kmalloc");
