/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Project for the subject SEmb, made by Diogo Rodrigues nº94240, Daniel Proanho nº101229, and Miguel Fernandes nº93790
 *
 * This file is the Active_state.c, this file was created to keep the main.c file clean and short.
 *
 * In this file we have alarm_Triggered(), check_PIN(), Active_state().
 *
 * This file has the possibility to become a task by itself.
 *
 * A more detailed description of the functions will be in the overhead of the functions.
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "System.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* void update_Date_Time(void);
 *
 * This function was made to update the Date and Time when called. We use the seconds since the last update or when it was set up for the
 * calculate of change in the Date and Time values. We divide by the each one for the time of one year, month, etc... when the value is
 * bigger than the accept value (ex one month has a max of 31 days) we subtracted until we can calculate the  accepted value.
 * After this the RTC clock is reseted to count from 0, so since this change has occurred.
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void update_Date_Time(void){
    uint32_t ui32Year, ui32Month, ui32Day, ui32Hour, ui32Min, ui32Sec;
    ui32Day = sTime.tm_mday;
    ui32Month = sTime.tm_mon;
    ui32Year = sTime.tm_year;
    ui32Year = ui32Year - 100;
    ui32Hour = sTime.tm_hour;
    ui32Min = sTime.tm_min;
    ui32Sec = sTime.tm_sec;


    int time = HibernateRTCGet();
    ui32Year = ui32Year + time/961848000;

    if((time/2635200) > 12){
        while((time/2635200) > 12) time = time - 961848000;
    }
    ui32Month = ui32Month + time/2635200;
    if(ui32Month > 12) ui32Month = ui32Month - 12;

    if((time/84600) > 31){
        while((time/84600) > 31) time = time - 2635200;
    }
    ui32Day = ui32Day + time/84600;
    if(ui32Day > 31) ui32Day = ui32Day - 31;

    if((time/3600) > 24){
        while((time/3600) > 24) time = time - 84600;
    }
    ui32Hour = ui32Hour + time/3600;
    if(ui32Hour > 24) ui32Hour = ui32Hour - 24;

    if((time/60) > 60){
    while((time/60) > 60) time = time - 3600;
    }
    ui32Min = ui32Min + time/60;
    if(ui32Min > 60) ui32Min = ui32Min - 60;

    if((time) > 60){
    while((time) > 60) time = time - 60;
    }
    ui32Sec = time;
    if(ui32Sec > 60) ui32Sec = ui32Sec - 60;

    Lcd_Clear();
    Lcd_Write_Integer2(ui32Day);
    Lcd_Write_Char('-');
    Lcd_Write_Integer2(ui32Month);
    Lcd_Write_Char('-');
    Lcd_Write_Integer2(ui32Year);
    Lcd_Write_Char(' ');
    Lcd_Write_Integer2(ui32Hour);
    Lcd_Write_Char(':');
    Lcd_Write_Integer2(ui32Min);
    Lcd_Write_Char(':');
    Lcd_Write_Integer2(ui32Sec);

    SysCtlDelay(1000000);

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* void update_Trigger_Date_Time(void);
 *
 * This function was made to update the Date and Time and to registed as the last time the alarm was triggered when called. We use the
 * seconds since the last update or when it was set up for the calculate of change in the Date and Time values. We divide by the each one
 * seconds for the time of one year, month, etc... when the value is bigger than the accept value (ex. one month has a max of 31 days) we
 * subtracted until we can calculate the  accepted value.
 * After this the RTC clock is reseted to count from 0, so since this change has occurred.
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void update_Trigger_Date_Time(void){
    uint32_t ui32Year, ui32Month, ui32Day, ui32Hour, ui32Min, ui32Sec;
    ui32Day = sTime.tm_mday;
    ui32Month = sTime.tm_mon;
    ui32Year = sTime.tm_year;
    ui32Year = ui32Year - 100;
    ui32Hour = sTime.tm_hour;
    ui32Min = sTime.tm_min;
    ui32Sec = sTime.tm_sec;


    int time = HibernateRTCGet();
    ui32Year = ui32Year + time/961848000;

    if((time/2635200) > 12){
        while((time/2635200) > 12) time = time - 961848000;
    }
    ui32Month = ui32Month + time/2635200;
    if(ui32Month > 12) ui32Month = ui32Month - 12;

    if((time/84600) > 31){
        while((time/84600) > 31) time = time - 2635200;
    }
    ui32Day = ui32Day + time/84600;
    if(ui32Day > 31) ui32Day = ui32Day - 31;

    if((time/3600) > 24){
        while((time/3600) > 24) time = time - 84600;
    }
    ui32Hour = ui32Hour + time/3600;
    if(ui32Hour > 24) ui32Hour = ui32Hour - 24;

    if((time/60) > 60){
    while((time/60) > 60) time = time - 3600;
    }
    ui32Min = ui32Min + time/60;
    if(ui32Min > 60) ui32Min = ui32Min - 60;

    if((time) > 60){
    while((time) > 60) time = time - 60;
    }
    ui32Sec = time;
    if(ui32Sec > 60) ui32Sec = ui32Sec - 60;

    sTrigger.tm_sec = ui32Sec;
    sTrigger.tm_min = ui32Min;
    sTrigger.tm_hour = ui32Hour;
    sTrigger.tm_mday = ui32Day;
    sTrigger.tm_mon = ui32Month;
    sTrigger.tm_year = 100 + ui32Year;

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* void show_Trigger_Date_Time(void)
 *
 * This function shows the last time the alarm was triggered by accessing the tm sTrigger struct.
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void show_Trigger_Date_Time(void){
    uint32_t ui32Year, ui32Month, ui32Day, ui32Hour, ui32Min, ui32Sec;
    ui32Day = sTrigger.tm_mday;
    ui32Month = sTrigger.tm_mon;
    ui32Year = sTrigger.tm_year;
    ui32Year = ui32Year - 100;
    ui32Hour = sTrigger.tm_hour;
    ui32Min = sTrigger.tm_min;
    ui32Sec = sTrigger.tm_sec;

    Lcd_Clear();
    Lcd_Write_Integer2(ui32Day);
    Lcd_Write_Char('-');
    Lcd_Write_Integer2(ui32Month);
    Lcd_Write_Char('-');
    Lcd_Write_Integer2(ui32Year);
    Lcd_Write_Char(' ');
    Lcd_Write_Integer2(ui32Hour);
    Lcd_Write_Char(':');
    Lcd_Write_Integer2(ui32Min);
    Lcd_Write_Char(':');
    Lcd_Write_Integer2(ui32Sec);

    SysCtlDelay(1000000);

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* int check_PIN (void);
 *
 * This function was made since we check the PIN more that once program.
 *
 * We receive the correct iPIN from the main.c and Active_state() and compare with iPIN_Comparator, a int value that was obtain in the same
 * as the value iPIN with Set_PIN().
 *
 * The rest is a cycle to only exit if the iPIN is right or if the user chooses to exit with a flag that says that the user did not get iPIN
 * right.
 *
 * If the iPIN is correct the function will return 1 and if the user gives up it returns 0.
 *
 * The 'A' button is to repeat the input of the iPIN and B is to give up.
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int check_PIN (void){
    char ckey;
    int iPIN_Comparator;
    while(1)
    {
       iPIN_Comparator = Set_PIN();// for more details of this function go to Config_state.c

       if(iPIN == iPIN_Comparator){
           Lcd_Write_String("PIN is correct");
           SysCtlDelay(300000);
           return 1;
       }

       Lcd_Write_String("Wrong PIN");
       SysCtlDelay(300000);
       Lcd_Write_String("B to exit");
       SysCtlDelay(300000);
       Lcd_Write_String("A to repeat");
       SysCtlDelay(300000);

       while(1)
       {
           if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS)
           {
               if(ckey == 'A') break;

               if(ckey == 'B') return 0;
           }
       }

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* void Active_state(void);
 *
 * This function was made to keep main.c cleaner. It is the state 1 of the State Machine it uses the iPIN, alarm_armed,
 * trigger_distance, state from main.c.
 *
 * There is 5 actions that the user can take:
 *      F-> return to Configuration state.
 *      E-> check date and time, we use the Tiva C Hibernation module which works with RTC to get the actual date and time
 *      after the time by seconds it was inputed.
 *      D-> check date and time of last trigger of the alarm, we use an array to store that data and update in case of a
 *      new trigger of the alarm.
 *      C-> to arm or disarm alarm. If the alarm is disarmed(which is by default) it will arm the alarm after the correct
 *      iPIN is inputed, on the otherhand, if the alarm is armed it will disarm it after the correct iPIN is inputed.
 *      After inputed the correct iPIN it will ask to press 'A'to arm or disarm the alarm depending on the already
 *      talked case or to press 'B' to return without changing it s previous state of armed or disarmed.
 *      Idle-> shows the distance of an object to the sensor and if that distance is lower or equal than the trigger distance,
 *      the function alarm_Triggered() will be called and the state value will be changed to 2, so that in main.c the state
 *      Machine enters Beacon state, since the Program Counter is in the function alarm_Triggered() in line 49 the other
 *      functions of the Active state are Locked.
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Active_state(void){
    Lcd_Write_String("Active state");
    SysCtlDelay(100000);

    while(1){
        GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, RED_LED|GREEN_LED);
        char ckey;
        Lcd_Write_String("F to config again");
        SysCtlDelay(700000);
        Lcd_Write_String("E to display time");
        SysCtlDelay(700000);
        Lcd_Write_String("D to display alarm");
        SysCtlDelay(700000);
        Lcd_Write_String("C to arm/disarm");
        SysCtlDelay(700000);
        Lcd_Write_String("B to read again");
        SysCtlDelay(700000);

        xSemaphoreGive(g_pLCDSemaphore);

        while(1)
        {
            if(istate == 2) return;

            if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS)
                break;
        }

        xSemaphoreTake(g_pLCDSemaphore, portMAX_DELAY);

        switch(ckey)
        {
            case 'F':
                if( alarm_armed == FALSE)
                {
                    if (check_PIN()==1){
                        istate = 0;
                        xSemaphoreGive(g_pLCDSemaphore);
                        return;
                    }else{
                        Lcd_Write_String("Config LOCKED");
                        SysCtlDelay(70000);
                    }
                }else{
                    Lcd_Write_String("The alarm is armed");
                    SysCtlDelay(70000);
                    Lcd_Write_String("disarm it for access");
                    SysCtlDelay(70000);
                }
                break;

            case 'E':
                update_Date_Time();
                SysCtlDelay(300000);
                break;

            case 'D':
                show_Trigger_Date_Time();
                SysCtlDelay(300000);
                break;

            case 'C':
                if(alarm_armed == TRUE){

                    Lcd_Write_String("The alarm is armed");
                    SysCtlDelay(300000);
                    Lcd_Write_String("The PIN is needed");
                    SysCtlDelay(300000);
                    Lcd_Write_String("to disarm the alarm");
                    SysCtlDelay(300000);

                    if (check_PIN() == 0){
                        Lcd_Write_String("since PIN was wrong");
                        SysCtlDelay(300000);
                        Lcd_Write_String("exiting function");
                        SysCtlDelay(300000);
                        return;
                    }

                    Lcd_Write_String("B to return");
                    SysCtlDelay(300000);
                    Lcd_Write_String("A to disarm alarm");
                    SysCtlDelay(300000);
                    while(1)
                    {
                        if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS)
                        {
                            if(ckey == 'B') break;

                            if(ckey == 'A'){
                                alarm_armed = FALSE;
                                break;
                            }
                        }
                    }
                }else{

                    Lcd_Write_String("Alarm is disarmed");
                    SysCtlDelay(300000);
                    Lcd_Write_String("The PIN is needed");
                    SysCtlDelay(300000);
                    Lcd_Write_String("to arm the alarm");
                    SysCtlDelay(300000);

                    if (check_PIN() == 0){
                        Lcd_Write_String("since PIN was wrong");
                        SysCtlDelay(300000);
                        Lcd_Write_String("exiting function");
                        SysCtlDelay(300000);
                        return;
                    }

                     Lcd_Write_String("B to return");
                     SysCtlDelay(300000);
                     Lcd_Write_String("A to arm the alarm");
                     SysCtlDelay(300000);
                     while(1)
                     {
                         if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS)
                         {
                             if(ckey == 'B') break;

                             if(ckey == 'A') {
                                 alarm_armed = TRUE;
                                 break;
                             }
                         }
                     }
                }
                break;

            case 'B':

                break;

            default:
                Lcd_Write_String("INVALID KEY PRESSED");
                SysCtlDelay(300000);
                return;
        }
    }
}
