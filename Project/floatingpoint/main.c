#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/fpu.h"

void UARTInit(void) {
    // Enable UART0 and GPIOA peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure GPIO pins for UART0 (PA0 -> RX, PA1 -> TX)
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure UART0 for 115200 baud rate, 8N1 format
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

void UARTSend(const char *str) {
    while (*str) {
        UARTCharPut(UART0_BASE, *str++);
    }
}

int main(void) {
    // Set system clock to 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Enable FPU
    FPUEnable();
    FPULazyStackingEnable();

    // Initialize UART
    UARTInit();

    // Variables for floating-point operations
    float a = 10.5;
    float b = 5.2;
    float sum, diff, product, quotient;

    // Perform floating-point calculations
    sum = a + b;
    diff = a - b;
    product = a * b;
    quotient = a / b;

    // Send results over UART
    UARTSend("Floating Point Calculation Results:\r\n");

    char buffer[50];
    sprintf(buffer, "Sum: %.2f\r\n", sum);
    UARTSend(buffer);
    sprintf(buffer, "Difference: %.2f\r\n", diff);
    UARTSend(buffer);
    sprintf(buffer, "Product: %.2f\r\n", product);
    UARTSend(buffer);
    sprintf(buffer, "Quotient: %.2f\r\n", quotient);
    UARTSend(buffer);

    while (1) {
        // Infinite loop
    }
}
