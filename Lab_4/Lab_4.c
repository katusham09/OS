#include <linux/module.h> 
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/time64.h>

#define PROC_FILENAME "tsulab"
#define START_YEAR 1947
#define START_MONTH 8
#define START_DAY 15

static struct proc_dir_entry *proc_file;

static ssize_t tsulab_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    struct tm start_time = {
        .tm_year = START_YEAR, 
        .tm_mon = START_MONTH,  
        .tm_mday = START_DAY,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    struct timespec64 now; 
    time64_t start_epoch; 
    long minutes; 
    char buffer[128]; 
    int len; 

    ktime_get_real_ts64(&now); 
    start_epoch = mktime64(start_time.tm_year, start_time.tm_mon, start_time.tm_mday, start_time.tm_hour, start_time.tm_min, start_time.tm_sec);

    minutes = (now.tv_sec - start_epoch) / 60;
    len = snprintf(buffer, sizeof(buffer), "Республика Индия существует %ld минут\n", minutes);

    if (*ppos >= len) {
        return 0;
    }

    if (copy_to_user(buf, buffer, len)) {
        return -EFAULT;
    }

    *ppos += len;
    return len;
}

static const struct proc_ops tsulab_fops = {
    .proc_read = tsulab_read,
};

int init_module(void)
{
    proc_file = proc_create(PROC_FILENAME, 0, NULL, &tsulab_fops);

    if (!proc_file) {
        pr_err("Error!!!");
        return -ENOMEM;
    }

    pr_info("Module loaded!\n");
    return 0;
}

void cleanup_module(void)
{
    if (proc_file) {
        proc_remove(proc_file);
    }
    
    pr_info("Tomsk State University forever!\n");
}

MODULE_LICENSE("GPL");
