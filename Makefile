obj-m += ex_kmalloc.o
obj-m += ex_vmalloc.o
obj-m += ex_kmem_cache.o
obj-m += ex_mempool.o
obj-m += ex_get_page.o

all: make

make:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	insmod ex_kmalloc.ko
	insmod ex_vmalloc.ko
	insmod ex_kmem_cache.ko
	insmod ex_mempool.ko
	insmod ex_get_page.ko

unload:
	rmmod ex_kmalloc
	rmmod ex_vmalloc
	rmmod ex_kmem_cache
	rmmod ex_mempool
	rmmod ex_get_page

format:
	clang-format -i *.c

check:
	dmesg -c
	make load
	dmesg | grep "kmalloc\|vmalloc\|kmem_cache\|mempool\|get_page"
	make unload
