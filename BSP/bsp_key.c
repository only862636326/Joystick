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
             函数
----------------------------------*/
#ifdef ADVANCE
/**
* @brief key init function   按键初始化函数
* @param [in] keyRccPeriph ：APB2外设时钟
* @param [in] keyPort：      IO端口
* @param [in] keyGpio：      IO管脚
* @param [in] short_press :  按键短按回调函数
* @param [in] long_press  :  按键长按回到函数
* @return key structure?pointer
*/
keyTypedef_t keyInitOne(uint32_t keyRccPeriph,
                        GPIO_TypeDef * keyPort, 
                        uint32_t keyGpio, 
                        keyCallBackFuction shortPress, 
                        keyCallBackFuction longPress)
{
	static uint8_t key_total = 0; 

	keyTypedef_t singleKey;                 //结构体变量，用于保存按键IO的参数

	//平台定义IO口
	singleKey.keyRccPeriph = keyRccPeriph;  //保存按键IO口所在的外设时钟总线
	singleKey.keyPort = keyPort;            //保存按键所在的端口
	singleKey.keyGpio = keyGpio;            //保存按键所在的IO口
	singleKey.keyNum  = key_total++;        //保存每个按键都有单独的编号
	

	//回调函数定义，按键长按和短按各有自己的回调函数
	singleKey.longPress  = longPress;       //保存按键长按函数指针
	singleKey.shortPress = shortPress;      //保存按键短按函数指针

	keyTotolNum++;      //全局静态变量，统计有多少个按键

	return singleKey;   //返回初始化的按键IO的参数的结构体变量
}

/**
* 按键初始化函数
* @param none
* @return none
*/
void Key_Init(void)
{
    /*将按键端口参数保存到singleKey结构体数组中，
    参数包括：外设端口、IO端口、IO管脚、长按函数指针、短按函数指针*/
	singleKey[0] = keyInitOne(RCC_APB2Periph_GPIOB, 
                            GPIOB, GPIO_Pin_12, 
                            key3ShortPress, key3LongPress); 
	singleKey[1] = keyInitOne(RCC_APB2Periph_GPIOA, 
                            GPIOA, GPIO_Pin_8, 
                            key2ShortPress, key2LongPress);
	keys.pSingleKey = (keyTypedef_t *)&singleKey; //pSingKey指针指向singleKey数组
	keyParaInit(&keys);     //初始化按键所在的IO口并初始化定时器
}

/**
* 按键3短按回调函数
* @param none
* @return none
*/
void key3ShortPress(void)
{
    
    
}

/**
* 按键3长按回调函数
* @param none
* @return none
*/
void key3LongPress(void)
{
    
}

/**
* KEY2短按回调函数
* @param none
* @return none
*/
void key2ShortPress(void)
{
 
}

/**
* 按键2长按回调函数
* @param none
* @return none
*/
void key2LongPress(void)
{
     
}


#endif

#ifdef NORMAL
/**
* @brief key init function   按键初始化函数
* @param [in] keyRccPeriph ：APB2外设时钟
* @param [in] keyPort：      IO端口
* @param [in] keyGpio：      IO管脚
* @param [in] press :  按键回调函数
* @return key structure?pointer
*/
keyTypedef_t keyInitOne(uint32_t keyRccPeriph,
                        GPIO_TypeDef * keyPort, 
                        uint32_t keyGpio, 
                        keyCallBackFuction Press)
{
	static uint8_t key_total = 0; 

	keyTypedef_t singleKey;                 //结构体变量，用于保存按键IO的参数

	//平台定义IO口
	singleKey.keyRccPeriph = keyRccPeriph;  //保存按键IO口所在的外设时钟总线
	singleKey.keyPort      = keyPort;       //保存按键所在的端口
	singleKey.keyGpio      = keyGpio;       //保存按键所在的IO口
	singleKey.keyNum       = key_total++;   //保存每个按键都有单独的编号
	

	//回调函数定义，
	singleKey.Press        = Press;

	keyTotolNum++;      //全局静态变量，统计有多少个按键

	return singleKey;   //返回初始化的按键IO的参数的结构体变量
}

/**
* 键值处理
* @param  keyvalue
* @return KEY_G(指针)
* 检测到按键按下记录 发送完清空
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
* 按键初始化函数
* @param none
* @return none
*/
void Key_Init(void)
{
    /*将按键端口参数保存到singleKey结构体数组中，
    参数包括：外设端口、IO端口、IO管脚、函数指针*/
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
		
	keys.pSingleKey = (keyTypedef_t *)&singleKey; //pSingKey指针指向singleKey数组
	keyParaInit(&keys);                           //初始化按键所在的IO口并初始化定时器
}

/**
* 按键1回调函数
* @param none
* @return none
*/
void key1Press(void)
{
//	LED0 = 1;
	Key_Pro	(key_value);
}

/**
* 按键2回调函数
* @param none
* @return none
*/
void key2Press(void)
{
//  LED0 = 0;  
	Key_Pro	(key_value);	
}

/**
* 按键3回调函数
* @param none
* @return none
*/
void key3Press(void)
{
//	LED1 = 1;
	Key_Pro	(key_value);
}

/**
* 按键4回调函数
* @param none
* @return none
*/
void key4Press(void)
{
//	LED1 = 0;
	Key_Pro	(key_value);
}

/**
* 按键5回调函数
* @param none
* @return none
*/
void key5Press(void)
{
	Key_Pro	(key_value);
}

/**
* 按键6回调函数
* @param none
* @return none
*/
void key6Press(void)
{
	Key_Pro	(key_value);
}

/**
* 按键7回调函数
* @param none
* @return none
*/
void key7Press(void)
{
	Key_Pro	(key_value);
}

/**
* 按键8回调函数
* @param none
* @return none
*/
void key8Press(void)
{
	Key_Pro	(key_value);
}

/**
* 按键9回调函数
* @param none
* @return none
*/
void key9Press(void)
{
	Key_Pro	(key_value);
     
}

/**
* 按键10回调函数
* @param none
* @return none
*/
void key10Press(void)
{
	Key_Pro	(key_value);
//	LED0 = 1;
}

/**
* 按键11回调函数
* @param none
* @return none
*/
void key11Press(void)
{
	Key_Pro	(key_value);
}

/**
* 按键12回调函数
* @param none
* @return none
*/
void key12Press(void)
{
	Key_Pro	(key_value);
}

/**
* 按键13回调函数
* @param none
* @return none
*/
void key13Press(void)
{
	Key_Pro	(key_value);
}
/**
* 按键14回调函数
* @param none
* @return none
*/
void key14Press(void)
{
	Key_Pro	(key_value);
}

/**
* 按键15回调函数
* @param none
* @return none
*/
void key15Press(void)
{
	Key_Pro	(key_value);
}

/**
* 按键16回调函数
* @param none
* @return none
*/
void key16Press(void)
{
	Key_Pro	(key_value);
}


#endif


/*
 *@brief 按键参数初始化：GPIO初始化，并启动定时器来检测按键状态
 *@param [in] pkeys:按键全局变量结构体 内含所有按键参数
 *@return none
*/
void keyParaInit(keysTypedef_t *pkeys)
{
	uint8_t i = 0;//判断传入的指针是否有指向
	
	if(NULL == pkeys) 
	{
		return;//如果没有指向，则直接返回，不往下执行
	}
	
	pkeys -> keyTotolNum = keyTotolNum; //获取按键总数
  /*误差判断，限制最多16个按键，可以通过修改宏KEY_MAX_NUMBER来实现支持更多的按键*/
	if(pkeys -> keyTotolNum > KEY_MAX_NUMBER)
	{
		 pkeys->keyTotolNum = KEY_MAX_NUMBER;
  }

  for(i = 0; i < pkeys->keyTotolNum; i++)   //取出所有的按键的参数，采用的是下标偏移法
	{
    GPIO_InitTypeDef GPIO_InitStructure;  //定义结构体，用于初始化IO参数
        //取出某按键的外设端口时钟，并使能其时钟
		RCC_APB2PeriphClockCmd(pkeys->pSingleKey[i].keyRccPeriph, ENABLE);  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //使能IO口速度
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;      //使能IO口的模式：上拉模式
		GPIO_InitStructure.GPIO_Pin   = pkeys -> pSingleKey[i].keyGpio;//取出某按键的管脚编号
		GPIO_Init(pkeys ->pSingleKey[i].keyPort,&GPIO_InitStructure);//取出某按键的端口号，进行按键初始化
	}
	TIM2_Init();//初始化定时器2，每隔1ms进入一次更新中断
}//初始化函数在TIMER.c

/**
* @brief 读取按键的状态，按键按下都是低电平
* @param keys : 按键全局结构体指针Key
* @return 按键的状态
*/
uint32_t getKey(keysTypedef_t *keyS)
{
  uint8_t  i = 0;
	uint32_t readKey = 0; //使用一个32位的变量用来保存16个按键的状态值,如果按键按下，位置1

	for(i = 0; i < keys.keyTotolNum; i++)  //通过循环来读取所有的按键值
  {
		if(!GPIO_ReadInputDataBit(
                         (GPIO_TypeDef*)keyS->pSingleKey[i].keyPort,
                         keyS->pSingleKey[i].keyGpio)
                              )
		{
			 G_SET_BIT(&readKey, keyS->pSingleKey[i].keyNum);  //置位   
		}
		else
		{
			 G_RESET_BIT(&readKey, keyS->pSingleKey[i].keyNum);  //取消置位   			
		}
	}
	return readKey;
}

/**
* @brief Read the KEY value 读取所有按键的按键值
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
	
	keyCountTime ++;     //每1ms加1
	
	if(keyCountTime >= (SCAN_TIME / KEY_TIMER_MS))     //该条件语句是为了5ms扫描一次按键，改变宏的值可以实现灵活控制按键扫描频率
	 {
    keyCountTime = 0;
    keyScanFlag = 1;     //标志位置1，需要进行一次按键扫描，使用标志位来控制按键的扫描速率
	 }
	  if(1 == keyScanFlag)   
  {
    keyScanFlag = 0;            //按键扫描标志位清零
    newKeyValue = getKey(keyS); //读取按键的值，还需要进行位域判断才能获知按下的按键是哪个
#ifdef ADVANCE
   switch (keyState)	
		{
    case 0:
      if(newKeyValue != 0)        //如果这个值不为0，则表示有按键按下
			{
        lastKeyValue = newKeyValue;  //保存按下的值
        keyState = 1;        //切换到下一个case
      }
			break;
			
		case 1:
			if(newKeyValue == lastKeyValue) //如果按键持续按下，则newKeyPressVal值和上次一样，
      {
        keyState = 2;        //切换到下一个case
				keyReturnValue= lastKeyValue | KEY_PRESS_DOWN;  //置位，表示有按键按下了
      }
			else                     //按键松开或者按键抖动，不做任何的响应，又回到case零
      {
        keyState = 0;
      }
			break;
		
		case 2:
			if(newKeyValue != lastKeyValue)   //按键松开了
      {
        keyState = 0;         //回到case 0
				keyLongPressCnt = 0;     //清零
        keyReturnValue = lastKeyValue | KEY_SHORT_PRESS;       //按键按下，又松开了
				return keyReturnValue;
			}
			if(newKeyValue == lastKeyValue)  //如果按键仍然按下，则这两个值继续相等
      {
				keyLongPressCnt++;      //统计长按按下的次数
        if(keyLongPressCnt >= (PRESS_LONG_TIME / SCAN_TIME))  //长按3秒有效
        {
					keyLongPressCnt = 0;//按键统计次数需要进行清零
          keyState = 3;    //切换到case3
          keyReturnValue= newKeyValue | KEY_LONG_PRESS; //按键长按
					return keyReturnValue;   //返回按键值
        }
      }
			break;

		case 3:
			if(newKeyValue != lastKeyValue)  //长按过后，如果按键松开了，则清零，再回到case 0
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
* @brief 检测按键是长按还是短按，并执行相应的回调函数
* @param [in] keys : 全局按键参数结构体
* @return none
*/
void keyHandle(keysTypedef_t *pkeyS)    //定时处理函数，每个10ms执行一次
{
  uint8_t i = 0;

  key_value = readKeyValue(pkeyS);    //读取按键的值			
				
	if(!key_value) 
	{
		return; //如果按键值为0，则返回，不往下执行
	}

#ifdef ADVANCE
  /*检测是哪个按键短按*/			
	 if(key_value & KEY_SHORT_PRESS)
  {
    for(i = 0; i < pkeyS->keyTotolNum; i++)  //通过循环来判断所有的按键
		{
      if(G_IS_BIT_SET(key_value, pkeyS->pSingleKey[i].keyNum))   //判断是不是置位了
			{
        if(pkeyS->pSingleKey[i].shortPress)     //如果有短按回调函数
        {
					pkeyS->pSingleKey[i].shortPress();  //执行短按回调函数
				}
			}
		}
	}

	/*检测是哪个按键长按*/
  if(key_value & KEY_LONG_PRESS)
  {
		for(i = 0; i < pkeyS->keyTotolNum; i++)  //循环判断所有按键
    {
      if(G_IS_BIT_SET(key_value, pkeyS->pSingleKey[i].keyNum))  //是不是置位了
			{
        if(pkeyS->pSingleKey[i].longPress)    //如果有按键长按回调函数
        {	
					 pkeyS->pSingleKey[i].longPress(); //执行按键长按回调函数
        }
			}
		}
	}
#endif

#ifdef NORMAL
  for(i = 0; i < pkeyS->keyTotolNum; i++)  //通过循环来判断所有的按键
	{
    if(G_IS_BIT_SET(key_value, pkeyS->pSingleKey[i].keyNum))  //是不是置位了
		{
			if(pkeyS->pSingleKey[i].Press)    //如果有按键回调函数
			{
				pkeyS->pSingleKey[i].Press(); //执行按键回调函数
			}
		}
	}
#endif
}


/**
* @brief 确认key_value的keyNum值是否为空
* @param [in] keyNum   :按键序号
							key_value:原始输入数据
* @return 置位完成的readKey
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
* @brief 给readKey的第keyNum位置1
* @param [in] keyNum :按键序号
							readKey:原始输入数据
* @return 置位完成的readKey
*/
void G_SET_BIT(uint32_t *readKey, uint8_t keyNum)   
{
	*readKey = *readKey | (0x0001<<keyNum);
	
}
				
/**
* @brief 给readKey的第keyNum位置0
* @param [in] keyNum :按键序号
							readKey:原始输入数据
* @return 置位完成的readKey
*/
void G_RESET_BIT(uint32_t *readKey, uint8_t keyNum)   
{				
	*readKey = *readKey & ((0xfffe<<keyNum)|(0xffff>>(16-keyNum)));
	
}
