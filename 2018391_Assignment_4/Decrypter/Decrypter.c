#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include <linux/random.h>
#include <linux/string.h>

#define deviceName "Decypter"

//Method Signatures
static int definitionOfOpen(struct inode*, struct file*);
static int definitionOfExit(struct inode*, struct file*);
static ssize_t definitionOfRead(struct file*, char*, size_t, loff_t*);
static ssize_t definitionOfWrite(struct file*, const char*, size_t, loff_t*);

static struct file_operations structure = {
	.open = definitionOfOpen,
	.release = definitionOfExit,
	.read = definitionOfRead,
	.write = definitionOfWrite,
};

static char userMessage[1024]={0};
static int messageLength=0;

static int status;

static int definitionOfOpen(struct inode* in, struct file* fil){
	return 0;
}

static int __init initializedFunction(void){
	status = register_chrdev(0, deviceName, &structure);
	if(status<0){
		printk(KERN_ALERT "Failed\n");
		return status;
	}
	printk("DECRYPTOR Successfully injected : %d\n",status);
	return 0;
}

static int definitionOfExit(struct inode* in, struct file* fil){
	unregister_chrdev(0,deviceName);
	return 0;
}

static ssize_t definitionOfRead(struct file* fil, char* buffer, size_t length, loff_t* off){
	copy_to_user(buffer,userMessage,messageLength);
	return messageLength;
}

static ssize_t definitionOfWrite(struct file* fil, const char* buffer, size_t length, loff_t* off){
	char localBuffer[1024];
	memset(localBuffer,0,1024);
	memset(userMessage,0,1024);
	int valueToReturn = copy_from_user(localBuffer,buffer,length);
	int index = 16;
	int userIndex = 0;
	while(index<length){
		userMessage[userIndex] = localBuffer[index]^localBuffer[index%16];
		index++;userIndex++;
	}
	messageLength=length-16;
	return valueToReturn;
}




static void __exit exitFunction(void){
	printk("DECRYPTOR Successfully Ejected\n");
}



module_init(initializedFunction);
module_exit(exitFunction);