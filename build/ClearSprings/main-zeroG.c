#include <nesi.h>

void greenStatus(boolean state)
{
    _TRISF3 = 0;
    _RF3 = !state;
}
void redStatus(boolean state)
{
    _TRISG9 = 0;
    _RG9 = !state;
}
void longNotifier(boolean state)
{
    redStatus(state);
}
void shortNotifier(boolean state)
{
    greenStatus(state);
}


void longRoutine(void)
{
    // accumulator to keep tract of number of times the long routine is executed
    static int longCount = 0;

    // indicate that the long routine is executing
    longNotifier(ON);

    // add to long routine accumulator
    ++longCount;

    // power down CO2 sensor during power intensive operations
    co2.off();

    // log routine iteration
    dataLog.add("Long routine number: ", longCount);

    // turn lights on
    redLed.dutycycle(50);
    blueLed.dutycycle(50);

    // take a picture
    camera.getPix("test.jpg");

    // turn lights off
    redLed.dutycycle(0);
    blueLed.dutycycle(0);

    // turn on CO2 sensor before reading data from it
    co2.on();

    // take measurements
    unsigned value_Q1 = resistiveSensors.getQ1(10,100); // get 10 samples, 100ms between samples
    unsigned value_Q2 = resistiveSensors.getQ2(10,100); // get 10 samples, 100ms between samples
    unsigned value_Q3 = resistiveSensors.getQ3(10,100); // get 10 samples, 100ms between samples
    unsigned value_Q4 = resistiveSensors.getQ4(10,100); // get 10 samples, 100ms between samples
    unsigned value_co2 = co2.get(10,100);       // get 10 samples, 100ms between samples

    // power down CO2 sensor during power intensive operations
    co2.off();

    // log readings to file
    dataLog.add("Q1 = ", value_Q1);   // log Q1 moisture measurement
    dataLog.add("Q2 = ", value_Q2);   // log Q2 moisture measurement
    dataLog.add("Q3 = ", value_Q3);   // log Q3 moisture measurement
    dataLog.add("Q4 = ", value_Q4);   // log Q4 moisture measurement
    dataLog.add("CO2 = ", value_co2); // log CO2 measurement

    // water plants (1ml of fluid)
    powerDriverA.on();  // solenoid open
    wait(2000);       // wait for 2 seconds (2000 ms)
    powerDriverA.off(); // close solenoid

    // turn lights on
    redLed.dutycycle(50);
    blueLed.dutycycle(50);

    // take a picture
    camera.getPix("test.jpg");

    // turn lights off
    redLed.dutycycle(0);
    blueLed.dutycycle(0);

    // power intensive operations over, continue to keep CO2 sensor warm
    co2.on();

    // indicate completion of long routine
    longNotifier(OFF);
}

void shortRoutine(void)
{
    // accumulator to keep tract of number of times the short routine is executed
    static int shortCount = 0;

    // indicate that the short routine is executing
    shortNotifier(ON);

    // add to short routine accumulator
    ++shortCount;

    // turn on CO2 sensor before reading data from it
    co2.on();

    // take measurements
    unsigned value_Q1 = resistiveSensors.getQ1(10,100); // get 10 samples, 100ms between samples
    unsigned value_Q2 = resistiveSensors.getQ2(10,100); // get 10 samples, 100ms between samples
    unsigned value_Q3 = resistiveSensors.getQ3(10,100); // get 10 samples, 100ms between samples
    unsigned value_Q4 = resistiveSensors.getQ4(10,100); // get 10 samples, 100ms between samples
    unsigned value_co2 = co2.get(10,100);       // get 10 samples, 100ms between samples

    // power down CO2 sensor during power intensive operations
    co2.off();

    // log readings to file
    dataLog.add("Short routine number: ", shortCount); // log routine iteration
    dataLog.add("Q1 = ", value_Q1);   // log Q1 moisture measurement
    dataLog.add("Q2 = ", value_Q2);   // log Q2 moisture measurement
    dataLog.add("Q3 = ", value_Q3);   // log Q3 moisture measurement
    dataLog.add("Q4 = ", value_Q4);   // log Q4 moisture measurement
    dataLog.add("CO2 = ", value_co2); // log CO2 measurement

    // turn lights on
    redLed.dutycycle(50);
    blueLed.dutycycle(50);

    // take a picture
    camera.getPix("test.jpg");

    // turn lights off
    redLed.dutycycle(0);
    blueLed.dutycycle(0);

    // power intensive operations over, continue to keep CO2 sensor warm
    co2.on();

    // indicate completion of short routine
    shortNotifier(OFF);
}

int main(void)
{
    // initialize all used modules
    nesi.init();

    // initialize status LEDs
    greenStatus(OFF);
    redStatus(OFF);

    // begin warming CO2 sensor
    co2.on();

    // connect to host (USB Mass Storage)
    usb.connect();

    Keystroke key = KEYSTROKE_NONE;

    while(1)
    {
        // get keystroke
        key = button.getStroke();

        if(key == KEYSTROKE_SINGLE) // if button is tapped once
        {
            // disable button keystroke detection
            button.disableStroke();

            // disconnect USB SD card (in case routine writes to SD memory)
            usb.eject();

            // execute long routine
            //longRoutine();
            greenStatus(OFF);
            redStatus(ON);
            camera.getPix("test.jpg");
            redStatus(OFF);

            // reconnect USB SD card (to read files)
            usb.connect();

            // enable button keystroke detection
            button.enableStroke();

        }
        if(key == KEYSTROKE_DOUBLE) // if button is double tapped
        {
            // disable button keystroke detection
            button.disableStroke();

            // disconnect USB SD card (in case routine writes to SD memory)
            usb.eject();

            // execute short routine
            //shortRoutine();
            redStatus(OFF);
            greenStatus(ON);
            camera.getPix("test.jpg");
            greenStatus(OFF);

            // reconnect USB SD card (to read files)
            usb.connect();

            // enable button keystroke detection
            button.enableStroke();

        }

        // always process USB
        usb.process();

    }

    return 0;
}
