#ifndef _PACK_DEAL_H_
#define _PACK_DEAL_H_

#include "stm32f10x.h"
#define FRAME_NONE     0x00

typedef struct TYPE_Pack_Info_t
{
    uint8_t data_size;         /*数据字节大小 不含帧头帧尾    */
    uint8_t frame_head1;       /*帧头1                        */
    uint8_t frame_head2;       /*帧头2 不存在设为 FRAME_NONE  */
    uint8_t frame_end1;        /*帧尾1                        */
    uint8_t frame_end2;        /*帧尾2 不存在设为 FRAME_NONE  */
    uint8_t* pack;             /*数据包缓存指针               */
    //无需初始化
    uint8_t i;                  /*数据接收 发送  计数          */
    uint8_t count;              /*数据接收  或  数据包总大小    */
}TYPE_Pack_Info_t, *pTYPE_Pack_Info_t;


uint8_t Pack_Deal_API_Read_Pack  (pTYPE_Pack_Info_t pPackInfo_t, uint8_t ch);
void    Pack_Deal_API_Data_Pack  (pTYPE_Pack_Info_t pPackInfo_t, uint8_t* p_t);

void    Pack_Deal_API_SetPackInfo(pTYPE_Pack_Info_t pPackInfo_t, uint8_t data_size,
                                  uint8_t frame_head1, uint8_t frame_head2,
                                  uint8_t frame_end1,  uint8_t frame_end2,
                                  uint8_t* buff);

void    Pack_Deal_API_Bit_Copy(uint8_t *pNew, uint8_t *pOld, uint8_t dataSize);

uint8_t Online_State_Deal(uint8_t oldSta, uint8_t frame_num, uint8_t min_frame);


#endif





















