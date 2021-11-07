#ifndef SENSORDEFINITIONS_H
#define SENSORDEFINITIONS_H

#include "SensorClass.h"
#include <array>

// define number of sensor here
#define NUM_SENSORS 25

//SENSOR format
// {"name", type of sensor, differential bool (diff read = true), pin number on ADC to read, sample rate - high speed mode, sample rate - low speed mode, node ID, CAN2.0 ID,}

SENSOR pt1 {"DomeRegFuel", pt1000, 0, 21, 10, 1, 3, 24};
SENSOR pt2 {"DomeRegLOX", pt1000, 0, 22, 10, 1, 3, 25};
SENSOR pt3 {"FuelTank", pt1000, 0, 23, 100, 10, 3, 26};
SENSOR pt4 {"LOXTank", pt1000, 0, 24, 100, 10, 3, 27};
SENSOR pt5 {"HiPressFuel", 0, pt5000, 20, 10, 1, 3, 29};
SENSOR pt6 {"HiPressLOX", 0, pt5000, 20, 10, 1, 3, 28};
SENSOR pt7 {"Chamber", 0, pt1000, 0, 1000, 1, 2, 11};
SENSOR pt8 {"Chamber2", 0, pt1000, 0, 1000, 1, 2, 12};
/// SENSOR pt8 {"Chamber3", 0, pt1000, 0, 1000, 1, 2, 12};
SENSOR pt9 {"FuelInletProp", 0, pt1000, 0, 100, 1, 2, 14};
SENSOR pt10 {"FuelInjector", 0, pt1000, 0, 100, 1, 2, 16};
SENSOR pt11 {"LOXInletProp", 0, pt1000, 0, 100, 1, 2, 18};
SENSOR pt12 {"MVPneumatic", 0, pt1000, 0, 10, 1, 3, 30};
SENSOR load_cell1 {"Thrust Load Cell 1", loadcell1000, 0, 1, 100, 10, 2, 19};
SENSOR load_cell2 {"Thrust Load Cell 2", loadcell1000, 0, 2, 100, 10, 2, 20};
SENSOR load_cell3 {"Thrust Load Cell 3", loadcell1000, 0, 3, 100, 10, 2, 21};
SENSOR tc1 {"External Chamber Wall TC", tcktype, 1, 10, 10, 1, 2, 10};
SENSOR tc2 {"LOX Tank Lower TC", tcktype, 1, 12, 1, 1, 2, 9};
/// SENSOR tc2 {"igniter", tcktype, 12, 10, 1, 2, 9};
SENSOR tc3 {"LOX Tank Mid TC", tcktype, 1, 12, 1, 1, 3, 8};
SENSOR tc4 {"LOX Tank Upper TC", tcktype, 1, 12, 1, 1, 3, 7};
SENSOR MCU1 {"MCU Temp Engine Node", mcu, 0, 0, 10, 1, 2, 31};
SENSOR MCU2 {"MCU Temp Upper Prop Node", mcu, 0, 0, 10, 1, 3, 32};
SENSOR MCU3 {"Pad Node", mcu, 0, 0, 10, 1, 4, 33};
SENSOR MCU4 {"TelemetryLogger Node", mcu, 0, 0, 10, 1, 5, 34};
SENSOR RTD1 {"Engine Cold Junction Temp", rtd, 0, 0, 7, 1, 2, 22};
SENSOR RTD2 {"Prop Cold Junction Temp", rtd, 0, 0, 7, 1, 3, 23};

std::array<SENSOR*, NUM_SENSORS> sensorArray{&pt1, &pt2, &pt3, &pt4, &pt5, &pt6, &pt7, &pt8, &pt9, &pt10, &pt11, &pt12, &load_cell1, &load_cell2, &load_cell3, &tc1, &tc2, &tc3, &tc4, &MCU1, &MCU2, &MCU3, &MCU4, &RTD1, &RTD2};




#endif