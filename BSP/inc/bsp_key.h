#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "drv_systick.h"
#include "drv_timer.h"
#include "bsp_led.h"
#include "stdio.h"
  
//#define ADVANCE 0x00
#define NORMAL  0x00
/*----------------------------------
             变量
----------------------------------*/
typedef void (*keyCallBackFuction)(void);

#ifdef ADVANCE
__packed typedef struct
{
	uint8_t            keyNum;         //按键编号
	uint32_t           keyRccPeriph;   //按键时钟
	GPIO_TypeDef       *keyPort;       //按键所在端口
	uint32_t           keyGpio;        //按键所在的IO口
	keyCallBackFuction shortPress;     //回调函数成员，短按回调函数
	keyCallBackFuction longPress;      //回调函数成员，长按回调函数
}keyTypedef_t;
#endif

#ifdef NORMAL
__packed typedef struct
{
	uint8_t            keyNum;         //按键编号
	uint32_t           keyRccPeriph;   //按键时钟
	GPIO_TypeDef       *keyPort;       //按键所在端口
	uint32_t           keyGpio;        //按键所在的IO口
	keyCallBackFuction Press;          //回调函数成员
}keyTypedef_t;
#endif

__packed typedef struct
{
	uint8_t      keyTotolNum;          //按键总数
	keyTypedef_t *pSingleKey;          //该指针单个按键的结构参数
}keysTypedef_t;


#define GPIO_KEY_NUM           16    //定义按键总数
#define KEY_MAX_NUMBER         16    //定义按键最大数
#define KEY_TIMER_MS           1
#define SCAN_TIME              5
#define PRESS_LONG_TIME        3000
	 
static uint8_t       keyTotolNum = 0;        //有与统计有多少个按键

extern keyTypedef_t     singleKey[GPIO_KEY_NUM];
extern keysTypedef_t    keys;

#ifdef NORMAL
extern uint32_t                key_value;
extern uint8_t                 KEY_G[16];
#define KEY_PRESS_DOWN         0x10
#define	NO_KEY_PRESS_DOWN      0xffff
/*
	0~15位为按键键值分为0/1
		其他为为0
*/
#endif

#ifdef ADVANCE
entern uint8_t           key_value[16];
/*
	   0位为按键键值分为0/1
	   3位为按下标志位 为0表示无按下 1表示有按下
		 4位为短按标志位
		 5位为长按标志位
		其他为为0
*/
#endif
/*
	丝印
	KEY1           左摇杆   K3              B13
	KEY2           右摇杆   K2              B12
	KEY3           左下独立 K17             D0
	KEY4           右下独立 K11             B15
	KEY5  6  7  8  左边十字 K13 19 12 18    D5 D6 D2 D3
	KEY9  10 11 12 右边十字 K5  8  6  7     B14 D7 D1 D4 
	KEY13 14       左下自锁 K10 14          D8 D9
	KEY15 16       右下自锁 K20 16          D11 D10
*/ 

#ifdef ADVANCE
keyTypedef_t keyInitOne(uint32_t keyRccPeriph,
                        GPIO_TypeDef * keyPort, 
                        uint32_t keyGpio, 
                        keyCallBackFuction shortPress, 
                        keyCallBackFuction longPress);
void key3ShortPress(void);
void key3LongPress(void);
void key2ShortPress(void);
void key2LongPress(void);
#endif

#ifdef NORMAL
keyTypedef_t keyInitOne(uint32_t keyRccPeriph,
                        GPIO_TypeDef * keyPort, 
                        uint32_t keyGpio, 
                        keyCallBackFuction Press);
void key1Press(void);
void key2Press(void);
void key3Press(void);
void key4Press(void);
void key5Press(void);
void key6Press(void);
void key7Press(void);
void key8Press(void);
void key9Press(void);
void key10Press(void);
void key11Press(void);
void key12Press(void);
void key13Press(void);
void key14Press(void);
void key15Press(void);
void key16Press(void);
#endif
void Key_Init(void);
void keyParaInit(keysTypedef_t *pkeys);
uint32_t getKey(keysTypedef_t *keyS);
uint32_t readKeyValue(keysTypedef_t *keyS);
void keyHandle(keysTypedef_t *pkeyS);
uint8_t G_IS_BIT_SET(uint32_t key_value, uint8_t keyNum);
void G_SET_BIT(uint32_t *readKey, uint8_t keyNum);    
void G_RESET_BIT(uint32_t *readKey, uint8_t keyNum);    
void Key_Pro(uint32_t keyvalue);
												

#endif
