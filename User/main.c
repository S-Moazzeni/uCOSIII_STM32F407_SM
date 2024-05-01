#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "os_app_hooks.h"
#include "user_led.h"
/**************************************************************************/
//Start Task
#define START_TASK_PRIO		3
#define START_STK_SIZE 		512
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);
/**************************************************************************/
// Task 1
#define Task1_PRIO		4
#define Task1_STK_SIZE 		128
OS_TCB Task1_TCB;
CPU_STK Task1_STK[Task1_STK_SIZE];
void Task1(void *p_arg);
/**************************************************************************/
// Task 2
#define Task2_PRIO		5
#define Task2_STK_SIZE 		128
OS_TCB Task2_TCB;
CPU_STK Task2_STK[Task2_STK_SIZE];
void Task2(void *p_arg);
/**************************************************************************/
// Task 3
#define Task3_PRIO		6
#define Task3_STK_SIZE 		128
OS_TCB Task3_TCB;
__align(8) CPU_STK Task3_STK[Task3_STK_SIZE];
void Task3(void *p_arg);
/**************************************************************************/
// Main	
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);				
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);  
	LED_Init();

	OSInit(&err);		
	OS_CRITICAL_ENTER();

	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				
	OS_CRITICAL_EXIT();		 
	OSStart(&err);  
	while(1);
}

/**************************************************************************/
// start_task
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����LED0����
	OSTaskCreate(	(OS_TCB 	* )&Task1_TCB,		
								(CPU_CHAR	* )"task1", 		
								(OS_TASK_PTR )Task1, 			
								(void		* )0,					
								(OS_PRIO	  )Task1_PRIO,     
								(CPU_STK   * )&Task1_STK[0],	
								(CPU_STK_SIZE)Task1_STK_SIZE/10,	
								(CPU_STK_SIZE)Task1_STK_SIZE,		
								(OS_MSG_QTY  )0,					
								(OS_TICK	  )0,					
								(void   	* )0,					
								(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
								(OS_ERR 	* )&err);				
				 
	OSTaskCreate(	(OS_TCB 	* )&Task2_TCB,		
								(CPU_CHAR	* )"task2", 		
								(OS_TASK_PTR )Task2, 			
								(void		* )0,					
								(OS_PRIO	  )Task2_PRIO,     
								(CPU_STK   * )&Task2_STK[0],	
								(CPU_STK_SIZE)Task2_STK_SIZE/10,	
								(CPU_STK_SIZE)Task2_STK_SIZE,		
								(OS_MSG_QTY  )0,					
								(OS_TICK	  )0,					
								(void   	* )0,					
								(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
								(OS_ERR 	* )&err);
								 
	OSTaskCreate(	(OS_TCB 	* )&Task3_TCB,		
								(CPU_CHAR	* )"task3", 		
								(OS_TASK_PTR )Task3, 			
								(void		* )0,					
								(OS_PRIO	  )Task3_PRIO,     
								(CPU_STK   * )&Task3_STK[0],	
								(CPU_STK_SIZE)Task3_STK_SIZE/10,	
								(CPU_STK_SIZE)Task3_STK_SIZE,		
								(OS_MSG_QTY  )0,					
								(OS_TICK	  )0,					
								(void   	* )0,					
								(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
								(OS_ERR 	* )&err);
				 
	//���������������
//	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
//				 (CPU_CHAR	* )"float test task", 		
//                 (OS_TASK_PTR )float_task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
//                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
//                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
//                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,					
//                 (void   	* )0,				
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
//                 (OS_ERR 	* )&err);				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�����ٽ���
}
/**************************************************************************/
// Task1
void Task1(void *p_arg)
{	 	
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LED0 = ~LED0;
		printf("Task1\r\n");
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	};
}
/**************************************************************************/
// Task2
void Task2(void *p_arg)
{	  
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		printf("Task2\r\n");
		OSTaskQPost(	(OS_TCB *)&Task3_TCB,         //?????????????
									(void *)"Bee",                 //???????
									(OS_MSG_SIZE)sizeof ("Bee"),        //???????
									(OS_OPT)OS_OPT_POST_FIFO,         //??????????????????????
									(OS_ERR *)&err);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	};
}
/**************************************************************************/
// Task3
void Task3(void *p_arg)
{
	OS_ERR  err; 
	CPU_TS         ts;
	OS_MSG_SIZE    msg_size;
	char * pMsg;
	
	while(1){
		printf("Task_3 Execution...\r\n");    
		pMsg = OSTaskQPend (  (OS_TICK        )0,                    //????????
													(OS_OPT         )OS_OPT_PEND_BLOCKING, //???????? ??????????????
													(OS_MSG_SIZE   *)&msg_size,            //???????????
													(CPU_TS        *)&ts,                  //???????????????????
													(OS_ERR        *)&err);                //???????????
		if(strcmp(pMsg,"Bee")==0)
				printf("Task_3 Done!\r\n");
	}
}

