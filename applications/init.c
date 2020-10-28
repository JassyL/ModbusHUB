/*
 * @Author: JunQiLiu
 * @Date: 2020-10-12 21:13:48
 * @LastEditTime: 2020-10-12 21:22:03
 * @Description: 初始化任务
 * @FilePath: \ModbusHUB\applications\init.c
 * @ 
 */
#include "init.h"
#include <fal.h>
#include "spi_flash.h"
#include "spi_flash_sfud.h"
#include "drv_spi.h"
#include <easyflash.h>

/**
 * @description: spi设备硬件初始化
 * @param void 
 * @return static int status
 * @note: CS PIN PB14
 * @Author: JunQiLiu
 */
#if defined(BSP_USING_SPI_FLASH)
static int rt_hw_spi_flash_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    rt_hw_spi_device_attach("spi1", "spi10", GPIOB, GPIO_PIN_14);

    if (RT_NULL == rt_sfud_flash_probe("W25Q128", "spi10"))
    {
        return -RT_ERROR;
    };

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_init);
#endif

/**
 * @description: 总初始化
 * @param {type} 
 * @return {type} 
 * @note: 
 * @Author: JunQiLiu
 */
int system_init(void)
{
    fal_init(); /* fal初始化 */
    easyflash_init();   /* easyflash 初始化 */
    return RT_EOK;
}

