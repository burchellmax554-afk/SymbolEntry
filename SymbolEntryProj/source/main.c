/* main.c */
#include "MCUType.h"                 /* Defines MCU-specific data types */
#include "os.h"                      /* uC/OS-III header for RTOS functions */
#include "FRDM_MCXN947_GPIO.h"       /* Board-specific GPIO functions */
#include "BasicIO.h"                 /* Basic IO functions for console output */
#include "FRDM_MCXN947ClkCfg.h"      /* Board-specific clock configuration */
#include "app_cfg.h"                 /* Application configuration */
#include "CsOS_SW.h"                 /* Switch handling functions */
#include "menu.h"                    /* Menu handling functions and constants (Very customizable) */


/*****************************************************************************************
* Allocate task control blocks for different tasks.
*****************************************************************************************/
static OS_TCB appStartTaskTCB;           /* Task Control Block for the start task */
static OS_TCB appTaskSymbolControlTCB;        /* Task Control Block for Symbol Control task */

/*****************************************************************************************
* Allocate stack space for each task.
*****************************************************************************************/
static CPU_STK appStartTaskStk[APP_CFG_TASK_START_STK_SIZE];
static CPU_STK appTaskSymbolControlStk[APP_CFG_TASK_SYMBOL_CONTROL_STK_SIZE];

/*****************************************************************************************
* Task Function Prototypes.
*   These tasks are private within this module and are declared here.
*****************************************************************************************/
static void appStartTask(void *p_arg);         /* Start task function prototype */
static void appTaskSymbolControl(void *p_arg);      /* Symbol  control task */

/*****************************************************************************************
* main()
*****************************************************************************************/
void main(void) {
    OS_ERR os_err;

    /* Initialize the clock for the board */
    FRDM_MCXN947InitBootClock();

    /* Set up Basic IO for console communication at 115200 baud */
    BIOOpen(BIO_BIT_RATE_115200);

    /* Disable all interrupts at the start (OS will manage interrupts later) */
    CPU_IntDis();

    /* Initialize uC/OS-III */
    OSInit(&os_err);
    assert(os_err == OS_ERR_NONE);  /* Ensure OS initialization is successful */

    /* Create the start task (first task to run) */
    OSTaskCreate(&appStartTaskTCB,
                 "Start Task",
                 appStartTask,
                 (void *) 0,
                 APP_CFG_TASK_START_PRIO,
                 &appStartTaskStk[0],
                 (APP_CFG_TASK_START_STK_SIZE / 10u),
                 APP_CFG_TASK_START_STK_SIZE,
                 0,
                 0,
                 (void *) 0,
                 (OS_OPT_TASK_NONE),
                 &os_err);
    assert(os_err == OS_ERR_NONE); /* Ensure task creation is successful */

    /* Start multitasking and give control to uC/OS-III */
    OSStart(&os_err);
    assert(0);  /* The program should never reach this point */
}


/*****************************************************************************************
* STARTUP TASK
* This task runs once and performs initialization tasks before creating other tasks.
*****************************************************************************************/
static void appStartTask(void *p_arg) {
    OS_ERR os_err;

    (void)p_arg;  /* Avoid unused parameter warning */

    /* Initialize system tick for OS and calculate CPU usage statistics */
    OS_CPU_SysTickInitFreq(SystemCoreClock);
    OSStatTaskCPUUsageInit(&os_err);

    /* Initialize LEDs, Debug bits, switches, and touchscreen */
    GpioLEDGREENInit();
    GpioLEDREDInit();
    GpioDBugBitsInit();
    SwInit();


    /* Create the Symbol Control task */
    OSTaskCreate(&appTaskSymbolControlTCB,
             "App Task Symbol Control",
             appTaskSymbolControl,
             (void *) 0,
             APP_CFG_TASK_SYMBOL_CONTROL_PRIO,
             &appTaskSymbolControlStk[0],
             (APP_CFG_TASK_SYMBOL_CONTROL_STK_SIZE / 10u),
             APP_CFG_TASK_SYMBOL_CONTROL_STK_SIZE,
             0,
             0,
             (void *) 0,
             (OS_OPT_TASK_NONE),
             &os_err);
    assert(os_err == OS_ERR_NONE); /* Ensure task creation is successful */



    /* Delete the start task as it is no longer needed */
    OSTaskDel((OS_TCB *)0, &os_err);
    assert(os_err == OS_ERR_NONE);
}
/*****************************************************************************************
* TASK - Symbol Control
* This task handles 2 jobs depending on the button press of either sending  or switching
* the symbols
*****************************************************************************************/
static void appTaskSymbolControl(void *p_arg) {
    OS_ERR os_err;               /* To catch OS errors */
    INT32U sw_in = 0;           /* Switch input value */

    (void)p_arg;

    /* Display the initial menu with the first symbol selected */
    SetCurrentSymbolIndex(0);
    const INT8C *symbol = GetCurrentSymbol();
    BIOPutStrg("\nSYMBOL_IDX: ");
    BIOPutStrg(symbol);
    BIOPutStrg("\n");

    while (1) {
        sw_in = SwPend(0, &os_err);  /* Block until Switch 2 or Switch 3 press */
        assert(os_err == OS_ERR_NONE); /* Should be no error in the switch press */

        /* Check for Switch 2 press event */
        if (sw_in == SW2) {
            /* Move to the next symbol in the list, wrapping at SYMBOL_COUNT */
        	INT8U next_index = (INT8U)((GetCurrentSymbolIndex() + 1) % SYMBOL_COUNT);
            SetCurrentSymbolIndex(next_index); /* Update index to match next_index and update the display */
            symbol = GetCurrentSymbol();
            BIOPutStrg("\nSYMBOL_IDX: ");
            BIOPutStrg(symbol);
            BIOPutStrg("\n");
        }

        /* Check for Switch 3 press */
        if (sw_in == SW3) {
            /* PROTOTYPE CODE TO TEST MY PROGRAM ON */
        	/* I eventually want something more streamlined */
            symbol = GetCurrentSymbol();
            BIOPutStrg("\nYour Current Symbol Is: ");
            BIOPutStrg(symbol);
            BIOPutStrg("\n");
        }
    }
}
