
#include "writelcd.h"
#include "inc/tm4c123gh6pm.h"



int main(void)
{

    Lcdilkayar();

    LcdGotoXY(1, 1);
    LcdYaz(85);
    LcdGotoXY(1, 2);
    LcdYaz(84);

    return 0;



}




