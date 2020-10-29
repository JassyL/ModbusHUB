/*
 * @Descripttion: modbusHUB 自定义采集数据
 * @version: 
 * @Author: JunQi Liu
 * @Date: 2020-10-30 00:34:25
 * @LastEditors: JunQi Liu
 * @LastEditTime: 2020-10-30 02:50:26
 */
#include "modbus_instance.h"
#include "modbus.h"
#include "stdio.h"
#include "string.h"

#define GET_BIT(value,bit)  ((value&(1<<bit))>>bit) 

modbus_mapping_t *mb_mapping = RT_NULL;

/**
 * @brief:  电能表16位原始数据转整形
 * @param {type} rt_uint16_t raw 原始数据
 * @msg: 
 * @return: 
 */
uint16_t raw16_to_int(rt_uint16_t raw)
{
    uint16_t data;
    data = raw/4096 * 1000 + raw%4096/256 *100 + raw%256/16 * 10 + raw%16;
    return data;
}

/**
 * @brief: 电能表32原始数据转整形
 * @param {type} rt_uint32_t raw 原始数据
 * @msg: 
 * @return: 
 */
uint16_t raw32_to_int(rt_uint32_t raw)
{
    uint16_t data;
    data = raw%268435456/16777216*10000 + raw%16777216/1048576*1000 + raw%1048576/65536*100\
            + raw%65536/4096*10 + raw%4096/256;
    if(data>=0xffff){data = 0xffff;}
    return data;
}

/**
 * @brief: 电能表32原始数据转整形
 * @param {type} rt_uint32_t raw 原始数据
 * @msg: 
 * @return: 
 */
uint16_t raw32_to_int_4point(rt_uint32_t raw)
{
    uint16_t data;
    data = raw%4294967296/268435456*10000 + raw%268435456/16777216*1000 + raw%16777216/1048576*100 + raw%1048576/65536*10\
            + raw%65536/4096;
	printf("data = %d \n",data);
    if(data>=0xffff){data = 0xffff;}
    return data;
}

/**
 * @brief: modbus 读取线程
 * @param {*}
 * @msg: 
 * @return {*}
 */
static void modbus_master_thread(void *parameter)
{
    uint16_t ZM0201_parameters[5][10] = {0};    //电能表参数区
    uint16_t ZM0201_status[5][45] = {0};        //电能表电状态参数区
    uint8_t index = 0;
    modbus_t *ctx = RT_NULL;
    int res = 0;
    int i = 0;
    ctx = modbus_new_rtu("/dev/uart2", 9600, 'N', 8, 1);
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    modbus_set_slave(ctx, 1);
    modbus_connect(ctx);
    modbus_set_response_timeout(ctx, 0, 1000000);

    while (1)
    {
        memset(ZM0201_parameters, 0, 5*10*2);
        memset(ZM0201_status, 0, 5*45*2);
        for(index = 1; index <= 1; index ++)
        {
            modbus_set_slave(ctx, index);
            res = modbus_read_registers(ctx, 0, 10, ZM0201_parameters[index-1]);
            printf("-------------Get parameters %d-----------------\n",index);
            printf("[read result = %d]", res);
            for (i = 0; i < 10; i++)
            {
                printf("<%#x>", ZM0201_parameters[index-1][i]);       //打印缓存区1数据
            }
            printf("\n");
            printf("-------------------------------------------\n");
            if(res != -1)
            {
                //读取成功
                mb_mapping->tab_bits[10*(index-1)+0] = GET_BIT(ZM0201_parameters[index-1][6],0);
                mb_mapping->tab_bits[10*(index-1)+1] = GET_BIT(ZM0201_parameters[index-1][6],1);
                mb_mapping->tab_bits[10*(index-1)+2] = GET_BIT(ZM0201_parameters[index-1][6],2);
                mb_mapping->tab_bits[10*(index-1)+3] = GET_BIT(ZM0201_parameters[index-1][6],3);
                mb_mapping->tab_bits[10*(index-1)+4] = GET_BIT(ZM0201_parameters[index-1][6],4);
            }
			rt_thread_mdelay(10);
            res = modbus_read_registers(ctx, 0x20, 0x2D, ZM0201_status[index-1]);
            printf("-------------Get status %d-----------------\n",index);
            printf("[read result = %d]", res);
            for (i = 0; i < 45; i++)
            {
                printf("<%#x>", ZM0201_status[index-1][i]);       //打印缓存区1数据
            }
            printf("\n");
            printf("-------------------------------------------\n");
            if(res != -1)
            {
                //读取成功
                mb_mapping->tab_registers[10*(index-1)+0] = raw16_to_int(ZM0201_status[index-1][0x2C-0X20]);
                mb_mapping->tab_registers[10*(index-1)+1] = raw16_to_int(ZM0201_status[index-1][0x38-0X20]);
                mb_mapping->tab_registers[10*(index-1)+2] = raw16_to_int(ZM0201_status[index-1][0x44-0X20]);
                mb_mapping->tab_registers[10*(index-1)+3] = raw32_to_int(((rt_uint32_t)(ZM0201_status[index-1][0x2D-0X20])<<16) + ZM0201_status[index-1][0x2E-0X20]);
                mb_mapping->tab_registers[10*(index-1)+4] = raw32_to_int(((rt_uint32_t)(ZM0201_status[index-1][0x39-0X20])<<16) + ZM0201_status[index-1][0x3A-0X20]);
                mb_mapping->tab_registers[10*(index-1)+5] = raw32_to_int(((rt_uint32_t)(ZM0201_status[index-1][0x45-0X20])<<16) + ZM0201_status[index-1][0x46-0X20]);
                mb_mapping->tab_registers[10*(index-1)+6] = raw32_to_int_4point(((rt_uint32_t)(ZM0201_status[index-1][0x24-0X20])<<16) + ZM0201_status[index-1][0x25-0X20]);
                mb_mapping->tab_registers[10*(index-1)+7] = raw16_to_int(ZM0201_status[index-1][0x4C-0X20]);
            }
        }
        rt_thread_mdelay(10000);
    }
    //7-关闭modbus端口
    modbus_close(ctx);

    //8-释放modbus资源
    modbus_free(ctx);
}

void modbus_slave_thread(void *parameter)
{
    modbus_t *ctx_s = RT_NULL;

    int rc;
    ctx_s = modbus_new_rtu("/dev/uart3", 9600, 'N', 8, 1);
    modbus_set_slave(ctx_s, 1);
    modbus_connect(ctx_s);
    modbus_set_response_timeout(ctx_s, 0, 1000000);
    mb_mapping = modbus_mapping_new(MODBUS_MAX_READ_BITS, 0,
                                    MODBUS_MAX_READ_REGISTERS, 0);
    if (mb_mapping == NULL) {
        printf("Failed to allocate the mapping\n");
        modbus_free(ctx_s);
    }
    while( 1 ){
        uint8_t query[MODBUS_RTU_MAX_ADU_LENGTH];
        //轮询接收数据，并做相应处理
        rc = modbus_receive(ctx_s, query);
        if (rc > 0) {
            modbus_reply(ctx_s, query, rc, mb_mapping);
        } else if (rc  == -1) {
            /* Connection closed by the client or error */
            break;
        }
    }

    printf("Quit the loop\n");
 
    modbus_mapping_free(mb_mapping);
    /* For RTU, skipped by TCP (no TCP connect) */
    modbus_close(ctx_s);
    modbus_free(ctx_s);
}


static int rtu_test_init(void)
{
    //rt_pin_mode(RS485_RE, PIN_MODE_OUTPUT);
    rt_thread_t tid,tid_s;
    tid = rt_thread_create("master",
                           modbus_master_thread, RT_NULL,
                           4096*2,
                           12, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    tid_s = rt_thread_create("slave",
                           modbus_slave_thread, RT_NULL,
                           2048,
                           11, 10);
    if (tid_s != RT_NULL)
        rt_thread_startup(tid_s);
    return RT_EOK;
}
INIT_APP_EXPORT(rtu_test_init);
