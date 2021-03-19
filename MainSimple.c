#include "main.h"
#include "Board_LED.h"                  // ::Board Support:LED
#include "Driver_I2C.h"                 // ::CMSIS Driver:I2C

#define Slave_I2C_ADDR (0x70)

void Init_I2C(void);
char read1byte(unsigned char composant,unsigned char registre);

extern ARM_DRIVER_I2C Driver_I2C1;

int main(void)
{
	uint8_t buffer[2],donnee[2];
	int i;
	LED_Initialize();
	Init_I2C();
	LED_Off(1);
  while (1)
  {
		buffer[0]=0x00;
		buffer[1]=0x51;  //mesure en centimètre
		LED_On (3);
		Driver_I2C1.MasterTransmit(Slave_I2C_ADDR,buffer,2,false);
		while (Driver_I2C1.GetStatus().busy==1);
		for (i=0; i<250000; i++);
		donnee[0]=read1byte(Slave_I2C_ADDR,0x03);
		donnee[1]=read1byte(Slave_I2C_ADDR,0x02);
  }
    
}

void Init_I2C(void){
Driver_I2C1.Initialize(NULL);
Driver_I2C1.PowerControl(ARM_POWER_FULL);
Driver_I2C1.Control(ARM_I2C_BUS_SPEED,ARM_I2C_BUS_SPEED_STANDARD);
LED_On (2);
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