#include "System.h"

void init_system(void){
    // Sets the clock of the Tiva to 4 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    // Enable and waits for RTC module to start, and sets it up with the calendar mode
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_HIBERNATE)){}
    HibernateEnableExpClk(SysCtlClockGet());
    HibernateClockConfig(HIBERNATE_OSC_LOWDRIVE);
    HibernateCounterMode(HIBERNATE_COUNTER_24HR);
    HibernateRTCEnable();
    uint32_t ui32Status;
    ui32Status = HibernateIntStatus(0);
    HibernateIntClear(ui32Status);

    // Enable and wait for the ports to be ready for access
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}

    // Configure the GPIO port for the LED operation.
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, D7|D6|D5|D4|EN|RS);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, R1|R2|R3|R4);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, C1|C2|C3|C4);

    //System, ports and peripherals set up! Now setting I2C, the Buzzer and the Lcd. The keyboard and the sensor doesn't need a setup
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED|RED_LED|BLUE_LED, GREEN_LED|RED_LED|BLUE_LED);
    start_I2C();
    pwm0_init();
    Lcd_Init();
}
