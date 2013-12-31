#include <nesi.h>
#include <string.h>

DateAndTime getTimeFromFile(String filename)
{
    char fileTime[24] = {0};
    FSFILE* timeFile = FSfopen(filename, "r");
    if(!timeFile) return dateTime.new(14,APRIL,15,0,0,0);
    FSfread(fileTime, sizeof(char), 24, timeFile);
    FSfclose(timeFile);

    return dateTime.parseStamp(fileTime);
}

void putTimeToFile(String filename)
{
    FSFILE* timeFile = FSfopen(filename, "w");
    if(!timeFile) return;
    FSfprintf(timeFile, "%s", dateTime.getStamp());
    FSfclose(timeFile);
}

void readTime(void)
{
    //DateAndTime temp = dateTime.get();
    DateAndTime temp = getTimeFromFile("time.txt");

    // open file
    FSFILE* logFile = FSfopen("datalog.txt", "a+"); // r/w append mode
    if(!logFile) return;

    // log time
    FSfprintf(logFile, "\nAt %s\n", dateTime.getStamp());

    if(temp.day)
    {
        // change time
        dateTime.set(temp);

        // log time change
        FSfprintf(logFile, "Time changed to: %s\n", dateTime.getStamp());
    }
    else
        // log error reading time
        FSfprintf(logFile, "Error reading time from file.\n");

    // close file
    FSfclose(logFile);
}

void setLightIntensity(int percentage)
{
    if(percentage < 0)  // negative used for 0 due to compiler scanf bug
        percentage = 0; //   "0" scanned/parsed as 0x00FF as of XC16 v1.20

    ledR.dutycycle(percentage);
    ledB.dutycycle(percentage);

    // open file
    FSFILE* logFile = FSfopen("datalog.txt", "a+"); // r/w append mode
    if(!logFile) return;

    // log time
    FSfprintf(logFile, "\nAt %s\n", dateTime.getStamp());

    // log LED status
    FSfprintf(logFile, "LED brightness set to %d%%\n", percentage);

    // close file
    FSfclose(logFile);
}

void setPump(int state)
{
    state = state > 0; // negative used for 0 due to compiler scanf bug
                       //   "0" scanned/parsed as 0x00FF as of XC16 v1.20
                       // FIXED - use legacy lib...problem with HITEC C lib
    
    powerDriverA.set(state); 

    // open file
    FSFILE* logFile = FSfopen("datalog.txt", "a+"); // r/w append mode
    if(!logFile) return;

    // log time
    FSfprintf(logFile, "\nAt %s\n", dateTime.getStamp());

    // log pump status
    FSfprintf(logFile, "Pump set to %s\n", state ? "OPEN": "CLOSED");

    // close file
    FSfclose(logFile);
}

void collectData(void)
{
    // open file
    FSFILE* logFile = FSfopen("datalog.txt", "a+"); // r/w append mode
    if(!logFile) return;

    // log time
    FSfprintf(logFile, "\nAt %s\n", dateTime.getStamp());

    // read temp
    Uint16 tempMeasurement = resistiveSensors.getQ1(10,1);
    FSfprintf(logFile, "Temperature = %d (out of 1024)\n", tempMeasurement);

    // read CO2
    Uint16 co2Measurement = co2.get(10,2);
    FSfprintf(logFile, "CO2 content = %d (out of 1024)\n", co2Measurement);

    // close file
    FSfclose(logFile);
}

void takePicture(void)
{
    // take image
    char imgName[24] = {'\0'};
    DateAndTime dt = dateTime.get();

    sprintf(imgName, "%02d.%02d.%02d-%02d.%02d.%02d.jpg",
        dt.year, dt.month,  dt.day,
        dt.hour, dt.minute, dt.second);

    int error = camera.getPix(imgName);

    // open file
    FSFILE* logFile = FSfopen("datalog.txt", "a+"); // r/w append mode
    if(!logFile) return;

    // log time
    FSfprintf(logFile, "\nAt %s\n", dateTime.getStamp());

    if(!error)
        // log image
        FSfprintf(logFile, "Image taken = %s\n", imgName);
    else
        // log image
        FSfprintf(logFile, "Error (#%d) taking picture = %s\n", error, imgName);

    // close file
    FSfclose(logFile);
}

// TODO - use type to specify different characters
void printHeader(int type)
{
    // open file
    FSFILE* logFile = FSfopen("datalog.txt", "a+"); // r/w append mode
    if(!logFile) return;

    // print header
    FSfprintf(logFile, "\n====================================\n");

    // close file
    FSfclose(logFile);
}

typedef struct {
    int param;
    char cmd;
    DateAndTime time;
} TimelineEntry;

#define CMD_PENDING    '#'
#define CMD_COMPLETED  '$'
#define CMD_UNSTARTED  '!'

static char exeState = CMD_COMPLETED;

char getExeState(void)
{
    return exeState;
}

TimelineEntry getNextTimelineEntry(void)
{
    TimelineEntry toExe;
    toExe.time = dateTime.new(0,0,0,0,0,0);
    toExe.time.day = 0;
    toExe.cmd = EOF; // assume that there are no commands to execute
    toExe.param = 0;
    char temp;
    FSFILE* fptr = FSfopen("timeline.txt", "r+");
    if(!fptr) return toExe;

    while(FSfread(&temp, sizeof(char), 1, fptr) > 0) // parse char by char
    {
        if(temp == CMD_UNSTARTED) // not started
        {
            usb.eject();
            // overwrite ! with #
            FSfseek(fptr, -1, SEEK_CUR);
            toExe.cmd = CMD_PENDING;
            FSfwrite(&toExe.cmd, sizeof(char), 1, fptr);
            // parse time, command, and parameter
            char line[32];
            FSfread(line, sizeof(char), 25, fptr)[line] = 0; // add NULL
            sscanf(line, "%d.%d.%d-%d:%d:%d %c %d",
                &toExe.time.year, &toExe.time.month, &toExe.time.day,
                &toExe.time.hour, &toExe.time.minute, &toExe.time.second,
                &toExe.cmd, &toExe.param);

            usb.connect();
            break;
        }
        else if(temp == CMD_PENDING) // pending
        {
            // wait until done
            toExe.cmd = CMD_PENDING;
            if(getExeState() == CMD_PENDING)
                break;

            // then overwrite # with $
            usb.eject();
            FSfseek(fptr, -1, SEEK_CUR);
            toExe.cmd = CMD_COMPLETED;
            FSfwrite(&toExe.cmd, sizeof(char), 1, fptr);
            usb.connect();
            break;
        }
        else if(temp == CMD_COMPLETED) // completed
        {
            // skip command
        }
        else if(temp == EOF) // file empty
        {
            toExe.cmd = EOF;
            break;
        }
        else
        {
            // skip character
        }
    }

    FSfclose(fptr);
    return toExe;
}

Boolean exeTimelineEntry(DateAndTime timeout)
{
    static TimelineEntry toExe = {EOF};
        
    if(getExeState() == CMD_COMPLETED)
        toExe = getNextTimelineEntry(); // get next command to execute

    if(toExe.cmd == EOF)
    {
        exeState = CMD_COMPLETED;
        wait(10000);
        return FALSE;
    }

    if(toExe.cmd == CMD_COMPLETED)
    {
        exeState = CMD_COMPLETED;
        return TRUE;
    }

    if(!toExe.time.day)
    {
        exeState = CMD_COMPLETED;
        wait(1000);
        return FALSE;
    }
   
    exeState = CMD_PENDING;
    
    // get max finishing time
    timeout = dateTime.add(dateTime.get(), timeout);

    // check for execution time until timeout occurs
    while(dateTime.cmp(dateTime.get(), toExe.time) < 0)
    {
        if(dateTime.cmp(dateTime.get(), timeout) > 0)
            return FALSE; // return if timeout expires
        usb.process();
    }

    usb.eject();
        
    switch(toExe.cmd)
    {
        case 'L': case 'l':
            setLightIntensity(toExe.param);
            break;
        case 'D': case 'd':
            collectData();
            break;
        case 'F': case 'f':
            setPump(toExe.param);
            break;
        case 'P': case 'p':
            takePicture();
            break;
        case 'T': case 't':
            readTime();
            break;
        case 'H': case 'h':
            printHeader(toExe.param);
            break;
        default:
            break;
    }

    exeState = CMD_COMPLETED;

    usb.attach();

    return TRUE;
}

int main_old(void)
{
    // initialize all modules
    nesi.init();

    //dateTime.set(getTimeFromFile("time.txt"));
    //putTimeToFile("start.txt");

    FSFILE* logFile = FSfopen("datalog.txt", "a+"); // r/w append mode
    if(logFile) FSfprintf(logFile, "\n******* Starting Log *******\n");
    if(logFile) FSfprintf(logFile, "Time is: %s\n", dateTime.getStamp());

    FSfclose(logFile);

    usb.connect();

    DateAndTime timeout = dateTime.new(0,0,0,0,0,0);
    //timeout.hour = 24; // wait to execute for a maximum of 24 hours
    //timeout.second = 10; // wait to execute for a maximum of 10 seconds

    while(1)
    {
        exeTimelineEntry(timeout);
        usb.process();
    }

    return 0;
}

int main(void)
{
    // initialize all modules
    nesi.init();

    usb.connect();

    char message[128] = {0};

    while(1)
    {
        sprintf(message, "Time is: %s\r\n", dateTime.getStamp());
        usb.print(message);
        wait(1500);
    }

    return 0;
}
