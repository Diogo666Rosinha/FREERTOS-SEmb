//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
/*void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );{
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    while(1){}
}*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Project for the subject SEmb, made by Diogo Rodrigues nº94240, Daniel Proanho nº101229, and Miguel Fernandes nº93790
 *
 * This file is considered the main file, contains the task creation and scheduler
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
 *                When in idle it show the distance of an object to the sensor and if that distance is lower or equal than the
 *                trigger distance, the alarm will trigger the buzzer in 15 seconds if the correct pin is not inputed, while
 *                this is happening the other functions of the Active State are Locked. The input of the PIN is done on
 *                the next state(Beacon State) but the count down and the trigger of the buzzer is done in Active State.
 *
 * The Beacon State:(state 2) It's the State that handles the PIN confirmation after the alarm has been or will be triggered. If the PIN is
 *                correct, this state will stop the buzzer and disarm the alarm. The alarm will stay disarmed until the user arm
 *                it in the Active State by pressing 'C'.
 *
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "System.h" // This header file has everything for this project to work, better explained in System.h file.

struct tm sTime, sTrigger;

int istate, iPIN, alarm_armed, itimeout, itrigger_distance, ialarm_fired; // State 0 so the program will start in Configuration State
    // PIN is the variable that will contain or correct PIN, the alarm will start disarmed,

xSemaphoreHandle g_pLCDSemaphore;

xSemaphoreHandle g_pAlarmTriggerSemaphore;

xQueueHandle g_pKEYBOARDQueue;

int main(void)
{
    init_system(); // Function that will iniciate every peripheral of the system, better explain in the Init_System.c file.

    g_pLCDSemaphore = xSemaphoreCreateMutex();

    g_pAlarmTriggerSemaphore = xSemaphoreCreateMutex();

    xSemaphoreTake(g_pLCDSemaphore, portMAX_DELAY);

    xSemaphoreTake(g_pAlarmTriggerSemaphore, portMAX_DELAY);

    Lcd_Write_String("Hello World");// Test the LCD and nice to have.

    SysCtlDelay(300000);

    xSemaphoreGive(g_pLCDSemaphore);

    if(KeyboardTaskInit() != 0)
    {
        while(1){
            Lcd_Write_String("ERROR KEYB INIT");
            SysCtlDelay(700000);
        }
    }

    if(StateMachineTaskInit() != 0)
    {
        while(1){
            Lcd_Write_String("ERROR STATE INIT");
            SysCtlDelay(700000);
        }
    }

    if(SonarTaskInit() != 0)
    {
        while(1){
            Lcd_Write_String("ERROR SONAR INIT");
            SysCtlDelay(700000);
        }
    }

    vTaskStartScheduler();

    while(1)
    {
        Lcd_Write_String("ERROR TASK SCHEDULER");
        SysCtlDelay(700000);
    }
}
