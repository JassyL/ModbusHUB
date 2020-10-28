#include "modbus_rtu_test.h"
#include "modbus.h"
#include "stdio.h"
#include "string.h"

#define RS485_RE GET_PIN(G, 8)

modbus_mapping_t *mb_mapping = RT_NULL;

static void test_thread(void *param)
{
    uint16_t tab_reg[64] = {0};
    modbus_t *ctx = RT_NULL;
    ctx = modbus_new_rtu("/dev/uart2", 115200, 'N', 8, 1);
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    //modbus_rtu_set_rts(ctx, RS485_RE, MODBUS_RTU_RTS_UP);
    modbus_set_slave(ctx, 1);
    modbus_connect(ctx);
    modbus_set_response_timeout(ctx, 0, 100000);
    int num = 0;
    while (1)
    {
        memset(tab_reg, 0, 64 * 2);
        int regs = modbus_read_registers(ctx, 0, 10, tab_reg);
        printf("-------------------------------------------\n");
        printf("[%4d][read num = %d]", num, regs);
        num++;
        int i;
        for (i = 0; i < 10; i++)
        {
            mb_mapping->tab_registers[i] = tab_reg[i];
            printf("<%#x>", tab_reg[i]);
        }
        printf("\n");
        printf("-------------------------------------------\n");
        rt_thread_mdelay(5000);
    }
    //7-关闭modbus端口
    modbus_close(ctx);

    //8-释放modbus资源
    modbus_free(ctx);
}

/**
 * @description: modbus slave 线程
 * @param {type} 
 * @return {type} 
 */
void modbus_slave_entry(void *parameter)
{
    modbus_t *ctx_s = RT_NULL;

    int rc;
    int use_backend;
    ctx_s = modbus_new_rtu("/dev/uart3", 9600, 'N', 8, 1);
    modbus_set_slave(ctx_s, 1);
    modbus_connect(ctx_s);
    modbus_set_response_timeout(ctx_s, 0, 1000000);
    mb_mapping = modbus_mapping_new(0, 0,
                                    MODBUS_MAX_READ_REGISTERS, 0);
    if (mb_mapping == NULL) {
        printf("Failed to allocate the mapping: %s\n");
        modbus_free(ctx_s);
    }
    mb_mapping->tab_registers[0] = 1;
    mb_mapping->tab_registers[1] = 2;
    mb_mapping->tab_registers[2] = 3;
    mb_mapping->tab_registers[3] = 4;
    mb_mapping->tab_registers[4] = 5;
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

    printf("Quit the loop: %s\n");
 
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
                           test_thread, RT_NULL,
                           2048,
                           12, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    tid_s = rt_thread_create("slave",
                           modbus_slave_entry, RT_NULL,
                           2048,
                           11, 10);
    if (tid_s != RT_NULL)
        rt_thread_startup(tid_s);
    return RT_EOK;
}
INIT_APP_EXPORT(rtu_test_init);
