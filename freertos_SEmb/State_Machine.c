/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Project for the subject SEmb, made by Diogo Rodrigues nº94240, Daniel Proanho nº101229, and Miguel Fernandes nº93790
 *
 * This file is considered the main file, contains the State Machine and calls the function to start the system(init_system).
 *
 * In this Program LEDs from the Tiva are turned on depending on where the program is running:
 *               White-> Initialization of the peripherals - Init_System.c
 *               Cyan-> Configuration State - Config_State.c
 *               Yellow-> Active State - Active_State.c
 *               Red-> Input on the Keyboard - various files
 *               Purple-> Beacon State - main.c
 *
 * The State Machine has 3 states:
 *
 * The Configuration State:(state 0) Gets the date and time for the RTC, the PIN too arm/disarm the alarm, the trigger distance
 *                       to trigger the alarm, and the max timeout time that the tasks can have.
 *
 * The Active State:(state 1) In this file there only the calling of the function, a resume from that is:
 *                F-> return to Configuration State.
 *                E-> check date and time.
 *                D-> access file with the alarm triggered dates and times.
 *                C-> to arm or disarm alarm.
 *                When in idle it show the distance of an object to the sensor and if that distance is lower or equal than the trigger distance,
 *                the alarm will trigger the buzzer in 15 seconds if the correct pin is not inputed, while this is happening the
 *                other functions of the Active State are Locked. The input of the PIN is done on the next state(Beacon State)
 *                but the count down and the trigger of the buzzer is done in Active State.
 *
 * The Beacon State:(state 2) It's the State that handles the PIN confirmation after the alarm has been or will be triggered. If the PIN is
 *                correct, this state will stop the buzzer and disarm the alarm. The alarm will stay disarmed until the user arm
 *                it in the Active State by pressing 'C'.
 *
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "System.h" // This header file has everything for this project to work, better explained in System.h file.

int StateMachineTaskInit(void){
    if(xTaskCreate(State_Machine, (const portCHAR *)"State_Machine", 258, NULL, tskIDLE_PRIORITY + 2, NULL) != pdTRUE){
        return(1);
    }
    // Success.
    return(0);
}

void State_Machine(void *pvParameters){

    char ckey;
    int iexit;
    istate = 0;
    alarm_armed = FALSE;

    while(1)
    {

        switch(istate)
        {

            case 0: // Configuration State for more details go to Config_State.c file
                GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
                xSemaphoreTake(g_pLCDSemaphore, portMAX_DELAY);
                Lcd_Write_String("Configuration State");
                SysCtlDelay(300000);
                DateTimeSet();
                iPIN = Set_PIN();
                itrigger_distance = Set_Distance();
                itimeout = Set_Timeout();
                Lcd_Write_Integer(iPIN);
                SysCtlDelay(800000);
                Lcd_Write_Integer(itrigger_distance);
                SysCtlDelay(800000);
                Lcd_Write_Integer(itimeout);
                SysCtlDelay(800000);
                istate = 1;
                break;

            case 1:// Active State , Active State will be a task in the future

                Active_state();

                break;

            case 2:// Beacon State, this state will handle to disarm of the Buzzer and check if the pin is correct

                if( xSemaphoreTake( g_pLCDSemaphore, portMAX_DELAY ) == pdTRUE )
                {
                    Lcd_Write_String("Press to input PIN");
                    SysCtlDelay(40000);
                    xSemaphoreGive(g_pLCDSemaphore);
                }
                iexit = 0;
                while(alarm_armed == TRUE) // checking if PIN is correct, for more details go to Active_State.c
                {
                    if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS)
                    {
                        xSemaphoreTake(g_pLCDSemaphore, portMAX_DELAY);
                        if (check_PIN()==1){
                            iexit = 1;
                        }else{
                            Lcd_Write_String("ALARM STILL ARMED");
                            SysCtlDelay(20000);
                        }
                        xSemaphoreGive(g_pLCDSemaphore);
                    }
                    if(iexit == 1) break;
                }
                pwm0_stop(); // this line will stop the Buzzer if it's on, for more details go to PWM.c
                Lcd_Write_String("ALARM DISARMED1!");// A similar line is in alarm_Triggered() to see if both get out of the alarm trigger "state"
                SysCtlDelay(40000);
                alarm_armed = FALSE;// This line is a flag for the Unblocking of the Active State
                istate = 1;  // Returning to Active State when the task are separated this line should be deleted
                if(ialarm_fired == 1)
                {
                    xSemaphoreGive(g_pAlarmTriggerSemaphore);
                }
                break;

        default:
                Lcd_Write_String("ERROR IN DEFAULT"); // Mandatory default, to check error in the State Machine, in case of ERROR return to
                SysCtlDelay(500000);                  // Active State
                istate = 1;
        }
    }
}
