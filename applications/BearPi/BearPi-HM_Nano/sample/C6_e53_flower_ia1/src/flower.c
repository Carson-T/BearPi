/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
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
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "cmsis_os2.h"
#include "flower.h"
#include "wifiiot_errno.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_i2c.h"
#include "wifiiot_i2c_ex.h"
#include "wifiiot_adc.h"

/***************************************************************
* 函数名称: Flower_IO_Init
* 说    明: Flower_GPIO初始化
* 参    数: 无
* 返 回 值: 无
***************************************************************/
static void Flower_IO_Init(void)
{

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_5, WIFI_IOT_IO_FUNC_GPIO_5_GPIO);//设置GPIO_5的复用功能为普通GPIO
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_5, WIFI_IOT_GPIO_DIR_OUT);//设置GPIO_5为输出模式

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_IO_FUNC_GPIO_8_GPIO);//设置GPIO_8的复用功能为普通GPIO
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_GPIO_DIR_OUT);//设置GPIO_8为输出模式

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_14, WIFI_IOT_IO_FUNC_GPIO_14_GPIO);//设置GPIO_14的复用功能为普通GPIO
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_14, WIFI_IOT_GPIO_DIR_OUT);//设置GPIO_14为输出模式

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_0, WIFI_IOT_IO_FUNC_GPIO_0_I2C1_SDA);   // GPIO_0复用为I2C1_SDA
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_1, WIFI_IOT_IO_FUNC_GPIO_1_I2C1_SCL);   // GPIO_1复用为I2C1_SCL
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_10, WIFI_IOT_IO_FUNC_GPIO_10_I2C0_SDA);   // GPIO_10复用为I2C1_SDA
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_IO_FUNC_GPIO_9_I2C0_SCL);   // GPIO_9复用为I2C1_SCL

    I2cInit(WIFI_IOT_I2C_IDX_1, 400000); /* baudrate: 400kbps */
    I2cSetBaudrate(WIFI_IOT_I2C_IDX_1, 400000);
    I2cInit(WIFI_IOT_I2C_IDX_0, 400000); /* baudrate: 400kbps */
    I2cSetBaudrate(WIFI_IOT_I2C_IDX_0, 400000);
}
/***************************************************************
* 函数名称: Init_BH1750
* 说    明: 写命令初始化BH1750
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void Init_BH1750(void)
{
    WifiIotI2cData bh1750_i2c_data = { 0 };
    uint8_t send_data[1] = { 0x01 };
    bh1750_i2c_data.sendBuf = send_data;
    bh1750_i2c_data.sendLen = 1;
	I2cWrite(WIFI_IOT_I2C_IDX_1,(BH1750_Addr<<1)|0x00,&bh1750_i2c_data); 
}

/***************************************************************
* 函数名称: Start_BH1750
* 说    明: 启动BH1750
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void Start_BH1750(void)
{
    WifiIotI2cData bh1750_i2c_data = { 0 };
    uint8_t send_data[1] = { 0x10 };
    bh1750_i2c_data.sendBuf = send_data;
    bh1750_i2c_data.sendLen = 1;
	I2cWrite(WIFI_IOT_I2C_IDX_1,(BH1750_Addr<<1)|0x00,&bh1750_i2c_data); 
}


/***************************************************************
* 函数名称: SHT30_reset
* 说    明: SHT30复位
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void SHT30_reset(void)
{
    WifiIotI2cData sht30_i2c_data = { 0 };
    uint8_t send_data[2] = { 0x30,0xA2 };
    sht30_i2c_data.sendBuf = send_data;
    sht30_i2c_data.sendLen = 2;
	I2cWrite(WIFI_IOT_I2C_IDX_0,(SHT30_Addr<<1)|0x00,&sht30_i2c_data); 
}

/***************************************************************
* 函数名称: Init_SHT30
* 说    明: 初始化SHT30，设置测量周期
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void Init_SHT30(void)
{
    WifiIotI2cData sht30_i2c_data = { 0 };
    uint8_t send_data[2] = { 0x22,0x36 };
    sht30_i2c_data.sendBuf = send_data;
    sht30_i2c_data.sendLen = 2;
    I2cWrite(WIFI_IOT_I2C_IDX_0,(SHT30_Addr<<1)|0x00,&sht30_i2c_data); 
}

/***************************************************************
* 函数名称: SHT3x_CheckCrc
* 说    明: 检查数据正确性
* 参    数: data：读取到的数据
						nbrOfBytes：需要校验的数量
						checksum：读取到的校对比验值
* 返 回 值: 校验结果，0-成功		1-失败
***************************************************************/
static uint8_t SHT3x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
{
	
    uint8_t crc = 0xFF;
    uint8_t bit = 0;
    uint8_t byteCtr ;
	const int16_t POLYNOMIAL = 0x131;
    //calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }
	
    if(crc != checksum)
		return 1;
    else
		return 0;
	
}

/***************************************************************
* 函数名称: SHT3x_CalcTemperatureC
* 说    明: 温度计算
* 参    数: u16sT：读取到的温度原始数据
* 返 回 值: 计算后的温度数据
***************************************************************/
static float SHT3x_CalcTemperatureC(uint16_t u16sT)
{
	
    float temperatureC = 0;            // variable for result

    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [℃] --
    temperatureC = (175 * (float)u16sT / 65535 - 45); //T = -45 + 175 * rawValue / (2^16-1)
	
    return temperatureC;
	
}

/***************************************************************
* 函数名称: SHT3x_CalcRH
* 说    明: 湿度计算
* 参    数: u16sRH：读取到的湿度原始数据
* 返 回 值: 计算后的湿度数据
***************************************************************/
static float SHT3x_CalcRH(uint16_t u16sRH)
{
	
    float humidityRH = 0;              // variable for result
	
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    humidityRH = (100 * (float)u16sRH / 65535);  // RH = rawValue / (2^16-1) * 10
	
    return humidityRH;
	
}
/***************************************************************
* 函数名称: Flower_Init
* 说    明: 初始化Flower_
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void Flower_Init(void)
{
    Flower_IO_Init();
   	Init_BH1750();
    Init_SHT30();
}

/***************************************************************
* 函数名称: Flower_Read_Data
* 说    明: 测量土壤湿度
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void Flower_Read_Data()
{
    unsigned int ret;
    unsigned short adc;

    ret = AdcRead(WIFI_IOT_ADC_CHANNEL_6, &adc, WIFI_IOT_ADC_EQU_MODEL_8, WIFI_IOT_ADC_CUR_BAIS_DEFAULT, 0xff); //读取土壤湿度
    if (ret != WIFI_IOT_SUCCESS)
    {
        printf("ADC Read Fail\n");
    }
    Flower_Data.Smo = (float)adc/1500*100;

    Start_BH1750(); // 启动光照强度传感器采集数据
	usleep(180000);
    WifiIotI2cData bh1750_i2c_data = { 0 };
    uint8_t recv_data[2] = { 0 };
    bh1750_i2c_data.receiveBuf = recv_data;
    bh1750_i2c_data.receiveLen = 2;
	I2cRead(WIFI_IOT_I2C_IDX_1, (BH1750_Addr<<1)|0x01,&bh1750_i2c_data);   // 读取光照强度传感器数据
	
    Flower_Data.Lux = (float)(((recv_data[0]<<8) + recv_data[1])/1.2);
    
    uint8_t  data[3];    //data array for checksum verification
    uint16_t dat,tmp;
    uint8_t SHT3X_Data_Buffer[6]; 								//byte 0,1 is temperature byte 4,5 is humidity
    
    WifiIotI2cData sht30_i2c_data = { 0 };
    uint8_t send_data[2] = { 0xE0,0x00};
    sht30_i2c_data.sendBuf = send_data;
    sht30_i2c_data.sendLen = 2;
    sht30_i2c_data.receiveBuf = SHT3X_Data_Buffer;
    sht30_i2c_data.receiveLen = 6;
	I2cWriteread(WIFI_IOT_I2C_IDX_0,(SHT30_Addr<<1)|0x00,&sht30_i2c_data); 
    
    //    /* check tem */
    data[0] = SHT3X_Data_Buffer[0];
    data[1] = SHT3X_Data_Buffer[1];
    data[2] = SHT3X_Data_Buffer[2];

    tmp=SHT3x_CheckCrc(data, 2, data[2]);
    if( !tmp ) /* value is ture */
    {
        dat = ((uint16_t)data[0] << 8) | data[1];
        Flower_Data.Temperature = SHT3x_CalcTemperatureC( dat );    
    }
    
    //    /* check humidity */
    data[0] = SHT3X_Data_Buffer[3];
    data[1] = SHT3X_Data_Buffer[4];
    data[2] = SHT3X_Data_Buffer[5];

    tmp=SHT3x_CheckCrc(data, 2, data[2]);
    if( !tmp ) /* value is ture */
    {
        dat = ((uint16_t)data[0] << 8) | data[1];
        Flower_Data.Humidity = SHT3x_CalcRH( dat );    
    }
}
/***************************************************************
* 函数名称: Motor_StatusSet
* 说    明: 电机状态设置
* 参    数: status,ENUM枚举的数据
*									CLOSE,关
*									OPEN,开
* 返 回 值: 无
***************************************************************/
void Motor_Flower_StatusSet(Flower_Status_ENUM status)
{
    if (status == OPEN)

        //设置GPIO_5输出高电平打开抽水电机
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_5, WIFI_IOT_GPIO_VALUE0);
    if (status == CLOSE)

        //设置GPIO_5输出低电平关闭抽水电机
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_5, WIFI_IOT_GPIO_VALUE1);
}

void Motor_Fan_StatusSet(Flower_Status_ENUM status)
{
    if (status == OPEN)

        //设置GPIO_8输出高电平打开风扇电机
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_8, WIFI_IOT_GPIO_VALUE1);
    if (status == CLOSE)

        //设置GPIO_8输出低电平关闭风扇电机
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_8, WIFI_IOT_GPIO_VALUE0);
}

void LED_Light_StatusSet(Flower_Status_ENUM status){
    if (status == OPEN)

        //设置GPIO_14输出高电平打开补光灯
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_14, WIFI_IOT_GPIO_VALUE1);
    if (status == CLOSE)

        //设置GPIO_14输出低电平关闭补光灯
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_14, WIFI_IOT_GPIO_VALUE0);
}