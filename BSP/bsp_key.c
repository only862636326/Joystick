/**
  ******************************************************************************
  * @file    key.c
  * @author  xjc
  * @version v1.0
  * @date    19-10-18
  * @brief   keyscan
  ******************************************************************************
  */
	
#include "bsp_key.h"

keyTypedef_t     singleKey[GPIO_KEY_NUM];
keysTypedef_t    keys;
uint32_t         key_value = 0;
uint8_t          KEY_G[16] = {0};

/*----------------------------------
             ����
----------------------------------*/
#ifdef ADVANCE
/**
* @brief key init function   ������ʼ������
* @param [in] keyRccPeriph ��APB2����ʱ��
* @param [in] keyPort��      IO�˿�
* @param [in] keyGpio��      IO�ܽ�
* @param [in] short_press :  �����̰��ص�����
* @param [in] long_press  :  ���������ص�����
* @return key structure?pointer
*/
keyTypedef_t keyInitOne(uint32_t keyRccPeriph,
                        GPIO_TypeDef * keyPort, 
                        uint32_t keyGpio, 
                        keyCallBackFuction shortPress, 
                        keyCallBackFuction longPress)
{
	static uint8_t key_total = 0; 

	keyTypedef_t singleKey;                 //�ṹ����������ڱ��水��IO�Ĳ���

	//ƽ̨����IO��
	singleKey.keyRccPeriph = keyRccPeriph;  //���水��IO�����ڵ�����ʱ������
	singleKey.keyPort = keyPort;            //���水�����ڵĶ˿�
	singleKey.keyGpio = keyGpio;            //���水�����ڵ�IO��
	singleKey.keyNum  = key_total++;        //����ÿ���������е����ı��
	

	//�ص��������壬���������Ͷ̰������Լ��Ļص�����
	singleKey.longPress  = longPress;       //���水����������ָ��
	singleKey.shortPress = shortPress;      //���水���̰�����ָ��

	keyTotolNum++;      //ȫ�־�̬������ͳ���ж��ٸ�����

	return singleKey;   //���س�ʼ���İ���IO�Ĳ����Ľṹ�����
}

/**
* ������ʼ������
* @param none
* @return none
*/
void Key_Init(void)
{
    /*�������˿ڲ������浽singleKey�ṹ�������У�
    ��������������˿ڡ�IO�˿ڡ�IO�ܽš���������ָ�롢�̰�����ָ��*/
	singleKey[0] = keyInitOne(RCC_APB2Periph_GPIOB, 
                            GPIOB, GPIO_Pin_12, 
                            key3ShortPress, key3LongPress); 
	singleKey[1] = keyInitOne(RCC_APB2Periph_GPIOA, 
                            GPIOA, GPIO_Pin_8, 
                            key2ShortPress, key2LongPress);
	keys.pSingleKey = (keyTypedef_t *)&singleKey; //pSingKeyָ��ָ��singleKey����
	keyParaInit(&keys);     //��ʼ���������ڵ�IO�ڲ���ʼ����ʱ��
}

/**
* ����3�̰��ص�����
* @param none
* @return none
*/
void key3ShortPress(void)
{
    
    
}

/**
* ����3�����ص�����
* @param none
* @return none
*/
void key3LongPress(void)
{
    
}

/**
* KEY2�̰��ص�����
* @param none
* @return none
*/
void key2ShortPress(void)
{
 
}

/**
* ����2�����ص�����
* @param none
* @return none
*/
void key2LongPress(void)
{
     
}


#endif

#ifdef NORMAL
/**
* @brief key init function   ������ʼ������
* @param [in] keyRccPeriph ��APB2����ʱ��
* @param [in] keyPort��      IO�˿�
* @param [in] keyGpio��      IO�ܽ�
* @param [in] press :  �����ص�����
* @return key structure?pointer
*/
keyTypedef_t keyInitOne(uint32_t keyRccPeriph,
                        GPIO_TypeDef * keyPort, 
                        uint32_t keyGpio, 
                        keyCallBackFuction Press)
{
	static uint8_t key_total = 0; 

	keyTypedef_t singleKey;                 //�ṹ����������ڱ��水��IO�Ĳ���

	//ƽ̨����IO��
	singleKey.keyRccPeriph = keyRccPeriph;  //���水��IO�����ڵ�����ʱ������
	singleKey.keyPort      = keyPort;       //���水�����ڵĶ˿�
	singleKey.keyGpio      = keyGpio;       //���水�����ڵ�IO��
	singleKey.keyNum       = key_total++;   //����ÿ���������е����ı��
	

	//�ص��������壬
	singleKey.Press        = Press;

	keyTotolNum++;      //ȫ�־�̬������ͳ���ж��ٸ�����

	return singleKey;   //���س�ʼ���İ���IO�Ĳ����Ľṹ�����
}

/**
* ��ֵ����
* @param  keyvalue
* @return KEY_G(ָ��)
* ��⵽�������¼�¼ ���������
*/
void Key_Pro(uint32_t keyvalue)
{
	static uint8_t i = 0;
	for(i = 0;i < 16;i++)
	{
		if(keyvalue & (0x0001 << i))
		{
			KEY_G[i] = 1;
		}
	}
}

/**
* ������ʼ������
* @param none
* @return none
*/
void Key_Init(void)
{
    /*�������˿ڲ������浽singleKey�ṹ�������У�
    ��������������˿ڡ�IO�˿ڡ�IO�ܽš�����ָ��*/
	singleKey[0] = keyInitOne(RCC_APB2Periph_GPIOB, 
                            GPIOB, GPIO_Pin_13, 
                            key1Press); 
	singleKey[1] = keyInitOne(RCC_APB2Periph_GPIOB, 
                            GPIOB, GPIO_Pin_12, 
                            key2Press);
	
	singleKey[2] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_0, 
                            key3Press);
	singleKey[3] = keyInitOne(RCC_APB2Periph_GPIOB, 
                            GPIOB, GPIO_Pin_15, 
                            key4Press);
	
	singleKey[4] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_5, 
                            key5Press);
	singleKey[5] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_6, 
                            key6Press);
	singleKey[6] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_2, 
                            key7Press);
	singleKey[7] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_3, 
                            key8Press);
														
	singleKey[8] = keyInitOne(RCC_APB2Periph_GPIOB, 
                            GPIOB, GPIO_Pin_14, 
                            key9Press);
	singleKey[9] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_7, 
                            key10Press);
	singleKey[10] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_1, 
                            key11Press);
	singleKey[11] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_4, 
                            key12Press);
		
	singleKey[12] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_8, 
                            key13Press);
	singleKey[13] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_9, 
                            key14Press);
														
	singleKey[14] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_11, 
                            key15Press);
	singleKey[15] = keyInitOne(RCC_APB2Periph_GPIOD, 
                            GPIOD, GPIO_Pin_10, 
                            key16Press);
		
	keys.pSingleKey = (keyTypedef_t *)&singleKey; //pSingKeyָ��ָ��singleKey����
	keyParaInit(&keys);                           //��ʼ���������ڵ�IO�ڲ���ʼ����ʱ��
}

/**
* ����1�ص�����
* @param none
* @return none
*/
void key1Press(void)
{
//	LED0 = 1;
	Key_Pro	(key_value);
}

/**
* ����2�ص�����
* @param none
* @return none
*/
void key2Press(void)
{
//  LED0 = 0;  
	Key_Pro	(key_value);	
}

/**
* ����3�ص�����
* @param none
* @return none
*/
void key3Press(void)
{
//	LED1 = 1;
	Key_Pro	(key_value);
}

/**
* ����4�ص�����
* @param none
* @return none
*/
void key4Press(void)
{
//	LED1 = 0;
	Key_Pro	(key_value);
}

/**
* ����5�ص�����
* @param none
* @return none
*/
void key5Press(void)
{
	Key_Pro	(key_value);
}

/**
* ����6�ص�����
* @param none
* @return none
*/
void key6Press(void)
{
	Key_Pro	(key_value);
}

/**
* ����7�ص�����
* @param none
* @return none
*/
void key7Press(void)
{
	Key_Pro	(key_value);
}

/**
* ����8�ص�����
* @param none
* @return none
*/
void key8Press(void)
{
	Key_Pro	(key_value);
}

/**
* ����9�ص�����
* @param none
* @return none
*/
void key9Press(void)
{
	Key_Pro	(key_value);
     
}

/**
* ����10�ص�����
* @param none
* @return none
*/
void key10Press(void)
{
	Key_Pro	(key_value);
//	LED0 = 1;
}

/**
* ����11�ص�����
* @param none
* @return none
*/
void key11Press(void)
{
	Key_Pro	(key_value);
}

/**
* ����12�ص�����
* @param none
* @return none
*/
void key12Press(void)
{
	Key_Pro	(key_value);
}

/**
* ����13�ص�����
* @param none
* @return none
*/
void key13Press(void)
{
	Key_Pro	(key_value);
}
/**
* ����14�ص�����
* @param none
* @return none
*/
void key14Press(void)
{
	Key_Pro	(key_value);
}

/**
* ����15�ص�����
* @param none
* @return none
*/
void key15Press(void)
{
	Key_Pro	(key_value);
}

/**
* ����16�ص�����
* @param none
* @return none
*/
void key16Press(void)
{
	Key_Pro	(key_value);
}


#endif


/*
 *@brief ����������ʼ����GPIO��ʼ������������ʱ������ⰴ��״̬
 *@param [in] pkeys:����ȫ�ֱ����ṹ�� �ں����а�������
 *@return none
*/
void keyParaInit(keysTypedef_t *pkeys)
{
	uint8_t i = 0;//�жϴ����ָ���Ƿ���ָ��
	
	if(NULL == pkeys) 
	{
		return;//���û��ָ����ֱ�ӷ��أ�������ִ��
	}
	
	pkeys -> keyTotolNum = keyTotolNum; //��ȡ��������
  /*����жϣ��������16������������ͨ���޸ĺ�KEY_MAX_NUMBER��ʵ��֧�ָ���İ���*/
	if(pkeys -> keyTotolNum > KEY_MAX_NUMBER)
	{
		 pkeys->keyTotolNum = KEY_MAX_NUMBER;
  }

  for(i = 0; i < pkeys->keyTotolNum; i++)   //ȡ�����еİ����Ĳ��������õ����±�ƫ�Ʒ�
	{
    GPIO_InitTypeDef GPIO_InitStructure;  //����ṹ�壬���ڳ�ʼ��IO����
        //ȡ��ĳ����������˿�ʱ�ӣ���ʹ����ʱ��
		RCC_APB2PeriphClockCmd(pkeys->pSingleKey[i].keyRccPeriph, ENABLE);  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //ʹ��IO���ٶ�
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;      //ʹ��IO�ڵ�ģʽ������ģʽ
		GPIO_InitStructure.GPIO_Pin   = pkeys -> pSingleKey[i].keyGpio;//ȡ��ĳ�����Ĺܽű��
		GPIO_Init(pkeys ->pSingleKey[i].keyPort,&GPIO_InitStructure);//ȡ��ĳ�����Ķ˿ںţ����а�����ʼ��
	}
	TIM2_Init();//��ʼ����ʱ��2��ÿ��1ms����һ�θ����ж�
}//��ʼ��������TIMER.c

/**
* @brief ��ȡ������״̬���������¶��ǵ͵�ƽ
* @param keys : ����ȫ�ֽṹ��ָ��Key
* @return ������״̬
*/
uint32_t getKey(keysTypedef_t *keyS)
{
  uint8_t  i = 0;
	uint32_t readKey = 0; //ʹ��һ��32λ�ı�����������16��������״ֵ̬,����������£�λ��1

	for(i = 0; i < keys.keyTotolNum; i++)  //ͨ��ѭ������ȡ���еİ���ֵ
  {
		if(!GPIO_ReadInputDataBit(
                         (GPIO_TypeDef*)keyS->pSingleKey[i].keyPort,
                         keyS->pSingleKey[i].keyGpio)
                              )
		{
			 G_SET_BIT(&readKey, keyS->pSingleKey[i].keyNum);  //��λ   
		}
		else
		{
			 G_RESET_BIT(&readKey, keyS->pSingleKey[i].keyNum);  //ȡ����λ   			
		}
	}
	return readKey;
}

/**
* @brief Read the KEY value ��ȡ���а����İ���ֵ
* @param [in] keys :Key global structure pointer
* @return GPIO status
*/
uint32_t readKeyValue(keysTypedef_t *keyS)
{
  static uint8_t keyScanFlag      = 0;
  static uint8_t keyCountTime     = 0;
  uint32_t newKeyValue    = 0;
	
#ifdef ADVANCE
  static uint8_t keyState         = 0;
  static uint32_t keyLongPressCnt = 0;
  static uint32_t lastKeyValue    = 0;      //last key
  uint32_t keyReturnValue = 0;
#endif
	
	keyCountTime ++;     //ÿ1ms��1
	
	if(keyCountTime >= (SCAN_TIME / KEY_TIMER_MS))     //�����������Ϊ��5msɨ��һ�ΰ������ı���ֵ����ʵ�������ư���ɨ��Ƶ��
	 {
    keyCountTime = 0;
    keyScanFlag = 1;     //��־λ��1����Ҫ����һ�ΰ���ɨ�裬ʹ�ñ�־λ�����ư�����ɨ������
	 }
	  if(1 == keyScanFlag)   
  {
    keyScanFlag = 0;            //����ɨ���־λ����
    newKeyValue = getKey(keyS); //��ȡ������ֵ������Ҫ����λ���жϲ��ܻ�֪���µİ������ĸ�
#ifdef ADVANCE
   switch (keyState)	
		{
    case 0:
      if(newKeyValue != 0)        //������ֵ��Ϊ0�����ʾ�а�������
			{
        lastKeyValue = newKeyValue;  //���水�µ�ֵ
        keyState = 1;        //�л�����һ��case
      }
			break;
			
		case 1:
			if(newKeyValue == lastKeyValue) //��������������£���newKeyPressValֵ���ϴ�һ����
      {
        keyState = 2;        //�л�����һ��case
				keyReturnValue= lastKeyValue | KEY_PRESS_DOWN;  //��λ����ʾ�а���������
      }
			else                     //�����ɿ����߰��������������κε���Ӧ���ֻص�case��
      {
        keyState = 0;
      }
			break;
		
		case 2:
			if(newKeyValue != lastKeyValue)   //�����ɿ���
      {
        keyState = 0;         //�ص�case 0
				keyLongPressCnt = 0;     //����
        keyReturnValue = lastKeyValue | KEY_SHORT_PRESS;       //�������£����ɿ���
				return keyReturnValue;
			}
			if(newKeyValue == lastKeyValue)  //���������Ȼ���£���������ֵ�������
      {
				keyLongPressCnt++;      //ͳ�Ƴ������µĴ���
        if(keyLongPressCnt >= (PRESS_LONG_TIME / SCAN_TIME))  //����3����Ч
        {
					keyLongPressCnt = 0;//����ͳ�ƴ�����Ҫ��������
          keyState = 3;    //�л���case3
          keyReturnValue= newKeyValue | KEY_LONG_PRESS; //��������
					return keyReturnValue;   //���ذ���ֵ
        }
      }
			break;

		case 3:
			if(newKeyValue != lastKeyValue)  //����������������ɿ��ˣ������㣬�ٻص�case 0
      {
				keyState = 0;
      }
			break;
    }
	}
  return  NO_KEY_PRESS_DOWN;
#endif

#ifdef NORMAL
	}
	return newKeyValue;
#endif
}

/**
* @brief ��ⰴ���ǳ������Ƕ̰�����ִ����Ӧ�Ļص�����
* @param [in] keys : ȫ�ְ��������ṹ��
* @return none
*/
void keyHandle(keysTypedef_t *pkeyS)    //��ʱ��������ÿ��10msִ��һ��
{
  uint8_t i = 0;

  key_value = readKeyValue(pkeyS);    //��ȡ������ֵ			
				
	if(!key_value) 
	{
		return; //�������ֵΪ0���򷵻أ�������ִ��
	}

#ifdef ADVANCE
  /*������ĸ������̰�*/			
	 if(key_value & KEY_SHORT_PRESS)
  {
    for(i = 0; i < pkeyS->keyTotolNum; i++)  //ͨ��ѭ�����ж����еİ���
		{
      if(G_IS_BIT_SET(key_value, pkeyS->pSingleKey[i].keyNum))   //�ж��ǲ�����λ��
			{
        if(pkeyS->pSingleKey[i].shortPress)     //����ж̰��ص�����
        {
					pkeyS->pSingleKey[i].shortPress();  //ִ�ж̰��ص�����
				}
			}
		}
	}

	/*������ĸ���������*/
  if(key_value & KEY_LONG_PRESS)
  {
		for(i = 0; i < pkeyS->keyTotolNum; i++)  //ѭ���ж����а���
    {
      if(G_IS_BIT_SET(key_value, pkeyS->pSingleKey[i].keyNum))  //�ǲ�����λ��
			{
        if(pkeyS->pSingleKey[i].longPress)    //����а��������ص�����
        {	
					 pkeyS->pSingleKey[i].longPress(); //ִ�а��������ص�����
        }
			}
		}
	}
#endif

#ifdef NORMAL
  for(i = 0; i < pkeyS->keyTotolNum; i++)  //ͨ��ѭ�����ж����еİ���
	{
    if(G_IS_BIT_SET(key_value, pkeyS->pSingleKey[i].keyNum))  //�ǲ�����λ��
		{
			if(pkeyS->pSingleKey[i].Press)    //����а����ص�����
			{
				pkeyS->pSingleKey[i].Press(); //ִ�а����ص�����
			}
		}
	}
#endif
}


/**
* @brief ȷ��key_value��keyNumֵ�Ƿ�Ϊ��
* @param [in] keyNum   :�������
							key_value:ԭʼ��������
* @return ��λ��ɵ�readKey
*/
uint8_t G_IS_BIT_SET(uint32_t key_value, uint8_t keyNum)
{
	if (key_value & (0x0001<<keyNum))
	{
		return 1;
	}
	return 0;
}

/**
* @brief ��readKey�ĵ�keyNumλ��1
* @param [in] keyNum :�������
							readKey:ԭʼ��������
* @return ��λ��ɵ�readKey
*/
void G_SET_BIT(uint32_t *readKey, uint8_t keyNum)   
{
	*readKey = *readKey | (0x0001<<keyNum);
	
}
				
/**
* @brief ��readKey�ĵ�keyNumλ��0
* @param [in] keyNum :�������
							readKey:ԭʼ��������
* @return ��λ��ɵ�readKey
*/
void G_RESET_BIT(uint32_t *readKey, uint8_t keyNum)   
{				
	*readKey = *readKey & ((0xfffe<<keyNum)|(0xffff>>(16-keyNum)));
	
}
