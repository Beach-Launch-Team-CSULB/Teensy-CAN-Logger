/*/

#include <Arduino.h>
#include <SD.h>

//Device enable values- checks if device is enabled, skips trying to talk to it if it is not true
bool teensy_sd_enabled;

SDClass teensy_sd;

//Sets initial value of device strings
String sd_string = "output0.csv";


void setup2()
{

    while (!Serial)
        ;
    Serial.print("Start logging\n");
    
    //Initializes SD card- uses BUILTIN_SDCARD to use SDIO with the Teensy's internal SD card slot
    teensy_sd_enabled = teensy_sd.begin(BUILTIN_SDCARD);
    if (!teensy_sd_enabled)
    {
        Serial.print("Teensy SD Init failed!\n\n");
    }
    else
    {
        Serial.print("Teensy SD Initialized!\n\n");
    }

    //Counts filenames upwards until it finds a free one on flash storage devices
    uint_fast32_t count = 0;
    if (teensy_sd_enabled)
    {
        while (teensy_sd.exists(sd_string.c_str()))
        {
            count++;
            sd_string = "output" + String(count) + ".csv";
        }
    }

}

void loop2()
{

    String file_header = "First thing written to file";
    //Checks if a given data point is enabled, writes data point if it is

    Serial.print("teensy_sd_enabled: ");
    Serial.println(teensy_sd_enabled);
    if (teensy_sd_enabled)
    {
        //open file
        File teensy_file = teensy_sd.open(sd_string.c_str(), FILE_WRITE);
        
        //write to file
        teensy_file.println(file_header);
        teensy_file.println("some data");

        //close and save
        teensy_file.close();
    }
    if (teensy_sd_enabled)
    {
        //open file
        File teensy_file = teensy_sd.open(sd_string.c_str(), FILE_READ);
        
        Serial.println("here");
        Serial.println(teensy_file.readString());

        //close and save
        teensy_file.close();
    }
    while (1)
        ;
}
//*/