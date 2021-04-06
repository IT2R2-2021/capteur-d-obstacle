/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#define Slave_I2C_ADDR (0x70)
#define Slave_I2C_ADDR2 (0x74)
#include "osObjects.h"                      // RTOS object definitions
#include "main.h"
#include "Board_LED.h"                  // ::Board Support:LED
#include "Driver_I2C.h"                 // ::CMSIS Driver:I2C

void CapteurAv(void const *argument);
void Init_I2C(void);
char read1byte(unsigned char composant,unsigned char registre);
void startranging(char SAD);

extern ARM_DRIVER_I2C Driver_I2C1;

osThreadId ID_TacheCapteurAv;

osThreadDef(CapteurAv,osPriorityNormal,1,0);
/*
 * main: initialize and start the system
 */
void startranging(char SAD){
	uint8_t buffer[2];
	buffer[0]=0x00;
	buffer[1]=0x51;  //mesure en centimètre
	Driver_I2C1.MasterTransmit(SAD,buffer,2,false);
	while (Driver_I2C1.GetStatus().busy==1);
}

 void Init_I2C(void){
Driver_I2C1.Initialize(NULL);
Driver_I2C1.PowerControl(ARM_POWER_FULL);
Driver_I2C1.Control(ARM_I2C_BUS_SPEED,ARM_I2C_BUS_SPEED_STANDARD);
//Driver_I2C1.Control(ARM_I2C_BUS_CLEAR,0);

}

char read1byte(unsigned char composant,unsigned char registre)
	{uint8_t lecture;
	int stat;
	do{
	stat=Driver_I2C1.MasterTransmit (composant, &registre, 1, true);		// rrue = sans stop
	while (Driver_I2C1.GetStatus().busy == 1);	// attente fin transmission
	}while (stat!=0);
	Driver_I2C1.MasterReceive (composant,&lecture, 1, false);		// false = avec stop
	while (Driver_I2C1.GetStatus().busy == 1);	// attente fin transmission*/
	
	return lecture;
	}
	
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here
	LED_Initialize();
	Init_I2C();
  // create 'thread' functions that start executing,
	ID_TacheCapteurAv=osThreadCreate(osThread(CapteurAv),NULL);
  // example: tid_name = osThreadCreate (osThread(name), NULL);

  osKernelStart ();                         // start thread execution
	osDelay(osWaitForever);
}

void CapteurAv(void const *argument){
	uint8_t donnee[2];
	int i;
	while (1)
  {
		startranging(Slave_I2C_ADDR);
		startranging(Slave_I2C_ADDR2);
		osDelay(40);
		donnee[0]=read1byte(Slave_I2C_ADDR,0x03);
		if (donnee[0]>10) LED_On(1);
		else LED_Off(1);
		//donnee[1]=read1byte(Slave_I2C_ADDR,0x02);*/
		donnee[1]=read1byte(Slave_I2C_ADDR2,0x03);
		if (donnee[1]>10) LED_On(2);
		else LED_Off(2);
  }
}
