/*
 * @Descripttion: 
 * @version: 
 * @Author: JunQi Liu
 * @Date: 2020-10-30 00:35:39
 * @LastEditors: JunQi Liu
 * @LastEditTime: 2020-10-30 02:14:45
 */
#ifndef __MODBUS_INSTANCE_H__
#define __MODBUS_INSTANCE_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* 寄存器定义
保持寄存器
|地址   |   定义    |
--------------------
00      A相电压
01      B相电压
02      C相电压
03      A相电流
04      B相电流
05      C相电流
06      总有功功率
07      电网频率   

线圈
|地址   |   定义    |
--------------------
00      1: A 相失压 0: A 相未失压
01      1: B 相失压 0: B 相未失压
02      1: C 相失压 0: C 相未失压
03      1: 电压逆向序错 0: 电压逆向序未错
04      1: 电流逆向序错 0: 电流逆向序未错

10      1: A 相失压 0: A 相未失压
11      1: B 相失压 0: B 相未失压
12      1: C 相失压 0: C 相未失压
13      1: 电压逆向序错 0: 电压逆向序未错
14      1: 电流逆向序错 0: 电流逆向序未错

20      1: A 相失压 0: A 相未失压
21      1: B 相失压 0: B 相未失压
22      1: C 相失压 0: C 相未失压
23      1: 电压逆向序错 0: 电压逆向序未错
24      1: 电流逆向序错 0: 电流逆向序未错

30      1: A 相失压 0: A 相未失压
31      1: B 相失压 0: B 相未失压
32      1: C 相失压 0: C 相未失压
33      1: 电压逆向序错 0: 电压逆向序未错
34      1: 电流逆向序错 0: 电流逆向序未错

40      1: A 相失压 0: A 相未失压
41      1: B 相失压 0: B 相未失压
42      1: C 相失压 0: C 相未失压
43      1: 电压逆向序错 0: 电压逆向序未错
44      1: 电流逆向序错 0: 电流逆向序未错
*/


#endif

