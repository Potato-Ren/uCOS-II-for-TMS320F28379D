#include <string.h>
#include "F28x_Project.h"
#include "F2837xD_Ipc_drivers.h"
#include <app_cfg.h>
#include <ucos_ii.h>
#include <cpu_core.h>
#include <lib_def.h>
__interrupt void cpu_timer0_isr(void);
#pragma DATA_SECTION(App_TaskStartStk,".stackvar");
#pragma DATA_SECTION(App_TaskPendStk,".stackvar");
#pragma DATA_SECTION(App_TaskPostStk,".stackvar");

CPU_STK_SIZE App_TaskStartStk[APP_CFG_TASK_STK_SIZE];
/* Ping Task's    stack.*/
CPU_STK_SIZE App_TaskPendStk[APP_CFG_TASK_STK_SIZE];
/* Pong Task's    stack.*/
CPU_STK_SIZE App_TaskPostStk[APP_CFG_TASK_STK_SIZE];
static OS_EVENT *AppTaskObjSem;
/*********************************************************************************************
************
* FUNCTION PROTOTYPES
**********************************************************************************************
***********
*/
/* Start Task.*/
static void App_TaskStart(void *p_arg);
/* Ping Task. */
//static void App_TaskPing (void *p_arg);
/* Pong Task. */
static void App_TaskPong (void *p_arg);


void CPU_Initfunc(void){
InitSysCtrl();
InitGpio();
DINT;
InitPieCtrl();
IER = 0x0000;
IFR = 0x0000;
InitPieVectTable();
EINT; // Enable Global interrupt INTM
ERTM; // Enable Global real time interrupt DBG
}
void BSP_INIT(void){
GPIO_SetupPinMux(10, GPIO_MUX_CPU1, 0);
GPIO_SetupPinOptions(10, GPIO_OUTPUT, GPIO_ASYNC);
}
void BSP_LED_Off(void){
GpioDataRegs.GPADAT.bit.GPIO10 = 0;
}
void BSP_Tick_Init(void){
EALLOW; // This is needed to write to EALLOW protected registers
PieVectTable.TIMER0_INT = &cpu_timer0_isr;
PieVectTable.OS_CPU_RTOSINT = &OS_CPU_RTOSINT_Handler;  // RTOS
EDIS; // This is needed to disable write to EALLOW protected registers
InitCpuTimers();
//CpuTimer0  1000=1ms
//CpuTimer1 setup sample timer time
ConfigCpuTimer(&CpuTimer0, 200, 4000);
CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
IER |= M_INT1;
IER |= M_INT13;
//IER |= M_INT14;
CpuTimer0Regs.TCR.bit.TSS = 1;
PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
EINT; // Enable Global interrupt INTM
ERTM; // Enable Global realtime interrupt DBGM
CpuTimer0Regs.TCR.all = 0x4000;
}
void BSP_LED_Toggle(void){
GpioDataRegs.GPATOGGLE.bit.GPIO10 = 1;
}

int main (void)
{
/* Initialize the CPU and    Board.*/
//CPU_Init();
//BSP_Init();
CPU_Initfunc();
BSP_INIT();
OSInit();
DELAY_US(1000000);
/* Create the Start    task.*/
OSTaskCreateExt(App_TaskStart,
(void *)0,
(CPU_STK *)&App_TaskStartStk[0],
(INT8U )APP_CFG_TASK_START_PRIO,
(INT16U )APP_CFG_TASK_START_PRIO,
(CPU_STK *)&App_TaskStartStk[APP_CFG_TASK_STK_SIZE - 1u],
(INT32U )APP_CFG_TASK_STK_SIZE,
(void *)0,
(INT16U )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
/* Start multitasking (i.e.
give control to uC/OS-II). */
OSStart();
/* Should never get    here.*/
while (DEF_TRUE) {
;
}
}
/*
**********************************************************************************************
***********
* App_TaskStart()
*
* Description : First task to be scheduled. Creates the application tasks.
*
* Argument(s) : p_arg the argument passed by 'OSTaskCreateExt()'.
*
* Return(s) : none.
*
* Caller(s) : This is a task.
*
* Note(s) : (1) This task creates the application task. The application is a simple LED
blinking
* demo where LD1 and LD4 blink at a 4:3 polyrhythm.
**********************************************************************************************
***********
*/
static void App_TaskStart (void *p_arg)
{
CPU_INT08U os_err;
/* Prevent compiler warning
for not using 'p_arg' */
(void)&p_arg;
/* Clear the    LEDs.*/
BSP_LED_Off();
/* Start the    Ticker.*/
BSP_Tick_Init();
/* Create the Ping task.*/
AppTaskObjSem = OSSemCreate(0);

/* Create the Pongtask.*/
OSTaskCreateExt(App_TaskPong,
(void *)0,
(CPU_STK *)&App_TaskPostStk[0],
(INT8U )APP_CFG_TASK_POST_PRIO,
(INT16U )APP_CFG_TASK_POST_PRIO,
(CPU_STK *)&App_TaskPostStk[APP_CFG_TASK_STK_SIZE - 1u],
(INT32U )APP_CFG_TASK_STK_SIZE,
(void *)0,
(INT16U )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
/* All tasks should be
written as an infinite
loop. */
while (DEF_TRUE) {
os_err = OSSemPost(AppTaskObjSem);
//OSTimeDlyHMSM(0, 0, 0, 10);
OSTimeDly(80);
DELAY_US(1000);
}
}
/*
**********************************************************************************************
***********
* App_TaskPing()
*
* Description : 'Ping' task, toggles LD1.
*
* Argument(s) : p_arg the argument passed by 'OSTaskCreateExt()'.
*
* Return(s) : none.
*
* Caller(s) : This is a task.
**********************************************************************************************
***********
*/
//static void App_TaskPing (void *p_arg)
//{
// CPU_INT08U os_err;
// /* Prevent compiler
//warning for not using 'p_arg' */
// (void)&p_arg;
//
// /* Task body, always
//written as an infinite loop. */
// while (DEF_TRUE) {
// OSSemPend( AppTaskObjSem,
// 0,
// &os_err);
// }
//}
/*
**********************************************************************************************
***********
* App_TaskPong)
*
* Description : 'Pong' task, toggles LD4.
*
* Argument(s) : p_arg the argument passed by 'OSTaskCreateExt()'.
*
* Return(s) : none.
*
* Caller(s) : This is a task.
**********************************************************************************************
***********
*/
static void App_TaskPong (void *p_arg)
{
/* Prevent compiler warning
for not using 'p_arg' */
(void)&p_arg;
/* Task body, always written
as an infinite loop. */
while (DEF_TRUE) {
BSP_LED_Toggle();
//OSTimeDlyHMSM(0, 0, 0, 20);
//DELAY_US(300000);
OSTimeDly(2);

}
}


__interrupt void cpu_timer0_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

OSIntEnter();
OSTimeTick();
OSIntExit();

}
