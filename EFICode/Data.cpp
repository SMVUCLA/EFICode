#include "Controller.h"

#include "Arduino.h"
#include "Constants.h"

void Controller::sendCurrentData() {
  //Sends data on each of our sensors to Serial output
  struct values {
      unsigned long frontPad = 0x80000001;
      unsigned long time;
      unsigned long totalRevs;
      float ECT;
      float IAT;
      float MAP;
      float TPS;
      float AFR;
      float totalPulseTime;
      long RPM;
      unsigned long backPad = 0x80000000;
  } values;

  //TODO: Convert to micros
  values.time = micros();
  values.totalRevs = totalRevolutions;
  values.ECT = ECT;
  values.IAT = IAT;
  values.MAP = MAP;
  values.TPS = TPS;
  values.AFR = AFR;
  values.RPM = (long) RPM;
  values.totalPulseTime = totalPulseTime;
  Serial.write((byte*)&values, 44);
}

void Controller::trySendingData() {
  if (currentlySendingData) {
    if (micros() - lastSerialOutputTime >= minTimePerSampleReported) {
      sendCurrentData();
      lastSerialOutputTime = micros();
    }
  }
}

// OBSOLETE! KEPT FOR REFERENCE!
//void Controller::printEndingData() {
//  unsigned long sumPulse = 0;
//  int pulseUnder3000 = 0;
//  for (int i = 0; i <= 31; i++) {
//    sumPulse += totalPulse[i];
//    if (i < 16) {
//      pulseUnder3000 += totalPulse[i];
//    }
//
//    //TODO: Do this with string interpolation
//    Serial.print(String(i*RPMIncrement) + "-" + String((i+1)*RPMIncrement));
//    Serial.print(": ");
//    Serial.println(totalPulse[i]);
//  }
//  Serial.println("Total     : " + String(sumPulse));
//  Serial.println("Under 3000: " + String(pulseUnder3000));
//  Serial.end();
//}
