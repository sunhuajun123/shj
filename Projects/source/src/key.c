#include "key.h"
#include "ws2812_app.h"

key_struct__ key_struct={0};

/****************************************************************************** 
* Function Name    	: KeyGpio_Init
* Modify/Creat time	: 2024/08/02
* Modified/Creat By	: shj
* Description    	: device key gpio init
* Input Param    	: void   
* Output Param    	: void
*******************************************************************************/ 
static void KeyGpio_Init(void)
{
    GPIO_InitTypeDef Struct_GPIOInit;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
    
//    GPIO_DeInit(KEY_PORT);
    Struct_GPIOInit.GPIO_Pin = KEY_GPIO_PIN;
    Struct_GPIOInit.GPIO_Mode = GPIO_Mode_IN;
    //Struct_GPIOInit.GPIO_PuPd = GPIO_PuPd_DOWN;
    Struct_GPIOInit.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_Init(KEY_PORT,&Struct_GPIOInit);
}

/****************************************************************************** 
* Function Name    	: CheckKey
* Modify/Creat time	: 2024/08/02
* Modified/Creat By	: shj
* Description    	: device key gpio input state check
* Input Param    	: void   
* Output Param    	: void
*******************************************************************************/ 
static uint16_t CheckKey(void)
{
    uint8_t key_val=0;
    if (GET_KEY_STA())          //��ȡ����GPIO��ƽ״̬
    {
	key_val=1;              //������ƽ��Ч
    }
    return key_val;             //���ذ���״̬
}

/****************************************************************************** 
* Function Name    	: KeyState_Process
* Modify/Creat time	: 2024/08/02
* Modified/Creat By	: shj
* Description    	: deal with the key gpio input state information
* Input Param    	: void   
* Output Param    	: void
*******************************************************************************/ 
static void KeyState_Process(key_struct__* key_param)
{
    if (key_param->keysta_current)                                      //��ǰ������Ϣ��Ч����
    {
        if (key_param->keysta_previous != key_param->keysta_current)    //�Ա�֮ǰ�İ���״̬
        {
            key_param->keysta_previous = key_param->keysta_current;     //���浱ǰ����״̬
        }
        if (key_param->time_cnt < (0xffff - \
          (KEY_PROCESS_STEP * KEY_PROCESS_CYCLE)))                      //������Чʱ���ж�
        {
            key_param->time_cnt += KEY_PROCESS_STEP * KEY_PROCESS_CYCLE;//���㰴����Чʱ��
        }
    }
    else                                                                //�����ɿ�״̬�߼�����
    {
        if (key_param->keysta_previous)                                 //��⵽֮ǰ�İ�����Ч
        {
            key_param->keysta_previous = 0;                             //���֮ǰ����״̬
            if (key_param->time_cnt < 1000 && key_param->time_cnt >100) //��������ʱ����200~1000ms
            {
                key_param->key_longshort_chk = 1;                       //��־�����̰�
            }
            else if (key_param->time_cnt > 2000)                        //��������ʱ�䳬��2s
            { 
                key_param->key_longshort_chk = 2;                       //��־��������
            }
            else                                                        //����/�����
            {
                key_param->key_longshort_chk = 0;                       //û�а�������
            }
            key_param->time_cnt = 0;                                    //�����������ʱ��
        }
    }
}

/****************************************************************************** 
* Function Name    	: KeyState_Check
* Modify/Creat time	: 2024/08/02
* Modified/Creat By	: shj
* Description    	: check the key state ,up or down, and process it 
* Input Param    	: void   
* Output Param    	: void
*******************************************************************************/ 
static void KeyState_Check(key_struct__* key_param)
{
    uint16_t temp;
    #define CNT_TIMES 10
    
    if (key_param->sample_cnt < CNT_TIMES)              //������Ϣ�˲���ʱ��10*ʱ����
    {		
	temp = CheckKey();                              //��ȡ����״̬
	if (temp)                                       //��⵽��������
	{
            key_param->key_filter_cnt++;                //�������´����ۻ�
	}
	key_param->sample_cnt++;                        //���������ۻ�
    }
    else                                                //�˲��հ������ݴ���
    {
	if (key_param->key_filter_cnt >= CNT_TIMES/3)   //��Ч�����ж�
	{
            key_param->keysta_current = 1;		//������Ч	
	}
        else
        {
            key_param->keysta_current = 0;
        }
	key_param->key_filter_cnt = 0;	                //�˲���������
	key_param->key_process_flg = 1;                 //������Ϣ�����־
        key_param->sample_cnt = 0;
    }
    
    if (key_param->key_process_flg)                     //������Ϣ����ӿ�
    {
        key_param->key_process_flg = 0;                 //������Ϣ���������������־
        KeyState_Process(key_param);                    //��������Ϣ
    }
}

/****************************************************************************** 
* Function Name    	: KeyInfo_Process
* Modify/Creat time	: 2024/08/02
* Modified/Creat By	: shj
* Description    	: key state function remap
* Input Param    	: void   
* Output Param    	: void
*******************************************************************************/ 
static void KeyInfo_Process(key_struct__* key_param)
{
    switch (key_param->key_longshort_chk)
    {
    case IDLE:
      
      break;;
    case SHORT_PRESS:
      key_param->key_longshort_chk = IDLE;
      if (Front_RGB.light_set_stl < 9)
      {
        Front_RGB.light_set_stl++;
      }
      else
      {
        Front_RGB.light_set_stl = 0;
      }
      break;
    case LONG_PRESS:
      
      break;
    default:break;
    }
}

/****************************************************************************** 
* Function Name    	: HardwareInit_Key
* Modify/Creat time	: 2024/08/02
* Modified/Creat By	: shj
* Description    	: key hardware init
* Input Param    	: void   
* Output Param    	: void
*******************************************************************************/ 
void HardwareInit_Key(void)
{
    KeyGpio_Init();
}

/****************************************************************************** 
* Function Name    	: task_Key
* Modify/Creat time	: 2024/08/02
* Modified/Creat By	: shj
* Description    	: key task
* Input Param    	: void   
* Output Param    	: void
*******************************************************************************/ 
void task_Key(void)
{
    KeyState_Check(&key_struct);
    KeyInfo_Process(&key_struct);
}






