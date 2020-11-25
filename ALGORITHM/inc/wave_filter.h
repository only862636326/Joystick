#ifndef __WAVE_FILTER_H__
#define __WAVE_FILTER_H__

/*
#include <stdio.h>
#include <stdlib.h>
*/
#include <sys.h>

typedef int16_t TYPE_WAVE, *pTYPE_WAVE;
typedef int16_t TYPE_WAVE_RETURN;




// 波选择排序
void wave_sort(pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end);


/*
    传入一个值,返回滤波后的值
    并把得到的值保存在滤波器头
    
    滤波器大小一般为奇数
    
*/

// 均值滤波
TYPE_WAVE_RETURN wave_filter_mean  (pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end, TYPE_WAVE input_data);
// 卷积滤波
TYPE_WAVE_RETURN wave_filter_conv(pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end, 
                                  pTYPE_WAVE filter,     TYPE_WAVE input_data);
// 中值滤波
TYPE_WAVE_RETURN wave_filter_median(pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end, TYPE_WAVE input_data);




#endif

