/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Project for the subject SEmb, made by Diogo Rodrigues nº94240, Daniel Proanho nº101229, and Miguel Fernandes nº93790
 *
 * This file is the Config_State.c, this file was created to keep the main.c file clean and short.
 *
 * In this file we have DateTimeSet(), Get_Date(), Get_Time(), Set_PIN, Set_Distance, Set_Timeout.
 *
 * A more detailed description of the functions will be in the overhead of the functions.
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "System.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* void DateTimeSet(void);
 *
 * this function is calling Get_Date() and Get_Time() to get current date and time inputed by the user and applied in the time global struct.
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTimeSet(void)
{
    uint32_t ui32Year, ui32Month, ui32Day, ui32Hour, ui32Min, ui32Sec;

    Get_Date(&ui32Year, &ui32Month, &ui32Day);
    Get_Time(&ui32Hour, &ui32Min, &ui32Sec);

    sTime.tm_sec = ui32Sec;
    sTime.tm_min = ui32Min;
    sTime.tm_hour = ui32Hour;
    sTime.tm_mday = ui32Day;
    sTime.tm_mon = ui32Month;
    sTime.tm_year = 100 + ui32Year;

    HibernateRTCSet(0);

    Lcd_Clear();
    Lcd_Write_Integer2(ui32Year);
    Lcd_Write_Char('-');
    Lcd_Write_Integer2(ui32Month);
    Lcd_Write_Char('-');
    Lcd_Write_Integer2(ui32Day);
    Lcd_Write_Char(' ');
    Lcd_Write_Integer2(ui32Hour);
    Lcd_Write_Char(':');
    Lcd_Write_Integer2(ui32Min);
    Lcd_Write_Char(':');
    Lcd_Write_Integer2(ui32Sec);

    SysCtlDelay(1000000);

    return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* void Get_Date(uint32_t *ui32Year, uint32_t *ui32Month, uint32_t *ui32Day);
 *
 * This function will get the current date by translating the char inputs of the keyboard to int values that will be set in the tm Time
 * struct with ui32Year, ui32Month, ui32Day.
 *
 * It asks the user to input data in the following format "DD-MM-YY", and then waits for the 'A' button to be pressed which is the
 * confirmation key, if the user has done a mistake it can press the 'B' button to repeat from the beginning. Only valid dates will be
 * accepted and if any letter excluding A or B is pressed the function repeat from the beginning to handle that error.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Get_Date(uint32_t *ui32Year, uint32_t *ui32Month, uint32_t *ui32Day){
    int i=0;
    char ckey;
    char cDateArray[7];
    char cTemp[3];
    Lcd_Write_String("Enter date, B resets");
    SysCtlDelay(300000);
    Lcd_Write_String("DD-MM-YY ");
    while(1){
        while(1)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
            if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS && i < 6)
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, RED_LED);

                if(ckey == 'A'){
                    i++;
                    break;
                }

                if(ckey == 'C' || ckey == 'D' || ckey == 'E' || ckey == 'F'){
                    Lcd_Write_String("A letter was inputed");
                    SysCtlDelay(300000);
                    Lcd_Write_String("Reseting");
                    SysCtlDelay(300000);
                    ckey = 'B';
                }

                cDateArray[i] = ckey;
                Lcd_Write_Char(ckey);

                if(i == 1|| i == 3) Lcd_Write_Char('-');

                i++;
            }
            if(ckey == 'A'){
                i++;
                break;
            }
            if(ckey == 'B')
            {
                for ( i = 0; i < 7; ++i)
                    cDateArray[i] = 0;
                i=0;
                Lcd_Write_String("DD-MM-YY ");
            }
            ckey = 'n';
        }
        ckey = 'n';
        cTemp[0] = cDateArray[0];
        cTemp[1] = cDateArray[1];
        *ui32Day = atoi(cTemp);

        cTemp[0] = cDateArray[2];
        cTemp[1] = cDateArray[3];
        *ui32Month = atoi(cTemp);

        cTemp[0] = cDateArray[4];
        cTemp[1] = cDateArray[5];
        *ui32Year = atoi(cTemp);

        if(*ui32Day < 31 && *ui32Month < 12 && *ui32Year <99){
            break;
        }else{
            Lcd_Write_String("Input date is not");
            SysCtlDelay(300000);
            Lcd_Write_String("VALID");
            SysCtlDelay(300000);
            Lcd_Write_String("Input date again");
            SysCtlDelay(300000);
            for ( i = 0; i < 7; ++i)
                cDateArray[i] = 0;
            i=0;
            Lcd_Write_String("DD-MM-YY ");
        }
    }
    for ( i = 0; i < 8; ++i)
        cDateArray[i] = 0;

    return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* void Get_Time(uint32_t *ui32Hour, uint32_t *ui32Min, uint32_t *ui32Sec);
 *
 * This function will get the current time by translating the char inputs of the keyboard to int values that will be set in the tm Time
 * strut with ui32Hour, ui32Min, ui32Sec.
 *
 * It asks the user to input data in the following format "hh:mm:ss", and then waits for the 'A' button to be pressed which is the
 * confirmation key, if the user has done a mistake it can press the 'B' button to repeat from the beginning. Only valid times will be
 * accepted and if any letter excluding A or B is pressed the function repeat from the beginning to handle that error.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Get_Time(uint32_t *ui32Hour, uint32_t *ui32Min, uint32_t *ui32Sec){
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
    int i=0;
    char ckey;
    char cTimeArray[7];
    char cTemp[3];
    Lcd_Write_String("Enter time, B resets");
    SysCtlDelay(300000);
    Lcd_Write_String("hh:mm:ss ");
    while(1){
        while(1)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
            if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS && i < 7)
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, RED_LED);

                if(ckey == 'A'){
                   i++;
                   break;
                }

                if(ckey == 'C' || ckey == 'D' || ckey == 'E' || ckey == 'F'){
                    Lcd_Write_String("A letter was inputed");
                    SysCtlDelay(300000);
                    Lcd_Write_String("Reseting");
                    SysCtlDelay(300000);
                    ckey = 'B';
                }

                cTimeArray[i] = ckey;
                Lcd_Write_Char(ckey);

                if(i == 1|| i == 3) Lcd_Write_Char(':');
                i++;
            }
            if(ckey == 'A'){
                i++;
                break;
            }
            if(ckey == 'B')
            {
                for ( i = 0; i < 7; ++i)
                    cTimeArray[i] = 0;
                i=0;
                Lcd_Write_String("hh:mm:ss ");
            }
            ckey = 'n';
        }
        ckey = 'n';

        cTemp[0] = cTimeArray[0];
        cTemp[1] = cTimeArray[1];
        *ui32Hour = atoi(cTemp);

        cTemp[0] = cTimeArray[2];
        cTemp[1] = cTimeArray[3];
        *ui32Min = atoi(cTemp);

        cTemp[0] = cTimeArray[4];
        cTemp[1] = cTimeArray[5];
        *ui32Sec = atoi(cTemp);

        if(*ui32Hour < 24 && *ui32Min < 60 && *ui32Sec <60){
            break;
        }else{
            Lcd_Write_String("Input time is not");
            SysCtlDelay(300000);
            Lcd_Write_String("VALID");
            SysCtlDelay(300000);
            Lcd_Write_String("Input time again");
            SysCtlDelay(300000);
            for ( i = 0; i < 7; ++i)
                cTimeArray[i] = 0;
            i=0;
            Lcd_Write_String("hh:mm:ss ");
        }
    }
    for ( i = 0; i < 8; ++i)
        cTimeArray[i] = 0;

    return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* int Set_PIN(void);
 *
 * This function will get the PIN by translating the char inputs of the keyboard to a int value that will be stored in main.c
 *
 * It asks the user to input data in the following format "****", and then waits for the 'A' button to be pressed which is the
 * confirmation key, if the user has done a mistake it can press the 'B' button to repeat from the beginning.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Set_PIN(void){
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
    int i=0;
    char ckey;
    char cPIN[4];
    Lcd_Write_String("Enter PIN, B resets");
    SysCtlDelay(300000);
    Lcd_Write_String("****, ");
    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
        if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS && i < 4)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, RED_LED);

            if(ckey == 'A'){
               i++;
               break;
            }

            if(ckey == 'C' || ckey == 'D' || ckey == 'E' || ckey == 'F'){
                Lcd_Write_String("A letter was inputed");
                SysCtlDelay(300000);
                Lcd_Write_String("Reseting");
                SysCtlDelay(300000);
                ckey = 'B';
            }

            cPIN[i] = ckey;
            Lcd_Write_Char(ckey);

            i++;
        }
        if(ckey == 'A'){
             i++;
             break;
        }
        if(ckey == 'B')
        {
             for (i = 0; i < 4; ++i)
                 cPIN[i] = 0;
             i=0;
             Lcd_Write_String("****, ");
        }
        ckey = 'n';
    }
    ckey = 'n';
    int intPIN = atoi(cPIN);

    for ( i = 0; i < 5; ++i)
        cPIN[i] = 0;

    return intPIN;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* int Set_Distance(void);
 *
 * This function will get the alarm triggering distance by translating the char inputs of the keyboard to a int value that will be stored
 * in main.c
 *
 * It asks the user to input data in the following format "***", and then waits for the 'A' button to be pressed which is the
 * confirmation key, if the user has done a mistake it can press the 'B' button to repeat from the beginning.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Set_Distance(void){
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
    int i=0;
    char ckey;
    char cdistance[3];
    Lcd_Write_String("Enter trigger distan");
    SysCtlDelay(300000);
    Lcd_Write_String("in *** cm, ");
    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
        if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS && i < 3)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, RED_LED);

            if(ckey == 'A'){
               i++;
               break;
            }

            if(ckey == 'C' || ckey == 'D' || ckey == 'E' || ckey == 'F'){
                Lcd_Write_String("A letter was inputed");
                SysCtlDelay(300000);
                Lcd_Write_String("Reseting");
                SysCtlDelay(300000);
                ckey = 'B';
            }

            cdistance[i] = ckey;
            Lcd_Write_Char(ckey);

            i++;
         }
         if(ckey == 'A'){
             i++;
             break;
         }
         if(ckey == 'B')
         {
             for ( i = 0; i < 3; ++i)
                 cdistance[i] = 0;
             i=0;
             Lcd_Write_String("in *** cm, ");
         }
         ckey = 'n';
      }
      ckey = 'n';
      int intDistance = atoi(cdistance);

      for ( i = 0; i < 4; ++i)
          cdistance[i] = 0;

      return intDistance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* int Set_Timeout(void);
 *
 * This function will get the tasks timeout time by translating the char inputs of the keyboard to a int value that will be stored
 * in main.c
 *
 * It asks the user to input data in the following format "****", and then waits for the 'A' button to be pressed which is the
 * confirmation key, if the user has done a mistake it can press the 'B' button to repeat from the beginning.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Set_Timeout(void){
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
    int i=0;
    char ckey;
    char ctimeout[2];
    Lcd_Write_String("Enter timeout time");
    SysCtlDelay(300000);
    Lcd_Write_String("in ** s, ");
    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|BLUE_LED);
        if(xQueueReceive(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 10) == pdPASS && i < 2)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, RED_LED);

            if(ckey == 'A'){
               i++;
               break;
            }

            if(ckey == 'C' || ckey == 'D' || ckey == 'E' || ckey == 'F'){
                Lcd_Write_String("A letter was inputed");
                SysCtlDelay(300000);
                Lcd_Write_String("Reseting");
                SysCtlDelay(300000);
                ckey = 'B';
            }

            ctimeout[i] = ckey;
            Lcd_Write_Char(ckey);

            i++;
         }
         if(ckey == 'A'){
             i++;
             break;
         }
         if(ckey == 'B')
         {
             for ( i = 0; i < 2; ++i)
                 ctimeout[i] = 0;
             i=0;
             Lcd_Write_String("in ** s, ");
         }
         ckey = 'n';
      }
      ckey = 'n';
      int intTimeout = atoi(ctimeout);

      for ( i = 0; i < 3; ++i)
          ctimeout[i] = 0;

      return intTimeout;
}


