
/**
  ******************************************************************************
  * @file    wave_filter.c
  * @author  Shengnan Wang
  * @version V1.0.0
  * @date    2019.9.15
  * @brief   �˲�����غ���
  *****************************************************************************
  */

#include "wave_filter.h"


/**
  * @brief  ��ֵ�˲���
            �˲���Ϊ��������
            ���ݻ��м����ӳ�
  * @param  pfilter_head : �˲���ͷָ��
            pfilter_end  : �˲���βָ�� + 1
            input_data   : ���������
  * @retval �˲���õ�������
  */
TYPE_WAVE_RETURN wave_filter_mean(pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end, TYPE_WAVE input_data)
{
    TYPE_WAVE sum = 0;
    pTYPE_WAVE ptemp_left = pfilter_head;
    pTYPE_WAVE ptemp_right = pfilter_head + 1;
    char num = pfilter_end - ptemp_left;

    while(pfilter_end != ptemp_right)
    {
        * ptemp_left = * ptemp_right;
        
        sum += * ptemp_left;
        
        ptemp_left ++;
        ptemp_right ++;
    }
    sum += input_data;
    
    *ptemp_left = input_data;
    
    return sum / (float)num;
}

/**
  * @brief  ����˲���,
            �˲���Ϊ��������
            ���ݻ��м����ӳ�
  * @param  pfilter_head : �˲���ͷָ��
            pfilter_end  : �˲���βָ�� + 1
            filter       : �������ͷָ��
            input_data   : ���������
  * @retval �˲���õ�������
  */
TYPE_WAVE_RETURN wave_filter_conv(pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end, 
                                  pTYPE_WAVE filter,     TYPE_WAVE input_data)
{
    pTYPE_WAVE ptemp_left = pfilter_head;
    pTYPE_WAVE ptemp_right = pfilter_head + 1;
    TYPE_WAVE sum_conv = 0;
    TYPE_WAVE sum = 0;
    char i   = 0;
    while(pfilter_end != ptemp_right)
    {
        * ptemp_left = * ptemp_right;
        sum_conv = filter[i];
        sum += *ptemp_left * filter[i++];
        
        ptemp_left ++;
        ptemp_right ++;
    }
    sum_conv = filter[i];
    sum += input_data * filter[i];
    
    *ptemp_left = input_data;
    
    return sum / sum_conv;
}


/**
  * @brief ���� ѡ������
  * @param 
           
           
  * @retval None
  */

void wave_sort(pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end)
{
    char num = pfilter_end - pfilter_head;
    char i, j, max;
    TYPE_WAVE temp;
    for(i = 0; i < num - 1;i ++)
    {
        max = i;
        for(j = i + 1;j < num;j ++)
        {
            if (pfilter_head[j] > pfilter_head[max])
                max = j;
        }
        temp = pfilter_head[i];
        pfilter_head[i] = pfilter_head[max];
        pfilter_head[max] = temp;
    }
}


/**
  * @brief  ��ֵ�˲���
            �˲���Ϊ����
            ���ݻ��м����ӳ�
  * @param  pfilter_head : �˲���ͷָ��
            pfilter_end  : �˲���βָ�� + 1
            input_data   : ���������
  * @retval �˲���õ�������
  */

TYPE_WAVE_RETURN wave_filter_median  (pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end, TYPE_WAVE input_data)
{
    char num = pfilter_end - pfilter_head;
    pTYPE_WAVE ptemp_left = pfilter_head ;
    pTYPE_WAVE ptemp_right = pfilter_head + 1;
    TYPE_WAVE temp_a[9];
    char i;
    while(pfilter_end != ptemp_right)
    {
        * ptemp_left = * ptemp_right;
        ptemp_left ++;
        ptemp_right ++;
        
    }
    *ptemp_left = input_data;
    for(i = 0; i < num; i++)
        temp_a[i] = pfilter_head[i];
   
    wave_sort(temp_a, temp_a + num);
    
    
    return temp_a[num / 2];
    
}

/*

int ds()
{
    TYPE_WAVE a[5] = {1,2,3,4,5};
    int i;
    int j;
    int input;
    for(j = 0;j < 100; j ++)
    {
        scanf("%d", &input);
        printf("median :%f  \r\n", wave_filter_median(a, a + 5, input));   
             
        for (i = 0; i < 5 ;i ++)
            printf("%f  ", a[i]);
        putchar('\n');
        
    }
    
    return 0;
    
}
*/











