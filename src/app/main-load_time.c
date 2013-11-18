#include <nesi.h>

void blink(int dc)
{
    blueLed.dutycycle(dc);
    redLed.dutycycle(dc);
    wait(500);
    blueLed.dutycycle(0);
    redLed.dutycycle(0);
    wait(500);
}

int main(void)
{
    // initialize all modules
    nesi.init();

    FileStream timeFile = NullFile;
    Byte setTime[18] = {0};
    DateAndTime dateAndTime = newDateAndTime();

    if(sdcard.isOk())
    {
        timeFile = getFileStream();
        if(timeFile.open)
        {
            timeFile.open("time.txt");
            int count = timeFile.read(setTime, 17);
            if(count == 17)
                dateAndTime = timeStampToDateAndTime(setTime);
            else
                dateAndTime = timeStampToDateAndTime("13.10.02-06:28:53");
            timeFile.close();
        }
    }

    setSystemDateAndTime(dateAndTime);

#if 0
    RTCCRegister rtcc = {0};
    DateAndTime rtcc = newDateAndTime();
    rtcc.tensOfYears = 1;
    rtcc.years = 3;
    rtcc.tensOfMonths = 1;
    rtcc.months = 0;
    rtcc.tensOfDays = 0;
    rtcc.days = 2;
    rtcc.tensOfHours = 0;
    rtcc.hours = 5;
    rtcc.tensOfMinutes = 0;
    rtcc.minutes = 56;
    rtcc.tensOfSeconds = 0;
    rtcc.seconds = 2;

    setSystemDateAndTime(rtcc);
    setRTCCRegister(rtcc);
#endif




#if 0
    int dc = 100;

    blink(dc);

    if(sdcard.isOk())
        dataLog.add("test = ", 7);
    else
        dc = 10;
#endif

    usb.connect();

    while(1)
    {
        blink(20);

        if(timeFile.open && button.getStroke() == KEYSTROKE_SINGLE)
        {
            redLed.dutycycle(100);
            blueLed.dutycycle(0);

            timeFile.open("time.txt");
            timeFile.read(setTime, 0);
            timeFile.write(getTimeStamp(), 18);
            timeFile.close();

            redLed.dutycycle(0);
        }
    }

    return 0;
}
