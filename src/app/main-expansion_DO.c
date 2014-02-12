#include <nesi.h>

int main(void)
{
    // initialize all modules
    nesi.init();

 
    while(!button.isPressed())
    {
        expansion.digitalOut(3,1);      //set expansion pin 3 to digital high
        expansion.digitalOut(4,1);      //set expansion pin 4 to digital high
        expansion.digitalOut(5,1);      //set expansion pin 5 to digital high
        expansion.digitalOut(6,1);      //set expansion pin 6 to digital high
        expansion.digitalOut(7,1);      //set expansion pin 7 to digital high
        expansion.digitalOut(8,1);      //set expansion pin 8 to digital high
        expansion.digitalOut(9,1);      //set expansion pin 9 to digital high
        expansion.digitalOut(10,1 );    //set expansion pin 10 to digital high

        ledB.dutycycle(20);             //set blue LED's at 20 percent dutycycle
        ledR.dutycycle(0);              //turn off red LED's
    }
      

    while(button.isPressed())
    {
        expansion.digitalOut(3,0);       //set expansion pin 3 to digital low
        expansion.digitalOut(4,0);       //set expansion pin 4 to digital low
        expansion.digitalOut(5,0);       //set expansion pin 5 to digital low
        expansion.digitalOut(6,0);       //set expansion pin 6 to digital low
        expansion.digitalOut(7,0);       //set expansion pin 7 to digital low
        expansion.digitalOut(8,0);       //set expansion pin 8 to digital low
        expansion.digitalOut(9,0 );      //set expansion pin 9 to digital low
        expansion.digitalOut(10,0 );     //set expansion pin 10 to digital low

        ledR.dutycycle(20);              //set red LED's at 20 percent dutycycle
        ledB.dutycycle(0);               //turn off blue LED's
    }



    return 0;
}
