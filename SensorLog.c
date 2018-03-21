#include "inc/hw_memmap.h" 
#include "inc/hw_types.h" 
#include "driverlib/debug.h" 
#include "driverlib/sysctl.h" 
#include "driverlib/adc.h" 
#include "driverlib/uart.h"
#include "driverlib/gpio.h" 

#ifdef DEBUG //Error Checking on APIs
void__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif
int main(void)
{
unsigned long ulADC0Value[8];
//We will use Sequencer 0 (having a FIFO Depth of 8). So, 8 Samples are generated.
volatile unsigned long ulTempAvg;
SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
//Configure System Clock to Run with 16Mhz crystal in Main Oscillator. Use PLL (400MHz). Divide by 5 (There is also, a default Divide by 2)
//Hence, Divide by 10. So, Clock Frequency = 400/10 = 40MHz.
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); //Enable ADC0 Peripheral
SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Enable UART Peripheral
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Enable Port A Peripheral
UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
//Configure UART; Baud Rate Set to 115200, WLEN (Word length/No. of Data Bits) set to 8, One Stop Bits, Parity None
GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); //PA0 - U0RX, PA1 = U0TX
UARTFIFOEnable(UART0_BASE); // UART FIFO Buffer Enable
UARTEnable(UART0_BASE); //Enable UART
SysCtlADCSpeedSet(SYSCTL_ADCSPEED_250KSPS); //ADC Sample Rate set to 250 Kilo Samples Per Second
ADCHardwareOversampleConfigure(ADC0_BASE, 64); // Hardware averaging. ( 2, 4, 8 , 16, 32, 64 )
//64 Samples are averaged here i.e, each sample will be a result of 64 averaged samples. Therefore, every result is a result of 64 x 4 = 256 samples.
ADCSequenceDisable(ADC0_BASE, 1); //Before Configuring ADC Sequencer 1, it should be OFF
ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
//ADC Configured so that Processor Triggers the sequence and we want to use highest priority. ADC Sequencer 0 is Used.
ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0); //Sequencer Step 0: Samples Channel PE3
ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1); //Sequencer Step 1: Samples Channel PE2
ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2); //Sequencer Step 2: Samples Channel PE1
ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3); //Sequencer Step 3: Samples Channel PE0
ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_TS); //Sequencer Step 4: Samples Temperature Sensor
ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_TS); //Sequencer Step 5: Samples Temperature Sensor
ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_TS); //Sequencer Step 6: Samples Temperature Sensor
ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
//Final Sequencer Step also Samples and enables Interrupt and we are telling the sequencer that this is the last step
//Configuring all eight steps in the ADC Sequence
ADCSequenceEnable(ADC0_BASE, 0); //Enable ADC Sequence

while(1)
{
ADCIntClear(ADC0_BASE, 0); // Clear ADC Interrupt
ADCProcessorTrigger(ADC0_BASE, 0); // Trigger ADC Interrupt
while(!ADCIntStatus(ADC0_BASE, 0, false)) //Wait for interrupt Status flag to go off
{
}
//Conversion Complete
ADCSequenceDataGet(ADC0_BASE, 0, ulADC0Value); //Grab the Entire FIFO
ulTempAvg = (ulADC0Value[4] + ulADC0Value[5] + ulADC0Value[6] + ulADC0Value[7] + 2)/4;
char x[4], y[4], z[4], w[4], a[4]; //Character arrays for storing ADC Data for TX
a[3] = ulTempAvg%10 + 48;
ulTempAvg = ulTempAvg/10 ;
a[2] = ulTempAvg%10 + 48;
ulTempAvg = ulTempAvg/10 ;
a[1] = ulTempAvg%10 + 48;
a[0] = ulTempAvg/10 + 48;
x[3] = ulADC0Value[0]%10 + 48;
ulADC0Value[0] = ulADC0Value[0]/10 ;
x[2] = ulADC0Value[0]%10 + 48;
ulADC0Value[0] = ulADC0Value[0]/10 ;
x[1] = ulADC0Value[0]%10 + 48;
x[0] = ulADC0Value[0]/10 + 48;
y[3] = ulADC0Value[1]%10 + 48;
ulADC0Value[1] = ulADC0Value[1]/10 ;
y[2] = ulADC0Value[1]%10 + 48;
ulADC0Value[1] = ulADC0Value[1]/10 ;
y[1] = ulADC0Value[1]%10 + 48;
y[0] = ulADC0Value[1]/10 + 48;
z[3] = ulADC0Value[2]%10 + 48;
ulADC0Value[2] = ulADC0Value[2]/10 ;
z[2] = ulADC0Value[2]%10 + 48;
ulADC0Value[2] = ulADC0Value[2]/10 ;
z[1] = ulADC0Value[2]%10 + 48;
z[0] = ulADC0Value[2]/10 + 48;
w[3] = ulADC0Value[3]%10 + 48;
ulADC0Value[3] = ulADC0Value[3]/10 ;
w[2] = ulADC0Value[3]%10 + 48;
ulADC0Value[3] = ulADC0Value[3]/10 ;
w[1] = ulADC0Value[3]%10 + 48;
w[0] = ulADC0Value[3]/10 + 48;
UARTCharPutNonBlocking(UART0_BASE, x[0]); // Put Character in UART Transmit Buffer
UARTCharPutNonBlocking(UART0_BASE, x[1]);
UARTCharPutNonBlocking(UART0_BASE, x[2]);
UARTCharPutNonBlocking(UART0_BASE, x[3]);
UARTCharPutNonBlocking(UART0_BASE, ' ');
SysCtlDelay(10000);
UARTCharPutNonBlocking(UART0_BASE, y[0]);
UARTCharPutNonBlocking(UART0_BASE, y[1]);
UARTCharPutNonBlocking(UART0_BASE, y[2]);
UARTCharPutNonBlocking(UART0_BASE, y[3]);
UARTCharPutNonBlocking(UART0_BASE, ' ');
SysCtlDelay(10000);
UARTCharPutNonBlocking(UART0_BASE, z[0]);
UARTCharPutNonBlocking(UART0_BASE, z[1]);
UARTCharPutNonBlocking(UART0_BASE, z[2]);
UARTCharPutNonBlocking(UART0_BASE, z[3]);
UARTCharPutNonBlocking(UART0_BASE, ' ');
SysCtlDelay(10000);
UARTCharPutNonBlocking(UART0_BASE, w[0]);
UARTCharPutNonBlocking(UART0_BASE, w[1]);
UARTCharPutNonBlocking(UART0_BASE, w[2]);
UARTCharPutNonBlocking(UART0_BASE, w[3]);
UARTCharPutNonBlocking(UART0_BASE, ' ');
SysCtlDelay(10000);
UARTCharPutNonBlocking(UART0_BASE, a[0]);
UARTCharPutNonBlocking(UART0_BASE, a[1]);
UARTCharPutNonBlocking(UART0_BASE, a[2]);
UARTCharPutNonBlocking(UART0_BASE, a[3]);
UARTCharPutNonBlocking(UART0_BASE, ' ');
UARTCharPutNonBlocking(UART0_BASE, '\n');
SysCtlDelay(2000000);
}
}

