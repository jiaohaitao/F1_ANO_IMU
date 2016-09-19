
#include "stm32f10x.h"
#include"usart1.h"
#include"led.h"
#include "timer.h"
#include "i2c_soft.h"
#include "mpu6050.h"
#include "ak8975.h"
#include "imu.h"

__IO unsigned char SYS_1MS_FLAG=0;
__IO unsigned char SYS_10MS_FLAG=0;
__IO unsigned char SYS_15MS_FLAG=0;
__IO unsigned char SYS_20MS_FLAG=0;
__IO unsigned char SYS_50MS_FLAG=0;
__IO unsigned char SYS_100MS_FLAG=0;
__IO unsigned char SYS_1000MS_FLAG=0;

__IO unsigned char Sensor_Acc_Calibrate_In=0;
__IO unsigned char Sensor_Gyo_Calibrate_In=0;
__IO unsigned char Sensor_Mag_Calibrate_In=0;

__IO unsigned char Imu_Start_Flag=0;

void Sener_Calibrate(void);
void Send_Data(int16_t ad1,int16_t ad2,int16_t ad3,int16_t ad4,int16_t ad5,int16_t ad6,int16_t ad7,int16_t ad8,int16_t ad9);
void Data_Send_Status(float rol,float pit,float yaw);


extern ak8975_t ak8975 ;
extern MPU6050_STRUCT mpu6050;

short Acc_Offset[3]={0};
short Gyo_Offset[3]={0};

short Mag_Max[3]={0};
short Mag_Min[3]={0};
short Mag_Offset[3]={0};

int main(void)
{
	//my code
	float rol,pitch,yaw;
	int i=0;
	LED_GPIO_Config(); 
	USART1_Config();
	NVIC_Configuration();
	Timer_Configuration();
	Timer_Nvic_Configuration();
	I2c_Soft_Init();
	MPU6050_Init(20);   
	
	Start_Timer();
	LED_ON();
	printf("-------------------F1 ANO IMU.....\r\n");
	
	mpu6050.Acc_Offset.x=112;
	mpu6050.Acc_Offset.y=91;
	mpu6050.Acc_Offset.z=-144;	

	mpu6050.Gyro_Offset.x=-32;
	mpu6050.Gyro_Offset.y=29;
	mpu6050.Gyro_Offset.z=-14;
	
	while(1){
		if(SYS_1MS_FLAG==1){
			SYS_1MS_FLAG=0;
		}
		
		if(SYS_10MS_FLAG==1){
			SYS_10MS_FLAG=0;
		}

		if(SYS_15MS_FLAG==1){
			SYS_15MS_FLAG=0;
			
	
				
		}

		if(SYS_20MS_FLAG==1){
			SYS_20MS_FLAG=0;
			MPU6050_Read(); 		
			ANO_AK8975_Read();				
			
			if(Imu_Start_Flag!=1)
				continue;
			
//			mpu6050.Acc_I16.x-=Acc_Offset[0];
//			mpu6050.Acc_I16.y-=Acc_Offset[1];
//			mpu6050.Acc_I16.z-=Acc_Offset[2];
//			
//			mpu6050.Gyro_I16.x-=Gyo_Offset[0];
//			mpu6050.Gyro_I16.y-=Gyo_Offset[1];
//			mpu6050.Gyro_I16.z-=Gyo_Offset[2];
			
	mpu6050.Acc_I16.x -= mpu6050.Acc_Offset.x;
	mpu6050.Acc_I16.y -= mpu6050.Acc_Offset.y ;
	mpu6050.Acc_I16.z -= mpu6050.Acc_Offset.z ;
 
	mpu6050.Gyro_I16.x -=mpu6050.Gyro_Offset.x;
	mpu6050.Gyro_I16.y -= mpu6050.Gyro_Offset.y ;
	mpu6050.Gyro_I16.z -=mpu6050.Gyro_Offset.z ;			
		
			mpu6050.Gyro_deg.x = mpu6050.Gyro_I16.x *TO_ANGLE;
			mpu6050.Gyro_deg.y = mpu6050.Gyro_I16.y*TO_ANGLE;
			mpu6050.Gyro_deg.z = mpu6050.Gyro_I16.z *TO_ANGLE;
			
//			ak8975.Mag_Val.x = ak8975.Mag_Adc.x -Mag_Offset[0];
//			ak8975.Mag_Val.y = ak8975.Mag_Adc.y -Mag_Offset[1];
//			ak8975.Mag_Val.z = ak8975.Mag_Adc.z -Mag_Offset[2];	
			

			
			IMUupdate(0.01,mpu6050.Gyro_deg.x ,mpu6050.Gyro_deg.y,mpu6050.Gyro_deg.z,mpu6050.Acc_I16.x,mpu6050.Acc_I16.y,mpu6050.Acc_I16.z,&rol,&pitch,&yaw);
			Send_Data(mpu6050.Acc_I16.x,mpu6050.Acc_I16.y,mpu6050.Acc_I16.z,mpu6050.Gyro_I16.x,mpu6050.Gyro_I16.y,mpu6050.Gyro_I16.z,ak8975.Mag_Val.x,ak8975.Mag_Val.y,ak8975.Mag_Val.z);
			Data_Send_Status(rol,pitch,yaw);
			
			LED_ON_OFF();
		}

		if(SYS_50MS_FLAG==1){
			
			Sener_Calibrate();
			SYS_50MS_FLAG=0;
		}		

		if(SYS_100MS_FLAG==1){
			SYS_100MS_FLAG=0;
		}		

		if(SYS_1000MS_FLAG==1){
			SYS_1000MS_FLAG=0;
			
		}				
	}
}
void Sener_Calibrate(){
	static int mag_cal_cnt=0;
	int i=0;
	
	while(Sensor_Acc_Calibrate_In==1){
		printf("\r\n------Acc Calibrate ----strart!\r\n");
		Acc_Offset[0]=mpu6050.Acc_I16.x;
		Acc_Offset[1]=mpu6050.Acc_I16.y;
		Acc_Offset[2]=mpu6050.Acc_I16.z-4095;
		printf("\r\nAcc_X_Offset:%d",Acc_Offset[0]);
		printf("\r\nAcc_Y_Offset:%d",Acc_Offset[1]);
		printf("\r\nAcc_Z_Offset:%d",Acc_Offset[2]);
		printf("\r\n------Acc Calibrate ----end!\r\n");
		Sensor_Acc_Calibrate_In=0;
	}
	
	while(Sensor_Gyo_Calibrate_In==1){
		printf("\r\n------Gyo Calibrate ----strart!\r\n");
		Gyo_Offset[0]=mpu6050.Gyro_I16.x;
		Gyo_Offset[1]=mpu6050.Gyro_I16.y;
		Gyo_Offset[2]=mpu6050.Gyro_I16.z;
		printf("\r\nGyo_X_Offset:%d",Gyo_Offset[0]);
		printf("\r\nGyo_Y_Offset:%d",Gyo_Offset[1]);
		printf("\r\nGyo_Z_Offset:%d",Gyo_Offset[2]);
		printf("\r\n------Gyo Calibrate ----end!\r\n");
		Sensor_Gyo_Calibrate_In=0;	
	}
	
	if(Sensor_Mag_Calibrate_In==1){
		if(mag_cal_cnt==0)
		printf("\r\n------Mag Calibrate ----strart!\r\n");
		
		if(ABS(ak8975.Mag_Adc.x)>1000||ABS(ak8975.Mag_Adc.y)>1000||ABS(ak8975.Mag_Adc.z)>1000)
			return;
		for(i=0;i<3;i++){
				if(ak8975.Mag_Adc.x>=Mag_Max[0]){
					Mag_Max[0]=ak8975.Mag_Adc.x;
				}
				else if(ak8975.Mag_Adc.x<=Mag_Min[0]){
					Mag_Min[0]=ak8975.Mag_Adc.x;
				}
				else{}
				
				if(ak8975.Mag_Adc.y>=Mag_Max[1]){
					Mag_Max[1]=ak8975.Mag_Adc.y;
				}
				else if(ak8975.Mag_Adc.y<=Mag_Min[1]){
					Mag_Min[1]=ak8975.Mag_Adc.y;
				}
				else{}

				if(ak8975.Mag_Adc.z>=Mag_Max[2]){
					Mag_Max[2]=ak8975.Mag_Adc.z;
				}
				else if(ak8975.Mag_Adc.z<=Mag_Min[2]){
					Mag_Min[2]=ak8975.Mag_Adc.z;
				}
				else{}
					
		}
			
			
			if(mag_cal_cnt++>400){
				mag_cal_cnt=0;
				Sensor_Mag_Calibrate_In=0;
				Mag_Offset[0]=(Mag_Max[0]+Mag_Min[0])/2;
				Mag_Offset[1]=(Mag_Max[1]+Mag_Min[1])/2;
				Mag_Offset[2]=(Mag_Max[2]+Mag_Min[2])/2;
				printf("\r\nMag_X_Offset:%d",Mag_Offset[0]);
				printf("\r\nMag_Y_Offset:%d",Mag_Offset[1]);
				printf("\r\nMag_Z_Offset:%d",Mag_Offset[2]);				
				printf("\r\n------Mag Calibrate ----end!\r\n");	
			}
			
	}
}
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
void Data_Send_Status(float rol,float pit,float yaw)
{
	u8 _cnt=0,data_to_send[100]={0};
	vs16 _temp;vs32 _temp2 = 0;	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
		
	
	
	data_to_send[3] = _cnt-4;
	

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	for(i=0;i<_cnt;i++)
	{
		USART_SendData(USART1,data_to_send[i]);
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );	
	}
}


void Send_Data(int16_t ad1,int16_t ad2,int16_t ad3,int16_t ad4,int16_t ad5,int16_t ad6,int16_t ad7,int16_t ad8,int16_t ad9)
{
	unsigned char i=0;
	unsigned char _cnt=0,sum = 0;
//	unsigned int _temp;
	u8 data_to_send[50];

	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	

	data_to_send[_cnt++]=BYTE1(ad1);
	data_to_send[_cnt++]=BYTE0(ad1);
	data_to_send[_cnt++]=BYTE1(ad2);
	data_to_send[_cnt++]=BYTE0(ad2);
	data_to_send[_cnt++]=BYTE1(ad3);
	data_to_send[_cnt++]=BYTE0(ad3);
	
	data_to_send[_cnt++]=BYTE1(ad4);
	data_to_send[_cnt++]=BYTE0(ad4);
	data_to_send[_cnt++]=BYTE1(ad5);
	data_to_send[_cnt++]=BYTE0(ad5);
	data_to_send[_cnt++]=BYTE1(ad6);
	data_to_send[_cnt++]=BYTE0(ad6);
	data_to_send[_cnt++]=BYTE1(ad7);
	data_to_send[_cnt++]=BYTE0(ad7);
	data_to_send[_cnt++]=BYTE1(ad8);
	data_to_send[_cnt++]=BYTE0(ad8);
	data_to_send[_cnt++]=BYTE1(ad9);
	data_to_send[_cnt++]=BYTE0(ad9);
	
	data_to_send[3] = _cnt-4;
	//o¨ªD¡ê?¨¦
	for(i=0;i<_cnt;i++)
		sum+= data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	for(i=0;i<_cnt;i++){
		USART_SendData(USART1,data_to_send[i]);
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
	}
}

