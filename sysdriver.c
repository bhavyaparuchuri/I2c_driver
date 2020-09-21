#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/mutex.h>
#include<linux/i2c.h>
#include<linux/slab.h>
#include<linux/sysfs.h>
#include<linux/fs.h>
#include<linux/property.h>


/* driver private structure to hold global config data used by all driver routines */
struct at24_prv {
	struct i2c_client *client_prv;
	struct kobject *at24_kobj;
	char *ptr;
	unsigned int size;
	unsigned int pagesize;
	unsigned int address_width;
	unsigned int page_no;
};

struct at24_prv *obj=NULL;


static ssize_t write(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count)
{
	int off;

	off = ((obj->page_no)*32);
	struct i2c_msg msg = {0};
	//unsigned long timeout,write_time;
	ssize_t status;
	char msgbuf[34];

	memset(msgbuf,0,sizeof(msgbuf));

	if(count>=32)
		count = 32;

	msg.addr = obj->client_prv->addr;
	msg.flags = 0;
	msg.buf = msgbuf;
	msg.buf[0] = off>>8;
	msg.buf[1] = off;
	memcpy(&msg.buf[2],buf,count);

	msg.len = count+2;

	status = i2c_transfer(obj->client_prv->adapter,&msg,1);
	if(status<0)
		pr_info("i2c transfer got failed\n");

	return count;
}

static ssize_t read(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{

	/*
	1.resolve offset
	2.construct an i2c_msg instance to transfer the offset to the device
	3.construct an i2c_msg object for fetching page data
	4.initiate an i2c_transfer
	*/
	int off,retval,i;


	struct i2c_msg msg[2];

	u8 msgbuf[2];

	memset(msg,0,sizeof(msg));
	memset(msgbuf,'\0',sizeof(msgbuf));

	off = (obj->page_no*32);
	msg[0].addr = obj->client_prv->addr;

	msgbuf[0] = off>>8;
	msgbuf[1] = off;

	msg[0].flags = 0;
	msg[0].len = 2;
	msg[0].buf = msgbuf;

	msg[1].addr = obj->client_prv->addr;

	msg[1].flags = 1;
	msg[1].len = 32;
	msg[1].buf = buf;


	retval = i2c_transfer(obj->client_prv->adapter,msg,2);
	if(retval<0)
		pr_info("i2c_transfer got failed\n");

	for(i=0;buf[i];i++);
	return i;

}

static ssize_t get_pageno(struct kobject *kobj,struct kobj_attribute *attr,char *pageno)
{
	sprintf(pageno,"%u\n",obj->page_no);
}

static ssize_t set_pageno(struct kobject *kobj,struct kobj_attribute *attr,const char *pageno,size_t count)
{
	unsigned int i;

	if(!(kstrtouint(pageno,0,&i)))
	{
		if((i >= 0)&&(i <= 127))
			obj->page_no = i;
		else
			printk("input is invalid format");
	}
	return count;
}

static struct kobj_attribute data_attribute = __ATTR(rdwr,S_IRUGO|S_IWUSR,read,write);
static struct kobj_attribute page_attribute = __ATTR(pageno,S_IRUGO|S_IWUSR,get_pageno,set_pageno);

static struct attribute *attrs[] = {
        	&data_attribute.attr,
        	&page_attribute.attr,
        	NULL,  /* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
        .attrs = attrs,
};
static int at24_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	int retval;
	/* 1. gather device configuration data from DT node */
	 

	obj = (struct at24_prv *)kmalloc(sizeof(struct at24_prv),GFP_KERNEL);
	if(obj == NULL)
	{
		pr_info("Requested memory not allocated\n");
		return -ENOMEM;
	}

	obj->client_prv = client;
	/* using kernel provide device_property_read family of calls access configuration data declared in dt node
		kernel header file linux/property.h contains definitions of device_property_read function calls */

	if(device_property_read_u32(&client->dev,"size",&obj->size)==0){
	}
	else{
		dev_err(&client->dev,"Error : missing \"size\" property\n");
		return -ENODEV;
	}

	if(device_property_read_u32(&client->dev,"pagesize",&obj->pagesize)==0){
	}
	else{
		dev_err(&client->dev,"Error : missing \"pagesize\" property\n");
		return -ENODEV;
	}

	if(device_property_read_u32(&client->dev,"address-width",&obj->address_width)==0){
	}
	else{
		dev_err(&client->dev,"Error : missing \"address-width\" property\n");
		return -ENODEV;
	}

	/* register with sysfs interface for application interaction */

	obj->at24_kobj = kobject_create_and_add("my_eeprom",NULL);

	retval = sysfs_create_group(obj->at24_kobj,&attr_group);
        if(retval)
                kobject_put(obj->at24_kobj);

	return 0;
}

static int at24_remove(struct i2c_client *client)
{
	kobject_put(obj->at24_kobj);
	return 0;
}

static const struct i2c_device_id eeprom_id[] = {
	{"24c32",0x50},
	{ }
};
MODULE_DEVICE_TABLE(i2c,eeprom_id);

static struct i2c_driver at24_driver = {
	.driver = {
		.name = "at24",
		.owner = THIS_MODULE,
	},
	.probe = at24_probe,
	.remove = at24_remove,
	.id_table = eeprom_id,
};

module_i2c_driver(at24_driver);

/*
 static int __init at24_init()
 {
 i2c_add_driver(&at24_driver);
 return 0;
 }

 static void __exit at24_exit()
 {
 i2c_del_driver(&at24_driver);
 }

 module_init(at24_init);
 module_exit(at24_exit);
 */

MODULE_LICENSE("GPL");
