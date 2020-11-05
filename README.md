<!--
 * @Descripttion: 
 * @version: 
 * @Author: JunQi Liu
 * @Date: 2020-10-28 12:51:02
 * @LastEditors: JunQi Liu
 * @LastEditTime: 2020-10-30 03:27:34
-->
# ModbusHUB
![CI](https://github.com/Team-Embedded/ModbusHUB/workflows/CI/badge.svg)
[![](https://img.shields.io/badge/LICENSE-Apache-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0)
## 简介

**ModbusHUB** 是一个运行 RT-Thread 的 Modbus 集中转换器，用于 Modbus 设备的地址转换，数据存储格式转换，多设备集中。提供一个 modbus 主机接口，用于连接下行 modbus 总线，提供一个 modbus 从机接口，用于连接上行 modbus 总线。

## 背景
* 各个小厂的 modbus 设备实际上对 modbus 协议的支持参差不齐，部分主机设备无法匹配部分从机设备，对使用 modbus 设备的系统很不友好，通过**ModbusHUB**，可以完成下行从设备的数据读取，并转换成可供上行从设备的读取的数据格式或者功能码支持。

* modbus 设备过多，部分 modbus 主机设备支持的从机设备数量有限，通过 **ModbusHUB** 可以将多个设备整合成一个设备，间接提高 modbus 主机设备可带从设备数量。

* 淘宝有类似的设备存在，大都通过上位机配置，但基本都无法达到完整自定义的需求，且做工也较为粗糙。

## 实现
基于 RT-Thread 嵌入式实时操作系统，BSP 支持了6串口，主要使用第三方移植到 RTT 的 [libmodbus](https://github.com/loogg/libmodbus) 库，其中使用了3个串口，一个用于调试，一个用于连接下行从机，一个用于连接上行主机。

实际上就是运行了一个 modbus master 和一个 modbus slave，原理非常简单，但是没有见到有完善的产品卖，故自行完成了一个。

## 使用
> TODO

## 演示
> TODO
