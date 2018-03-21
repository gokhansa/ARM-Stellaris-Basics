// Appoýntment management application for room avaýlability


// Interrupt Handler function - Timer break
void
Timer0IntHandler(void)
{
    //
    // Clear the timer interrupt.
    //
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Turn back on the green light
    //
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_3, 9);	
    
    //
    // Update the interrupt status on the display.
    //
    ROM_IntMasterDisable();
    UARTprintf("\rT1: %d  T2: %d", HWREGBITW(&g_ulFlags, 0) ? 1 : 0,
               HWREGBITW(&g_ulFlags, 1) ? 1 : 0);
    ROM_IntMasterEnable();
}

// Randevu Ayarla butonunu kontrol eden ana fonksiyon (Default Kodlar Hariç)
int
main(void)
{
     unsigned char button_val;
     unsigned char Delta;
     unsigned char Rawstate;

    // Initialize the buttons
    ButtonsInit();

    // Configure the two 32-bit periodic timers.
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() * 10);

    // Set the default color to green 
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_3, 9);
		
    // Loop forever while the timers run.
    while(1)
    {			
			button_val = ButtonsPoll(&Delta, &Rawstate);

			if(BUTTON_PRESSED(LEFT_BUTTON,button_val,Delta))
{
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_3, 9);
				ROM_TimerEnable(TIMER0_BASE, TIMER_A);
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_3, 3);
			}
			
    }
}
