//*/
// Renegade CAN Data logger and SF Stand message interpreter
// Unfortunately by Dan

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <WireKinetis.h>
#include <SD.h>
#include <SPI.h>
//#include <SensorDefinitions.h>
//#include <SensorClass.h>
//#include <vector>

CAN_message_t message0;
CAN_message_t message1;
elapsedMillis sinceGUIsend;

//loop iterator declarations
int i;
int j;
int k;

//The Pressure calculated
int Pressure;
//stupid bit math
int byte0;
int byte1;
int bitshift;
int value;

//This looks awful, but it creates arrays to store every possible CANID frame's bytes to then retransmit

int CANIDARRAYBytes[2048][8]; //array with positions for all possible CANIDs in 11 bit standard ID format for storing recent messages
bool CANIDARRAYLen[2048];

//array for using in the values converted from GUI
//This exists so that any value that doesn't get a conversion applied still gets sent to the Pi.
//That automates sending the state report frames, anything we didn't convert is passed as is.
//It also sends any frames that happen to not have been in our conversion code incidentally to the Pi for it's low rate logging code to still catch.
//This could be structured as ints instead of 2D array, but then you need to write CAN code to chop up the ints automatically for the sends.
int CANIDARRAYConvertedBytes[2048][8];

int counter = 0;

int MAXSENSORVALUE = 36;

float PtConversion[2048][2];

/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11, pin 7 on Teensy with audio board
 ** MISO - pin 12
 ** CLK - pin 13, pin 14 on Teensy with audio board
 ** CS - pin 4, pin 10 on Teensy with audio board
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */

//*/

//File myFile;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
//const int chipSelect = BUILTIN_SDCARD;

bool teensy_sd_enabled;
SDClass teensy_sd;
String sd_string = "can_log0.bin";

void setup()
{
  Can0.begin(500000);
  Can1.begin(500000);
  //[Sensor ID]
  //[][0] = M Value
  //[][1] = B Value
  // Dome Reg Fuel PT
  PtConversion[79][0] = 0.0186;
  PtConversion[79][1] = -126.67;
  // Dome Reg LOx PT
  PtConversion[80][0] = 0.0185;
  PtConversion[80][1] = -133.36;
  // Fuel Tank PT
  PtConversion[81][0] = 0.0186;
  PtConversion[81][1] = -129.3;
  // LOx Tank PT
  PtConversion[82][0] = 0.0187;
  PtConversion[82][1] = -125.36;
  // Fuel High Press PT
  PtConversion[83][0] = 0.0933;
  PtConversion[83][1] = -638.38;
  // LOx High Press PT
  PtConversion[84][0] = 0.093;
  PtConversion[84][1] = -629.72;
  // MV Pneumatic PT
  PtConversion[78][0] = 0.0186;
  PtConversion[78][1] = -126.56;
  // Chamber PT 1
  PtConversion[56][0] = 0.0185;
  PtConversion[56][1] = -128.88;
  // Chamber PT 2
  PtConversion[55][0] = 0.0186;
  PtConversion[55][1] = -102.94;
  // Fuel Injector PT
  PtConversion[58][0] = 0.0186;
  PtConversion[58][1] = -123.27;
  // Fuel Inlet Prop Side PT
  PtConversion[57][0] = 0.0185;
  PtConversion[57][1] = -125.74;
  // LOx Inlet Prop Side PT
  PtConversion[59][0] = 0.0186;
  PtConversion[59][1] = -128.58;

  ////////////////////SD STUFF
  while (!Serial)
    ; //WARNING, REMOVE BEFORE PRODUCTION

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
      sd_string = "can_log" + String(count) + ".bin";
    }
  }

  ///////////////////////////////////////////////END SD SETUP
}

//SD CARD STUFF GOES HERE, Code is in

void dump_CAN_To_Serial(String filename)
{
  CAN_message_t *can_pointer = new CAN_message_t();
  if (teensy_sd_enabled)
  {
    //open file
    File teensy_file = teensy_sd.open(filename.c_str(), FILE_READ);

    Serial.println("here");
    while (teensy_file.readBytes((char *)can_pointer, sizeof(CAN_message_t)) > 0)
    {
      printf("id: %i, ext_id: %i, len: %i, buf = [", can_pointer->id, can_pointer->flags.extended, can_pointer->len);
      for (int i = 0; i < can_pointer->len; i++)
      {
        printf("%i, ", can_pointer->buf[i]);
      }
      printf("]\n");
    }

    //close and save
    teensy_file.close();
  }
  delete (can_pointer);
}

void loop()
{
  /// CAN read on CAN bus from prop stand/rocket

  //if (Can0.available())
  //{
  if (Can0.read(message0) > 0)
  {
    Serial.println("received message!)");
    if (teensy_sd_enabled)
    {
      //open file
      File can_log = teensy_sd.open(sd_string.c_str(), FILE_WRITE);

      //write to file
      can_log.write(&message0, sizeof(message0));

      //close and save
      can_log.close();

      Serial.println("all the CAN frames in this file:");
      dump_CAN_To_Serial(can_log.name());
    }
  }

  CANIDARRAYLen[message0.id] = message0.len;
  // Serial.print(message0.len);
  // Serial.print(" ");
  // Serial.print(CANIDARRAYLen[message0.id]);
  // Serial.println();
  // Serial.print(message0.id);
  // Serial.println();
  // Serial.print(message0.buf[0]);
  // Serial.println();
  // Serial.print(message0.buf[1]);
  // Serial.println();
  for (int i = 0; i < message0.len; i++)
  {
    CANIDARRAYBytes[message0.id][i] = message0.buf[i];
    CANIDARRAYConvertedBytes[message0.id][i] = message0.buf[i]; //fills second array to swap converted values over in later step
  }

  /// CAN send on CAN bus to GUI, every 100 ms
  if (sinceGUIsend >= 1000)
  {

    //STICK SENSOR CONVERSIONS HERE!!!!!!!! Only do it for GUI refreshes not continuously
    //Use the CANIDARRAYConvertedBytes array to overwrite the converted values onto,
    //that way anything like state reports that we don't convert pass through.
    //output of conversions should break the value back into CAN frame byte format to write to the send array

    for (int i = 0; i < 2048; i++)
    {
      if (CANIDARRAYBytes[i][0] != 0)
      {
        if (PtConversion[i][0] != 0)
        {
          byte0 = CANIDARRAYBytes[i][0];
          byte1 = CANIDARRAYBytes[i][1];
          bitshift = byte0 * 256;
          value = bitshift + byte1;
          Pressure = value * PtConversion[i][0] + PtConversion[i][1];
          CANIDARRAYConvertedBytes[i][0] = (Pressure >> 8) & 0xff;
          CANIDARRAYConvertedBytes[i][1] = Pressure & 0xff;
        }
        Serial.print(value);
        Serial.print("  ");
        Serial.print(byte0);
        Serial.print("  ");
        Serial.print(byte1);
        Serial.print("  ");
        Serial.print(Pressure);
        Serial.print("  ");
        Serial.print(bitshift);
        Serial.println();
      }
    }

    /////BELOW IS THE CANSEND CODE, as long as you pack the converted values into CANIDARRAYConvertedBytes it will handle everything
    for (int j = 0; j < 2048; j++)
    {
      if (CANIDARRAYLen[j] != 0)
      {

        // Serial.print(CANIDARRAYLen[j]);
        // Serial.println();
        // Serial.print(j);
        // Serial.println();

        message1.id = j;
        message1.len = 2; //CANIDARRAYLen[j];
        // Serial.print("Length CANID  ");
        // Serial.print(CANIDARRAYLen[j]);
        // Serial.println();
        // Serial.print("Length of message 1  bef  ");
        // Serial.print(message1.len);
        // Serial.println();
        for (int k = 0; k < message1.len; k++)
        {
          //CANIDARRAYConvertedBytes[j][k] = message1.buf[k];
          message1.buf[k] = CANIDARRAYConvertedBytes[j][k];
          // Serial.print(CANIDARRAYConvertedBytes[j][k]);
          // Serial.println();
        }
        // Serial.print("Length of message 1  after  ");
        // Serial.print(message1.len);
        Can1.write(message1);
      }
    }
    counter = counter + 1;
    // Serial.println();
    // Serial.print("GUI Frame Loop Counter: ");
    // Serial.print(counter);
    // Serial.println();
    sinceGUIsend = 0;
  }
}

//*/