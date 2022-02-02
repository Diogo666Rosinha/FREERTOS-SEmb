//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Project for the subject SEmb, made by Diogo Rodrigues nº94240, Daniel Proanho nº101229, and Miguel Fernandes nº93790
 *
 * This file contains all the code concerning the sonar handling and communication.
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "System.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* void alarm_Triggered(void);
 *
 * This function was made to keep the Active_state() a little cleaner and maybe this function can be used in another setting.
 *
 * It was made to handle the count down in case the alarm was triggered to give time to the user to input the correct PIN to disarm
 * the alarm. In case this is not done in time the Buzzer will start ringing continually and the program will stay locked until the alarm is disarm
 * in Beacon state (state 2) also blocking the access to the other functionalities of Active state.
 *
 * For the count down we use the RTC, we give a fixed value to n2  and update n1 with the time that has passed since the
 * end of line 36, if we subtract itimeout with n1 - n2 we will have a count down of itimeout seconds, n3 is there so that the display only shows
 * when n1 changes value and for the n1-n2 subtraction. Each second there will be a beep to show that time is passing.
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void alarm_Triggered(void){
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, RED_LED|GREEN_LED);
    int n1, n2, n3, i;
    Lcd_Write_String("ALARM TRIGGERED!");
    SysCtlDelay(40000);
    Lcd_Write_String("ALARM TRIGGERED!");
    SysCtlDelay(40000);
    update_Trigger_Date_Time();
    ialarm_fired = 0;
    n3 = 0;
    i = 0;
    n2 = HibernateRTCGet();
    istate = 2;
    while(i < itimeout && alarm_armed == TRUE){
        GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, RED_LED|GREEN_LED);
        n1 = HibernateRTCGet();
        if(n3 != n1){
            pwm0_start(1000, 50);
            if( xSemaphoreTake( g_pLCDSemaphore, ( TickType_t ) 10 ) == pdTRUE ) // if the user is not inputing the PIN display countdown
            {
            n3 = n1 - n2;
            Lcd_Clear();
            Lcd_Write_Integer((itimeout-n3));
            xSemaphoreGive(g_pLCDSemaphore);
            }
            SysCtlDelay(500);
            pwm0_stop();
            n3 = n1;
            i++;
        }
    }
    if(alarm_armed == TRUE){ // if alarm still not disarmed
        Lcd_Clear();
        Lcd_Write_Integer(0);
        ialarm_fired = 1;   //flag that says to the State Machine task that the alarm was not disarmed in time
        pwm0_start(1000, 80);
        xSemaphoreTake(g_pAlarmTriggerSemaphore, portMAX_DELAY);
        Lcd_Write_String("ALARM DISARMED2!");
        SysCtlDelay(40000);
    }
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*int SonarTaskInit(void);
 *
 * This function was made to tell the program if the task was created successfully or not.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SonarTaskInit(void){
    if(xTaskCreate(SonarTask, (const portCHAR *)"Sonar",
                   128, NULL, tskIDLE_PRIORITY + 1, NULL) != pdTRUE)
    {
        return(1);
    }
    // Success.
    return(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void SonarTask(void *pvParameters);
 *
 * This function is the task responsible for the sonar, it will never exit until the power is disconnected
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SonarTask(void *pvParameters){
    portTickType ui16LastTime;
    uint32_t ui32SwitchDelay = 25;
    while(1){
        get_value_sonar();
        vTaskDelayUntil(&ui16LastTime, ui32SwitchDelay / portTICK_RATE_MS);
    }
}


void measure_sonar(void){
    uint8_t sense_address = 0x70;
    uint8_t data = 0x51;

    I2CMasterSlaveAddrSet(I2C1_BASE, sense_address, false);

    I2CMasterDataPut(I2C1_BASE, data);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C1_BASE));
    //if(I2CMasterErr()) Lcd_Write_String("ERROR MEASURE SONAR");

    return;
}

void get_value_sonar(void){

    measure_sonar();

    SysCtlDelay(160000);

    int verifica;
    uint8_t sense_address = 0x70;

    I2CMasterSlaveAddrSet(I2C1_BASE, sense_address, true);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START );
    while(I2CMasterBusy(I2C1_BASE));
    if((verifica = (int)I2CMasterDataGet(I2C1_BASE))!=0 || verifica == 2)
        return;

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
    //if(I2CMasterErr()) Lcd_Write_String("ERROR VALUE SONAR");
    int int_dist = (int)I2CMasterDataGet(I2C1_BASE);

    if( xSemaphoreTake( g_pLCDSemaphore, ( TickType_t ) 10 ) == pdTRUE )
    {
        Lcd_Write_String("Distance: ");
        Lcd_Write_Integer2(int_dist);
        Lcd_Write_String2(" cm");
        xSemaphoreGive(g_pLCDSemaphore);
    }

    if(int_dist <= itrigger_distance && alarm_armed == TRUE){
        alarm_Triggered();
    }

    return;
}

void start_I2C(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));

    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);
}




