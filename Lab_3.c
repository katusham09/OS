#include <linux/module.h>
#include <linux/printk.h>
 
int init_module(void) 
{ 
    pr_info("Welcome to the Tomsk State University\n"); 
    return 0; 
} 
 
void cleanup_module(void) 
{ 
    pr_info("Tomsk State University forever!\n"); 
} 
 
MODULE_LICENSE("GPL");
