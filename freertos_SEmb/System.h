/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Project for the subject SEmb, made by Diogo Rodrigues nº94240, Daniel Proanho nº101229, and Miguel Fernandes nº93790
 *
 * This file is the System.h, this file was created to keep the program cleaner of multiple repetition of library, call, and headers.
 *
 * In this file we have headers for the System, LCD, KEYBOARD, PWM, Configuration State, Active State, I2C and the Ultrasonic Sensor
 *
 * This file has the possibility to become a task by itself.
 *
 * A more detailed description of the functions will be in the overhead of the functions.
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "inc/hw_i2c.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/hibernate.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/////////////////////////////////////////////////////////////////////////////

// Global Variable

////////////////////////////////////////////////////////////////////////////

struct tm sTime, sTrigger;

int istate, iPIN, alarm_armed, itimeout, itrigger_distance, ialarm_fired;

xSemaphoreHandle g_pLCDSemaphore;

xSemaphoreHandle g_pAlarmTriggerSemaphore;

xQueueHandle g_pKEYBOARDQueue;

/////////////////////////////////////////////////////////////////////////////

// INICIATE THE SYSTEM

/////////////////////////////////////////////////////////////////////////////

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3
#define TRUE 1
#define FALSE 0

void init_system(void);

/////////////////////////////////////////////////////////////////////////////

// LCD

////////////////////////////////////////////////////////////////////////////

#define D7 GPIO_PIN_0
#define D6 GPIO_PIN_1
#define D5 GPIO_PIN_2
#define D4 GPIO_PIN_3
#define EN GPIO_PIN_4
#define RS GPIO_PIN_5

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(const char *a);
void Lcd_Write_String2(const char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Lcd_Write_Integer(long v);
void Lcd_Write_Integer2(long v);

///////////////////////////////////////////////////////////////////////////////

// KEYBOARD

//////////////////////////////////////////////////////////////////////////////

#define R4 GPIO_PIN_0
#define R3 GPIO_PIN_1
#define R2 GPIO_PIN_2
#define R1 GPIO_PIN_3
#define C4 GPIO_PIN_4
#define C3 GPIO_PIN_5
#define C2 GPIO_PIN_6
#define C1 GPIO_PIN_7

#define KEY_SIZE sizeof(char)
#define QUEUE_SIZE 5

int KeyboardTaskInit(void);

void KeyboardTask(void *pvParameters);

char keypadScan(void);

/////////////////////////////////////////////////////////////////////////////

// PWM

/////////////////////////////////////////////////////////////////////////////

void pwm0_init(void);

void pwm0_start(unsigned long frequence, unsigned long duty_cycle);

void pwm0_stop(void);

////////////////////////////////////////////////////////////////////////////

// STATE MACHINE

///////////////////////////////////////////////////////////////////////////

int StateMachineTaskInit(void);

void State_Machine(void *pvParameters);

////////////////////////////////////////////////////////////////////////////

// CONFIG STATE

///////////////////////////////////////////////////////////////////////////

void DateTimeSet(void);

void Get_Date(uint32_t *ui32Year, uint32_t *ui32Month, uint32_t *ui32Day);

void Get_Time(uint32_t *ui32Hour, uint32_t *ui32Min, uint32_t *ui32Sec);

int Set_PIN(void);

int Set_Distance(void);

int Set_Timeout(void);

////////////////////////////////////////////////////////////////////////////

// ATIVE STATE

////////////////////////////////////////////////////////////////////////////

void update_Date_Time(void);

void update_Trigger_Date_Time(void);

void show_Trigger_Date_Time(void);

int check_PIN (void);

void Active_state(void);

////////////////////////////////////////////////////////////////////////////

// I2C & ULTRASONIC SENSOR

///////////////////////////////////////////////////////////////////////////

void alarm_Triggered(void);

int SonarTaskInit(void);

void SonarTask(void *pvParameters);

void measure_sonar(void);

void get_value_sonar(void);

void start_I2C(void);

///////////////////////////////////////////////////////////////////////////

#endif /* SYSTEM_H_ */
