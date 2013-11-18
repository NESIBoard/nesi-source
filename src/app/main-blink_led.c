#include <nesi.h>

int main(void)
{
    // initialize all modules
    nesi.init();

    while(1)
    {
        blueLed.dutycycle(50);
        redLed.dutycycle(50);
        wait(500);
        blueLed.dutycycle(0);
        redLed.dutycycle(0);
        wait(500);


      }



    return 0;
}
