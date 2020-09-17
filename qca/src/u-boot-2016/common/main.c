/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* #define	DEBUG	*/

#include <common.h>
#include <autoboot.h>
#include <cli.h>
#include <console.h>
#include <version.h>
#include <uboot_config.h>

DECLARE_GLOBAL_DATA_PTR;

extern void board_reset_default();
extern void StartTftpServerToRecoveFirmware();
/*
 * Board-specific Platform code can reimplement show_boot_progress () if needed
 */
__weak void show_boot_progress(int val) {}

static void modem_init(void)
{
#ifdef CONFIG_MODEM_SUPPORT
	debug("DEBUG: main_loop:   gd->do_mdm_init=%lu\n", gd->do_mdm_init);
	if (gd->do_mdm_init) {
		char *str = getenv("mdm_cmd");

		setenv("preboot", str);  /* set or delete definition */
		mdm_init(); /* wait for modem connection */
	}
#endif  /* CONFIG_MODEM_SUPPORT */
}

static void run_preboot_environment_command(void)
{
#ifdef CONFIG_PREBOOT
	char *p;

	p = getenv("preboot");
	if (p != NULL) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif

		run_command_list(p, -1, 0);

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}
#endif /* CONFIG_PREBOOT */
}

static int init_fan(void)
{
    run_command("mw 0x1019004 0", 0); // Set GPIO_25 to 0
    return 0;
}

static int pwr_blink(int color,int toggle) /*0: white/ 1: amber || 0: off*/
{
	if(!toggle)
	{
	    run_command("mw 0x1036000 0xC0", 0); //Disable green
    	run_command("mw 0x1038000 0xc0", 0); //Disable red
		return 0;		
	}
	if(color == 0)
	{
	    run_command("mw 0x1038000 0xC0", 0); //Disable amber
	    run_command("mw 0x1036000 0xC0", 0); //Disable amber		
    	run_command("mw 0x1036000 0x7C3", 0); //Enable green	
	}else if(color == 1)
	{
	    run_command("mw 0x1036000 0x7C3", 0); //Enable amber
    	run_command("mw 0x1038000 0x7C3", 0); 		
	}else if(color == 2)
	{
	    run_command("mw 0x1036000 0xC0", 0); //Enable red
    	run_command("mw 0x1038000 0x7C3", 0); 		
	}
	return 0;
}


static int check_reset(void)
{
    int i;
    s32 ret = 0 ;
    char cmd[256];
    int toggle = 0;
    
	
#ifdef ORBI11AX
    unsigned int *addr_reset = (unsigned int*)GPIO_IN_OUT_ADDR(33);
    unsigned int *addr_wps = (unsigned int*)GPIO_IN_OUT_ADDR(34);
    run_command("mw 0x1021000 0xc3", 0); // Set GPIO_33 to IN
    run_command("mw 0x1022000 0xc3", 0); // Set GPIO_34 to IN
    mdelay(100);
#else
    unsigned int *addr_reset = (unsigned int*)GPIO_IN_OUT_ADDR(18);
    unsigned int *addr_wps = (unsigned int*)GPIO_IN_OUT_ADDR(44);
#endif      
    if( (*addr_reset==0x0) && (*addr_wps==0x0))  /* reset $ wps button are pressed */
    {
        for (i=0; i<30; i++)
        {
            mdelay(100);
            if( (*addr_reset!=0x0) || (*addr_wps!=0x0))
                return 0;
        }
        /* Enter NMRP mode */
#ifdef ORBI11AX //temp remove to wait I2C of IPQ807 ready
        run_command("i2c led 4 1 100", 0); //Solid amber
#endif
        //setenv("bootargs", "NMRP");
		StartTftpServerToRecoveFirmware();
        return 0;
    }
    else if( *addr_reset == 0x0 )  /* reset button is pressed */
    {
#ifdef ORBI11AX //temp remove to wait I2C of IPQ807 ready
       run_command("i2c led 4 1 100", 0);//Solid Amber
#endif
		
		pwr_blink(1,1);
		toggle = 1;
        for (i=0; i<50; i++)
        {
            mdelay(100);
/*            
            if( (i%5) == 0)
            {
            	toggle++;
            	toggle = toggle%2;            
            	pwr_blink(1,toggle);
			}
*/			
            if(*addr_reset != 0x0)
            {
                run_command("reset", 0);    /*reset button is pressed less than 5 seconds, reboot device */
            }
        }
#ifdef ORBI11AX //temp remove to wait I2C of IPQ807 ready
        run_command("i2c led 4 2 100", 0); /*reset button is pressed more than 5 seconds, pulse amber LED */
#endif
		i = 0;
		pwr_blink(1,1);
		toggle = 1;		
        for(;;)
        {
        
            mdelay(100); 
           	if( (i%5) == 0)
            {
            	toggle++;
            	toggle = toggle%2;
            	pwr_blink(1,toggle);
			}
			i = i%5;
			i++;			           
            if(*addr_reset != 0x0)
                break;  /* reset button is released, reboot device */
        }
        board_reset_default();
        printf("load default!\n");
        mdelay(300);
        run_command("reset", 0);
    }
    return 0;
}


extern int set_firmware_partition(void);


/* We come here after U-Boot is initialised and ready to process commands */
void main_loop(void)
{
	const char *s;

    /* BDDA added start for dual image. Set environment firmware_partition to 1, boot loader will load 2nd kernel+rootfs, otherwise, load 1st kernel+rootfs */
        
    char cmd[256];
    /*
    sprintf(cmd, "setenv firmware_partition %d", run_command("firmware_partition", 0));
    printf("%s\n", cmd);
    run_command(cmd, 0);
    */
    set_firmware_partition();
    /* BDDA added end for dual image */
        
#ifdef ORBI11AX
    init_fan();
#endif

	bootstage_mark_name(BOOTSTAGE_ID_MAIN_LOOP, "main_loop");

#ifndef CONFIG_SYS_GENERIC_BOARD
	puts("Warning: Your board does not use generic board. Please read\n");
	puts("doc/README.generic-board and take action. Boards not\n");
	puts("upgraded by the late 2014 may break or be removed.\n");
#endif

	modem_init();
#ifdef CONFIG_VERSION_VARIABLE
	setenv("ver", version_string);  /* set version variable */
#endif /* CONFIG_VERSION_VARIABLE */

	cli_init();

	run_preboot_environment_command();

#ifndef ORBI11AX //temp remove to wait I2C of IPQ807 ready
    run_command("i2c led 3 3 100", 0);
#endif
#ifdef ORBI11AX 
	run_command("i2c led 3 3 100", 0);
//	printf("%s:%d\n",__func__,__LINE__);
/*	
    run_command("mw 0x1036000 0xC0", 0); //Disable amber
    run_command("mw 0x1038000 0xc3", 0); //Enable white
*/    
    run_command("mw 0x1036000 0xC0", 0); //Disable amber
    run_command("mw 0x1038000 0xC0", 0); //Disable amber
    run_command("mw 0x1036000 0x7C3", 0); //Enable green
#endif
//	printf("%s:%d\n",__func__,__LINE__);
    run_command("mw 0x1036000 0xC0", 0); //Disable amber
    run_command("mw 0x1038000 0xC0", 0); //Disable amber
    run_command("mw 0x1036000 0x7C3", 0); //Enable green
    check_reset();
#ifdef CONFIG_SYS_NMRP
    StartNmrpClient();
#endif

#if defined(CONFIG_UPDATE_TFTP)
	update_tftp(0UL, NULL, NULL);
#endif /* CONFIG_UPDATE_TFTP */

	s = bootdelay_process();
	if (cli_process_fdt(&s))
		cli_secure_boot_cmd(s);

	autoboot_command(s);

	cli_loop();
}
