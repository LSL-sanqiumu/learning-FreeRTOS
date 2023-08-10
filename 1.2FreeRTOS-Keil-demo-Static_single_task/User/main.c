#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"


// ��̬�ڴ��еĳ�ʼ������,��ʼ��GPIO��
static void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED_Task_Handle;

static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE]; 
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

static StaticTask_t Idle_Task_TCB; 
static StaticTask_t Timer_Task_TCB; 

static StackType_t AppTaskCreate_Stack[128];
static StackType_t LED_Task_Stack[128];

static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;

static void LED_Task(void* parameter);
static void AppTaskCreate(void);
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) ;


int main(void) 
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	
	OLED_Init();
    LED_Init();
	OLED_ShowString(1,1,"STM32F103C8 Keil");
	OLED_ShowString(2,1,"FreeRTOSv9.0.0");
	/* ��̬�������� start */
    AppTaskCreate_Handle= xTaskCreateStatic((TaskFunction_t)AppTaskCreate,
    (const char*)"AppTaskCreate",(uint32_t)128,(void*)NULL,(UBaseType_t)3,
    (StackType_t*)AppTaskCreate_Stack,
    (StaticTask_t*)&AppTaskCreate_TCB);
    if (NULL != AppTaskCreate_Handle)
    vTaskStartScheduler();
    /* ��̬�������� end */
	while(1);
}
static void LED_Task(void* parameter)
{
    while(1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(500);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		vTaskDelay(500);
    }
}
static void AppTaskCreate(void){/* ���ڴ������� */

    taskENTER_CRITICAL(); //�����ٽ���
    /* ���� LED_Task ���� */
    LED_Task_Handle = xTaskCreateStatic((TaskFunction_t)LED_Task, //������
    (const char*)"LED_Task",//��������
    (uint32_t)128, //�����ջ��С
    (void* )NULL, //���ݸ��������Ĳ���
    (UBaseType_t)4, //�������ȼ�
    (StackType_t*)LED_Task_Stack,//�����ջ
    (StaticTask_t*)&LED_Task_TCB);//������ƿ�
    if (NULL != LED_Task_Handle) /* �����ɹ� */
        OLED_ShowString(3,1,"success");
    else
        OLED_ShowString(3,1,"fail");
    
    vTaskDelete(AppTaskCreate_Handle); //ɾ�� AppTaskCreate ����
    
    taskEXIT_CRITICAL(); //�˳��ٽ���
}
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) 
{ 
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB;           /* ������ƿ��ڴ� */ 
    *ppxIdleTaskStackBuffer = Idle_Task_Stack;        /* �����ջ�ڴ� */ 
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE; /* �����ջ��С */ 
}
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) 
{ 
    *ppxTimerTaskTCBBuffer=&Timer_Task_TCB;             /* ������ƿ��ڴ� */ 
    *ppxTimerTaskStackBuffer=Timer_Task_Stack;          /* �����ջ�ڴ� */ 
    *pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* �����ջ��С */ 
}
