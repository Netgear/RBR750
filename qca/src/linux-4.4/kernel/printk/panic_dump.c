#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
#include <linux/slab.h>
#endif
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/concat.h>
#include <linux/mtd/partitions.h>
//#include <asm/addrspace.h>
#include <linux/init.h>
#include <linux/err.h>
#include <asm/io.h>
extern int get_logsize (void);
extern char *get_logbuf (void);
extern int panic_crash_log;
int mtd_store_crash(void)
{
	struct mtd_info *mtd = NULL;
    struct erase_info ei;
    loff_t ofs = 0, start_ofs = 0;
	int i;
	char *buffer;
	unsigned char buf[16];
	int buf_len = 0,remain_len = 0;
	int len = 0;
    int ret = 0;
    size_t wrlen = 0;
	u_char *bak = NULL;    

	printk(KERN_EMERG"%s(%d): Trying to store crash\n", __FUNCTION__, __LINE__);

	mtd = get_mtd_device_nm("DebugMsg");
	if(!mtd)
	{
		printk(KERN_EMERG"%s(%d):DebugMsg partition not found\n", __FUNCTION__, __LINE__);
		return 1;
	}
	
	if(!IS_ERR(mtd))
	{
		;
	}else
	{
		printk(KERN_EMERG"%s(%d): Could not find DebugMsg partition\n", __FUNCTION__, __LINE__);
		return 0;
	}
		
	buf_len = get_logsize();
	buffer = get_logbuf();
	
	printk(KERN_EMERG"%s(%d): buf_len (%d) mtd->erasesize(%d)\n", __FUNCTION__, __LINE__,buf_len,mtd->erasesize);
	
	if (buf_len > mtd->erasesize){
		buf_len = mtd->erasesize;
	}
	printk(KERN_EMERG"%s(%d): buf_len (%d) mtd->erasesize(%d)\n", __FUNCTION__, __LINE__,buf_len,mtd->erasesize);	
	for(ofs=0; ofs<mtd->size; ofs += mtd->erasesize)
	{
		printk(KERN_EMERG"%s(%d) ofs(%d)\n", __FUNCTION__, __LINE__,ofs);
	    if(mtd_block_isbad(mtd, ofs) == 0){
	    	printk(KERN_EMERG"%s(%d) ofs(%d)\n", __FUNCTION__, __LINE__,ofs);
	        break;
	    }
	}
	
	printk(KERN_EMERG"%s(%d)\n", __FUNCTION__, __LINE__);
	if(ofs >= mtd->size)
	{
	    printk(KERN_EMERG"warning: %s:%d No good block to write data\n", __FUNCTION__,__LINE__);	
	    goto end_store;
	}
	
	start_ofs = ofs;  
	ei.mtd = mtd;
	ei.callback = NULL;
	//ei.addr = ofs;
	ei.addr = start_ofs;
	ei.len = mtd->erasesize;
	ei.priv = 0;
	
	ret = mtd->_erase(mtd, &ei);
	bak = buffer;
    printk(KERN_EMERG"%s(%d)\n", __FUNCTION__, __LINE__);
	if(ret != 0) 
	{
		printk(KERN_EMERG"warning: %s:%d fail to erase\n", __FUNCTION__,__LINE__);
		goto end_store;
	}  
	printk(KERN_EMERG"%s(%d)\n", __FUNCTION__, __LINE__);

	memset(buf,0,sizeof(buf));
	mtd->_read(mtd, 0, sizeof(buf), &len, buf);

	for (len=0;len<sizeof(buf);len++)
	{
		if (buf[len]!=0xff) 
		{
			printk(KERN_EMERG"%s(%d): Could not save crash, partition not clean len %d (%02x)\n", __FUNCTION__, __LINE__,len,buf[len]);
			break;
		}
	}


    len = 0;
	remain_len = buf_len;
    wrlen = len;    
    do{
    	ret = mtd->_write(mtd, 0, remain_len, &len, bak);
	    if(ret != 0)
	    {
	        if(ret != -EIO)
	        {
	            goto end_store;
	        }
	        /* bad blcok */
	        break;
	    }    	

    	wrlen += len;
    	bak += len;
    	printk(KERN_EMERG"%s(%d) %d : %d\n", __FUNCTION__, __LINE__,buf_len,wrlen);
		if (buf_len ==  wrlen){
			printk(KERN_EMERG"%s(%d): Crash Saved\n", __FUNCTION__, __LINE__);
			panic_crash_log = 0;
			goto end_store;
		}else if(remain_len == 0){
			printk(KERN_EMERG"%s(%d): Crash Saved remain_len(%d\n", __FUNCTION__, __LINE__,remain_len);
			panic_crash_log = 0;
			goto end_store;			
		}else{
			remain_len -= len;
			printk(KERN_EMERG"%s(%d): keep write crash info remain_len(%d)\n", __FUNCTION__, __LINE__,remain_len);		
		}    	
	}while(len != buf_len);
    
end_store:
	put_mtd_device(mtd);		
	return 0;
}

EXPORT_SYMBOL(mtd_store_crash);