/*
 * Copyright (c) 2022 FuZhou Lockzhiner Electronic Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include "ohos_init.h"
#include "los_task.h"
#include "e53_intelligent_street_lamp.h"
#include "e53_gesture_sensor.h"

/***************************************************************
* 函数名称: e53_isl_thread
* 说    明: E53智慧路灯线程
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void e53_isl_thread()
{
    float lum = 0;

    e53_isl_init();

    while (1)
    {
        lum = e53_isl_read_data();

        printf("luminance value is %.2f\n", lum);

        if (lum < 20)
        {
            isl_light_set_status(ON);
            printf("light on\n");
        }
        else
        {
            isl_light_set_status(OFF);
            printf("light off\n");
        }

        LOS_Msleep(2000);
    }
}


void e53_gs_process(void *arg)
{
    unsigned int ret = 0;
    unsigned short flag = 0;
    LzGpioValue value = LZGPIO_LEVEL_LOW;

    float lum = 0;

    e53_isl_init();
    
    e53_gs_init();

    LzGpioInit(GPIO0_PA2);
    PinctrlSet(GPIO0_PA2, MUX_FUNC0, PULL_KEEP, DRIVE_LEVEL0);
    LzGpioSetDir(GPIO0_PA2, LZGPIO_DIR_OUT);

    
    while (1)
    {

        

        LOS_Msleep(2000);

        ret = e53_gs_get_gesture_state(&flag);
        if (ret != 0)
        {
            printf("Get Gesture Statu: 0x%x\n", flag);
            if (flag & GES_UP)
            {
                printf("\tUp\n");
                lum = e53_isl_read_data();

                printf("luminance value is %.2f\n", lum);

                if (lum < 60)
                {
                    isl_light_set_status(ON);
                    printf("light on\n");
                }
                else
                {
                    isl_light_set_status(OFF);
                    printf("light off\n");
                }

            }
            if (flag & GES_DOWM)
            {
                printf("\tDown\n");
                
                isl_light_set_status(OFF);
                printf("light off\n");
            }
            if (flag & GES_LEFT)
            {
                printf("\tLeft\n");

                // 检测到Left状态时，将GPIO0_PA2设置为LOW

                LzGpioSetVal(GPIO0_PA2, 0);
                LzGpioGetVal(GPIO0_PA2, &value);
                printf("\tgpio set %d => gpio get %d\n", 0, value);

            }
            if (flag & GES_RIGHT)
            {
                printf("\tRight\n");

                // 检测到Right状态时，将GPIO0_A2设置为HIGH

                LzGpioSetVal(GPIO0_PA2, 1);
                LzGpioGetVal(GPIO0_PA2, &value);
                printf("\tgpio set %d => gpio get %d\n", 1, value);

            }
            if (flag & GES_FORWARD)
            {
                printf("\tForward\n");
            }
            if (flag & GES_BACKWARD)
            {
                printf("\tBackward\n");
            }
            if (flag & GES_CLOCKWISE)
            {
                printf("\tClockwise\n");
            }
            if (flag & GES_COUNT_CLOCKWISE)
            {
                printf("\tCount Clockwise\n");
            }
            if (flag & GES_WAVE)
            {
                printf("\tWave\n");
            }
            
            e53_gs_led_up_set((flag & GES_UP) ? (1) : (0));
            e53_gs_led_down_set((flag & GES_DOWM) ? (1) : (0));
            e53_gs_led_left_set((flag & GES_LEFT) ? (1) : (0));
            e53_gs_led_right_set((flag & GES_RIGHT) ? (1) : (0));
            e53_gs_led_forward_set((flag & GES_FORWARD) ? (1) : (0));
            e53_gs_led_backward_set((flag & GES_BACKWARD) ? (1) : (0));
            e53_gs_led_cw_set((flag & GES_CLOCKWISE) ? (1) : (0));
            e53_gs_led_ccw_set((flag & GES_COUNT_CLOCKWISE) ? (1) : (0));
            e53_gs_led_wave_set((flag & GES_WAVE) ? (1) : (0));
        }
        else
        {
            /* 如果没有数据，则多等待 */
            LOS_Msleep(100);
        }
    }
}

/***************************************************************
* 函数名称: savo_step_2_1_example
* 说    明: 第二阶段第一次测试
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void savo_step_2_1_example()
{
    unsigned int ret = LOS_OK;
    unsigned int thread_id;
    TSK_INIT_PARAM_S task = {0};

    // // 创建智慧路灯案例
    // task.pfnTaskEntry = (TSK_ENTRY_FUNC)e53_isl_thread;
    // task.uwStackSize = 10240;
    // task.pcName = "e53_isl_thread";
    // task.usTaskPrio = 24;
    
    // // 创建线程
    // ret = LOS_TaskCreate(&thread_id, &task);
    // if (ret != LOS_OK)
    // {
    //     printf("Falied to create e53_isl_thread ret:0x%x\n", ret);
    //     return;
    // }

    // 检测智能手势案例
    task.pfnTaskEntry = (TSK_ENTRY_FUNC)e53_gs_process;
    task.uwStackSize = 2048;
    task.pcName = "e53 getsture sensor process";
    task.usTaskPrio = 24;

    // 创建线程
    ret = LOS_TaskCreate(&thread_id, &task);
    if (ret != LOS_OK)
    {
        printf("Falied to create Task_One ret:0x%x\n", ret);
        return;
    }

}

APP_FEATURE_INIT(savo_step_2_1_example);
