////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Project for the subject SEmb, made by Diogo Rodrigues nº94240, Daniel Proanho nº101229, and Miguel Fernandes nº93790
 *
 * This file contains all the code concerning the keyboard handling and communication.
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include"System.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*int KeyboardTaskInit(void);
 *
 * This function was made to tell the program if the task was created successfully or not.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int KeyboardTaskInit(void){

    g_pKEYBOARDQueue = xQueueCreate(QUEUE_SIZE, KEY_SIZE);

    if(xTaskCreate(KeyboardTask, (const portCHAR *)"KeyboardTask", 128, NULL, tskIDLE_PRIORITY + 1, NULL) != pdTRUE){
        return(1);
    }
    // Success.
    return(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void KeyboardTask(void *pvParameters);
 *
 * This function is the task responsible for the State Machine, it will never exit until the power is disconnected.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KeyboardTask(void *pvParameters){
    char ckey;
    portTickType ui16LastTime;
    uint32_t ui32SwitchDelay = 25;
    while(1){
        ckey = keypadScan();
        if(ckey != 'n'){
            //send to queue
            if(xQueueSend(g_pKEYBOARDQueue, &ckey, ( TickType_t ) 50 ) != pdPASS)
            {
                // Error. The queue should never be full. If so print the
                Lcd_Write_String("Queue full");
                xQueueReset(g_pKEYBOARDQueue);
                SysCtlDelay(700000);
            }
        }
        vTaskDelayUntil(&ui16LastTime, ui32SwitchDelay / portTICK_RATE_MS);
    }
}
char keypadScan(){
  //Scan 1st row
  //r1 = 1; r2 = 0; r3 = 0; r4 = 0;
  GPIOPinWrite(GPIO_PORTB_BASE, R1|R2|R3|R4, R1);
  // Read columns
  if (GPIOPinRead(GPIO_PORTB_BASE, C1) == C1){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C1) == C1); SysCtlDelay(200000); return '1';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C2) == C2){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C2) == C2); SysCtlDelay(200000); return '2';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C3) == C3){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C3) == C3); SysCtlDelay(200000); return '3';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C4) == C4){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C4) == C4); SysCtlDelay(200000); return 'F';}
  //Scan 2nd row
  //r1 = 0; r2 = 1; r3 = 0; r4 = 0;
  GPIOPinWrite(GPIO_PORTB_BASE, R1|R2|R3|R4, R2);
  // Read columns
  if (GPIOPinRead(GPIO_PORTB_BASE, C1) == C1){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C1) == C1); SysCtlDelay(200000); return '4';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C2) == C2){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C2) == C2); SysCtlDelay(200000); return '5';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C3) == C3){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C3) == C3); SysCtlDelay(200000); return '6';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C4) == C4){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C4) == C4); SysCtlDelay(200000); return 'E';}
  //Scan 3rd row
  //r1 = 0; r2 = 0; r3 = 1; r4 = 0;
  GPIOPinWrite(GPIO_PORTB_BASE, R1|R2|R3|R4, R3);
  // Read columns
  if (GPIOPinRead(GPIO_PORTB_BASE, C1) == C1){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C1) == C1); SysCtlDelay(200000); return '7';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C2) == C2){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C2) == C2); SysCtlDelay(200000); return '8';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C3) == C3){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C3) == C3); SysCtlDelay(200000); return '9';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C4) == C4){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C4) == C4); SysCtlDelay(200000); return 'D';}
  //Scan 4th row
  //r1 = 0; r2 = 0; r3 = 0; r4 = 1;
  GPIOPinWrite(GPIO_PORTB_BASE, R1|R2|R3|R4, R4);
  // Read columns
  if (GPIOPinRead(GPIO_PORTB_BASE, C1) == C1){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C1) == C1); SysCtlDelay(200000); return 'A';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C2) == C2){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C2) == C2); SysCtlDelay(200000); return '0';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C3) == C3){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C3) == C3); SysCtlDelay(200000); return 'B';}
  if (GPIOPinRead(GPIO_PORTB_BASE, C4) == C4){SysCtlDelay(200000); while(GPIOPinRead(GPIO_PORTB_BASE, C4) == C4); SysCtlDelay(200000); return 'C';}
  return 'n'; //n for "not pressed"
  }


