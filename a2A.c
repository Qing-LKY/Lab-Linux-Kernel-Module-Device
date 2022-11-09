#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

dev_t dev_no;
struct cdev chrdev; 
struct mutex lock;

int open_dev(struct inode *i, struct file *f) {
    mutex_lock(&lock);
    printk(KERN_INFO "Open a2A dev!");
    return 0;
}

int release_dev(struct inode *i, struct file *f) {
    printk(KERN_INFO "Release a2A dev!");
    mutex_unlock(&lock);
    return 0;
}

#define POOL_SIZE (1 << 14)

char pool[POOL_SIZE];

static ssize_t
writea(struct file *f, const char __user *buf, size_t count, loff_t *p) {
    int ret, cnt;
    char tmp;
    cnt = count;
    while(cnt) {
        *p %= POOL_SIZE;
        ret = copy_from_user(pool + *p, buf, 1);
        if(ret != 0) return ret;
        tmp = pool[*p];
        if(tmp >= 'a' && tmp <= 'z') {
            tmp += -'a' + 'A';
        }
        pool[*p] = tmp;
        (*p)++;
        buf++;
        cnt--;
    }
    return count;
}

static ssize_t
readA(struct file *file, char __user *buf, size_t count, loff_t *p) {
    int ret, cnt;
    cnt = count;
    while(cnt) {
        *p %= POOL_SIZE;
        ret = copy_to_user(buf, pool + *p, 1);
        if(ret != 0) return ret;
        (*p)++;
        buf++;
        cnt--;
    }
    return count;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = open_dev,
    .write = writea,
    .read = readA,
    .release = release_dev,
    .llseek = default_llseek
};

static int init_a2A(void) {
    int ret;
    printk(KERN_INFO "Init a2A Module!");
    ret = alloc_chrdev_region(&dev_no, 0, 1, "a2A");
    if(ret != 0) return 1;
    cdev_init(&chrdev, &fops);
    chrdev.owner = THIS_MODULE;
    cdev_add(&chrdev, dev_no, 1);
    mutex_init(&lock);
    return 0;
}

static void exit_a2A(void) {
    printk(KERN_INFO "Goodbye!");
    unregister_chrdev_region(dev_no, 1);
    cdev_del(&chrdev);
}

MODULE_LICENSE("GPL");
module_init(init_a2A);
module_exit(exit_a2A);
