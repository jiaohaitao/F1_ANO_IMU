/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * 作者   ：匿名科创
 * 文件名  ：ak8975.c
 * 描述    ：电子罗盘驱动
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
**********************************************************************************/

#include "ak8975.h"
#include "mymath.h"
#include "include.h"
#include "i2c_soft.h"

#define USE_AK8975
// 	xyz_f_t Mag_Offset = { -1 , -1 , -1 };
// 	xyz_f_t Mag_Gain   = { 1 , 0.8538 , 0.9389 };

//ak8975_t ak8975 = { {0,0,0},{-1,-1,-1},{1,0.8538,0.9389},{0,0,0} };
ak8975_t ak8975 = { {0,0,0},{-76,-36,-58},{1,0.8538,0.9389},{0,0,0} };
bool ANO_AK8975_Run(void)
{
#ifdef USE_AK8975	
	return IIC_Write_1Byte(AK8975_ADDRESS,AK8975_CNTL,0x01);	
#else			//USE_HMC5883
	//			IIC_Write_1Byte(HMC5883_Addr,0x00,0x14);
	//return			IIC_Write_1Byte(HMC5883_Addr,0x02,0x00); 
	IIC_Write_1Byte(HMC5883_Addr,0x02,0x01);
#endif	
	return 0;
}

xyz_f_t XYZ_STRUCT_COPY(float x,float y, float z)
{
	xyz_f_t m ;
	m.x = x;
	m.y = y;
	m.z = z;
	return m;
}
u8 ak8975_ok;

void delay()
{
	unsigned int i=0;
	while(i++<10000);
}

#define AK8975_MODE_POWERDOWN     0x0
#define AK8975_MODE_SINGLE        0x1
#define AK8975_MODE_SELFTEST      0x8
#define AK8975_MODE_FUSEROM       0xF


#define AK8975_RA_CNTL            0x0A
void ANO_AK8975_Read_Mag_Data(void)
{
	int16_t mag_temp[3];
	u8 ak8975_buffer[6]; //接收数据缓存
	
	I2C_FastMode = 0;
	
#ifdef 	USE_AK8975
	IIC_Read_1Byte(AK8975_ADDRESS,AK8975_HXL,&ak8975_buffer[0]); 
	IIC_Read_1Byte(AK8975_ADDRESS,AK8975_HXH,&ak8975_buffer[1]);
	mag_temp[0] = ((((int16_t)ak8975_buffer[1]) << 8) | ak8975_buffer[0]) ;  //磁力计X轴

	IIC_Read_1Byte(AK8975_ADDRESS,AK8975_HYL,&ak8975_buffer[2]);
	IIC_Read_1Byte(AK8975_ADDRESS,AK8975_HYH,&ak8975_buffer[3]);
	mag_temp[1] = -((((int16_t)ak8975_buffer[3]) << 8) | ak8975_buffer[2]) ;  //磁力计Y轴

	IIC_Read_1Byte(AK8975_ADDRESS,AK8975_HZL,&ak8975_buffer[4]);
	IIC_Read_1Byte(AK8975_ADDRESS,AK8975_HZH,&ak8975_buffer[5]);
	mag_temp[2] = -((((int16_t)ak8975_buffer[5]) << 8) | ak8975_buffer[4]) ;  //磁力计Z轴	
#else		//USE_HMC5883
//	IIC_Write_1Byte(HMC5883_Addr,0x00,0x14);		
//	//Delay_ms(1);
//	IIC_Write_1Byte(HMC5883_Addr,0x01,0x20);		
//	//Delay_ms(1);
//	IIC_Write_1Byte(HMC5883_Addr,0x02,0x00);
	//Delay_ms(1);	
	
	
	IIC_Read_1Byte(HMC5883_Addr,HMC5883_HXL,&ak8975_buffer[0]); 
	IIC_Read_1Byte(HMC5883_Addr,HMC5883_HXH,&ak8975_buffer[1]);
	mag_temp[0] = ((((int16_t)ak8975_buffer[1]) << 8) | ak8975_buffer[0]) ;  //磁力计X轴

	IIC_Read_1Byte(HMC5883_Addr,HMC5883_HYL,&ak8975_buffer[2]);
	IIC_Read_1Byte(HMC5883_Addr,HMC5883_HYH,&ak8975_buffer[3]);
	mag_temp[1] = ((((int16_t)ak8975_buffer[3]) << 8) | ak8975_buffer[2]) ;  //磁力计Y轴

	IIC_Read_1Byte(HMC5883_Addr,HMC5883_HZL,&ak8975_buffer[4]);
	IIC_Read_1Byte(HMC5883_Addr,HMC5883_HZH,&ak8975_buffer[5]);
	mag_temp[2] = ((((int16_t)ak8975_buffer[5]) << 8) | ak8975_buffer[4]) ;  //磁力计Z轴		
	
//		Single_Write(HMC5883L_Addr,0x00,0x14);   //ÅäÖÃ¼Ä´æÆ÷A£º²ÉÑùÆ½¾ùÊý1 Êä³öËÙÂÊ75Hz Õý³£²âÁ¿
//	Single_Write(HMC5883L_Addr,0x01,0x20);   //ÅäÖÃ¼Ä´æÆ÷B£ºÔöÒæ¿ØÖÆ
//	Single_Write(HMC5883L_Addr,0x02,0x00);   //Ä£Ê½¼Ä´æÆ÷£ºÁ¬Ðø²âÁ¿Ä£Ê½
		
	


#endif	
	
	ak8975.Mag_Adc.x = mag_temp[0];
	ak8975.Mag_Adc.y = mag_temp[1];
	ak8975.Mag_Adc.z = mag_temp[2];
	
	ak8975.Mag_Val.x = (ak8975.Mag_Adc.x - ak8975.Mag_Offset.x) ;
	ak8975.Mag_Val.y = (ak8975.Mag_Adc.y - ak8975.Mag_Offset.y) ;
	ak8975.Mag_Val.z = (ak8975.Mag_Adc.z - ak8975.Mag_Offset.z) ;
	//磁力计中点矫正	
//	ANO_AK8975_CalOffset_Mag();
	
	//AK8975采样触发
	ANO_AK8975_Run();
}

xyz_f_t ANO_AK8975_Get_Mag(void)
{
	return ak8975.Mag_Val;
}

u8 Mag_CALIBRATED = 0;
//磁力计中点矫正

void ANO_AK8975_CalOffset_Mag(void)
{
	static xyz_f_t	MagMAX = { -100 , -100 , -100 }, MagMIN = { 100 , 100 , 100 }, MagSum;
	static uint16_t cnt_m=0;
	
	if(Mag_CALIBRATED)
	{	
		
		if(ABS(ak8975.Mag_Adc.x)<1000&&ABS(ak8975.Mag_Adc.y)<1000&&ABS(ak8975.Mag_Adc.z)<1000)
		{
			MagMAX.x = _MAX(ak8975.Mag_Adc.x, MagMAX.x);
			MagMAX.y = _MAX(ak8975.Mag_Adc.y, MagMAX.y);
			MagMAX.z = _MAX(ak8975.Mag_Adc.z, MagMAX.z);
			
			MagMIN.x = _MIN(ak8975.Mag_Adc.x, MagMIN.x);
			MagMIN.y = _MIN(ak8975.Mag_Adc.y, MagMIN.y);
			MagMIN.z = _MIN(ak8975.Mag_Adc.z, MagMIN.z);		
			
			if(cnt_m == CALIBRATING_MAG_CYCLES)
			{
				ak8975.Mag_Offset.x = (int16_t)((MagMAX.x + MagMIN.x) * 0.5f);
				ak8975.Mag_Offset.y = (int16_t)((MagMAX.y + MagMIN.y) * 0.5f);
				ak8975.Mag_Offset.z = (int16_t)((MagMAX.z + MagMIN.z) * 0.5f);
	
				MagSum.x = MagMAX.x - MagMIN.x;
				MagSum.y = MagMAX.y - MagMIN.y;
				MagSum.z = MagMAX.z - MagMIN.z;
				
				ak8975.Mag_Gain.y = MagSum.x / MagSum.y;
				ak8975.Mag_Gain.z = MagSum.x / MagSum.z;
				
//				Param_SaveMagOffset(&ak8975.Mag_Offset);//param_Save();//保存数据
				cnt_m = 0;
				Mag_CALIBRATED = 0;
			}
		}
		cnt_m++;
		
	}
	else
	{

	}
}

void ANO_AK8975_Read(void)
{
		//读取磁力计
		ANO_AK8975_Read_Mag_Data();
}


/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/

