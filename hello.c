#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/input.h>

MODULE_LICENSE("Dual BSD/GPL");
extern void set_input_touch_click(int x,int y);
extern void set_input_touch_slide(int start_x,int start_y,int end_x,int end_y);

//点击
#define INPUT_TOUCH_CLICK    1
//滑动
#define INPUT_TOUCH_SLIDE    2

struct zeyu_io_cmd_data{
    int cmd;
    union
    {
        int x_start;
        int x;
    } ;
    union
    {
        int y_start;
        int y;
    } ;
    int x_end;
    int y_end;
};

static int myDev_open(struct inode* inode,struct file* filp);
static int myDev_release(struct inode* inode,struct file* filp);
static ssize_t myDev_read(struct file* filp, char __user * buf, size_t count,loff_t* f_pos);
static ssize_t myDev_write(struct file* filp, const char __user* buf, size_t count,loff_t * f_pos);
static long myDev_compat_ioctl(struct file * filp, unsigned int cmd, unsigned long arg);

static const struct file_operations h_ops = {
	.owner		= THIS_MODULE,
	.open		= myDev_open,
	.read		= myDev_read,
	.unlocked_ioctl		= myDev_compat_ioctl,
	.compat_ioctl		= myDev_compat_ioctl,
	.release	= myDev_release,
	.write		= myDev_write,
};

ssize_t zeyu_read_attr(struct device *dev, struct device_attribute *attr, char *buf){
	printk(KERN_ALERT "zeyu zeyu_read_attr\n");
	return 0;
}

ssize_t zeyu_write_attr(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
	printk(KERN_ALERT "zeyu zeyu_write_attr\n");
	return 0;
}

DEVICE_ATTR(zeyu_attr,S_IRUGO | S_IWUSR,zeyu_read_attr,zeyu_write_attr);

//设备类别和设备变量
static struct class* g_class = 0;
dev_t devno = 0;
static struct cdev* dev = 0;
char lele[0x10] = "zeyudev";
static int hello_init(void)
{
    int err = -1;
    struct device* temp = NULL;
    printk(KERN_ALERT "zeyu Hello, world\n");

    //动态分配主设备号与从设备号
    err = alloc_chrdev_region(&devno,0,1,"zeyu_dev");
    if(err < 0){
        printk(KERN_ALERT "zeyu alloc_chrdev_region erro\n");
    }

    dev = kmalloc(sizeof(struct cdev),GFP_KERNEL);
    if(!dev){
        printk(KERN_ALERT "zeyu kmalloc erro\n");
    }
    memset(dev,0,sizeof(struct cdev));
    //初始化设备,初始化结构体
    cdev_init(dev,&h_ops);
    dev->owner = THIS_MODULE;
    dev->ops = &h_ops;

    //注册字符串设备
    err = cdev_add(dev,devno,1);
    if(err < 0){
        printk(KERN_ALERT "zeyu cdev_add erro\n");
    }

    //在/sys/class/下创建设备类别目录
    g_class = class_create(THIS_MODULE,"zeyu_dev");
    if(IS_ERR(g_class)){
        printk(KERN_ALERT "zeyu g_class erro\n");
    }

    //在/dev目录和/sys/class/freg目录下分别创建文件freg
    temp = device_create(g_class,NULL,devno,"%s","zeyudev");
    if(IS_ERR(temp)){
        printk(KERN_ALERT "zeyu device_create erro\n");
    }

    //在/sys/class/freg/freg目录下创建属性文件val
    printk(KERN_ALERT "zeyu dev_attr_zeyu_attr %d\n",dev_attr_zeyu_attr.attr.mode);
    dev_attr_zeyu_attr.attr.name = &lele[0];
    dev_attr_zeyu_attr.attr.mode = 0666;
    printk(KERN_ALERT "zeyu dev_attr_zeyu_attr %d\n",dev_attr_zeyu_attr.attr.mode);
    err = device_create_file(temp,&dev_attr_zeyu_attr);
    if(err < 0){
        printk(KERN_ALERT "zeyu device_create_file erro\n");
    }
    //fchmod(temp,777);
    return 0;
}
static void hello_exit(void)
{
    printk(KERN_ALERT "zeyuu Goodbye, cruel world\n");
}


module_init(hello_init);
module_exit(hello_exit);

static int myDev_open(struct inode* inode,struct file* filp){
    printk(KERN_ALERT "zeyu myDev_open\n");
    return 0;
}
static int myDev_release(struct inode* inode,struct file* filp){
    printk(KERN_ALERT "zeyu myDev_release\n");
    return 0;
}
static ssize_t myDev_read(struct file* filp,char __user *buf,size_t count,loff_t* f_pos){
    printk(KERN_ALERT "zeyu myDev_read\n");
    return 0;
}
static ssize_t myDev_write(struct file* filp,const char __user *buf,size_t count,loff_t* f_pos){
    printk(KERN_ALERT "zeyu myDev_write\n");
    return 0;
}
static long myDev_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    struct zeyu_io_cmd_data data = {0};
    struct zeyu_io_cmd_data* data_user_add = 0;
    unsigned long ret = 0;
    data_user_add = (struct zeyu_io_cmd_data*)arg;
    ret = copy_from_user((void *)(&data),(void __user*)data_user_add,sizeof(data));
    switch(data.cmd){
	case INPUT_TOUCH_CLICK:
		set_input_touch_click(data.x,data.y);
		break;
	case INPUT_TOUCH_SLIDE:
		set_input_touch_slide(data.x_start,data.y_start,data.x_end,data.y_end);
		break;
    }
    //printk(KERN_ALERT "zeyu myDev_compat_ioctl cmd:%d arg:%d arg1:%d\n",data.cmd,data.x,data.y);
    return 0;
}
