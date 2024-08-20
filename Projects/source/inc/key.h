#ifndef __KEY_H__
#define __KEY_H__
#include "stm32f0xx.h"

typedef enum
{
    IDLE=0,
    SHORT_PRESS,
    LONG_PRESS
}enum_KeySta_;

#define KEY_PORT GPIOA
#define KEY_GPIO_PIN GPIO_Pin_5

#define GET_KEY_STA() GPIO_ReadInputDataBit(KEY_PORT,KEY_GPIO_PIN)

#define KEY_PROCESS_STEP 1      //��������ִ�д�������
#define KEY_PROCESS_CYCLE 10    //��������ִ�����ڣ���λ:ms

typedef struct
{
    uint8_t sample_cnt;                 //�ܲ������� 
    uint8_t key_filter_cnt;             //��Ч��������
    uint8_t keysta_current;             //��ǰ����״̬
    uint8_t keysta_previous;            //�ϴΰ���״̬	
    uint8_t key_longshort_chk;
    uint8_t key_process_flg;            //�����źŴ����־��1��ʹ���жϣ�0���ȴ��ж�
    uint8_t shortpress_cnt;             //�����̰�����
    uint16_t time_cnt;
}key_struct__;
extern key_struct__ key_struct;

extern void HardwareInit_Key(void);
extern void task_Key(void);
#endif
