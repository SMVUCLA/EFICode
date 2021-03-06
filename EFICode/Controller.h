#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Constants.h"
#include "NoiseReduced.h"

class Controller {
public:
  // Constructor
  Controller();

  // Updates sensor values.
  bool readSensors();

  // Sends the current sensor values and requested parameter information.
  void sendCurrentData();

  // If the currentlySendingData flag is set and the minTimePerSampleReported
  // has passed since that last transmission, sends out a data sample.
  void trySendingData();

  // Possibly obsolete.
  void printEndingData();

  // Checks the serial read buffer for incoming bytes. If bytes are found, acts
  // according to the documentation specified in EFI Documentation.
  bool getCommand();

  // Increments the counted number of revolutions since the last RPM update.
  // Every 2nd revolution, switches the INJ_Pin to HIGH and sets a timer for
  // the amount of time equal to the most recent calculated pulse time.
  void countRevolution();

  // Updates the RPM reading by dividing the total number of revolutions since
  // the last update by the number of minutes that have passed.
  void updateRPM();

  // Performs 2D interpolation to lookup a pulse time for the given engine conditions.
  void lookupPulseTime();
  double computeThrottleAdjustment();

  // If singleVal is true, determines the pulse time for the specified cell [row][col]
  // of the AFR Table in units of microseconds times kelvin.
  // If singleVal is false, determines the pulse time for every single AFR Table value
  // in the AFR Table. In its current state, this will overwrite any adjustments that
  // the O2 sensor feedback loop has made to the base pulse times.
  void calculateBasePulseTime(bool singleVal, int row, int col);

  // Sets the currentlySendingData flag to true, allowing the controller to report
  // sampled data back to the DAQ system.
  bool startDataTransmission();

  // Sets the currentlySendingData flag to false, preventing the controller from
  // reporting sampled data back to the DAQ system.
  bool stopDataTransmission();

  // Returns true if the engine drops below the minimum RPM to consider the engine running.
  bool detectEngineOff();

  // Checks to see if the engine is on or off. If the engine switches state since the last
  // check, changes parameters accordingly.
  void checkEngineState();

  // Turns the injector on if it is not disabled.
  void pulseOn();

  // Turns the injector off.
  void pulseOff();

  // Attaches the interrupt timer for shutting the injector off and 
  // sets the INJisDisabled flag to false.
  void enableINJ();

  // Detaches the interrupt timer for shutting the injector off and
  // sets the INJisDisabled flag to true.
  void disableINJ();

  int getRPM (long int timePassed, int rev);
  double getTPS();
  double getTemp(int pin);
  double getMAP();
  double getAFR();

  void initializeParameters();

  void AFRFeedback();
  void idleRPMFeedback();
  long interpolate2D(int blrow, int blcol, double x, double y);

  void setStartupModifier();
  void lowerStartupModifier();
  void raiseStartupModifier();
  
private:
  // Has a value of true if the timer3 interrupt is detached from the "pulseOff" function.
  // Also prevents the injector from pulsing on if true.
  bool INJisDisabled;
  bool AFRFeedbackisEnabled;
  bool RPMFeedbackisEnabled;

  int revsPerCalc;
  int revolutions;
  unsigned long totalRevolutions;
  unsigned long startingRevolutions;
  
  unsigned long totalPulseTime;
  unsigned long lastPulse;
  long lastRPMCalcTime;
  long injectorPulseTime;
  int delayCount;
  bool currentlySendingData;
  long int minTimePerSampleReported;
  long lastSerialOutputTime;
  int mapIndex;
  int rpmIndex;
  long prevDeltaRPM;

  double startupModifier;
  double throttleAdjustment;
  unsigned long lastThrottleMeasurementTime;

  int magnetsHit;
  double constModifier;
  
  long RPM;
  long desiredRPM;
  double TPS;
  double DTPS;
  double ECT;
  double IAT;
  double MAP;
  NoiseReduced* MAPAvg;
  double AFR;
  NoiseReduced* AFRVolts;

  double fuelRatioTable[numTableRows][numTableCols] = 
    {
    {14.5,14.8,15.0,15.3,15.3,15.2,14.6,14.3,13.8,13.6},  // minimum pressure
    {14.5,14.8,15.0,15.3,15.3,15.2,14.6,14.3,13.8,13.6},
    {14.5,14.8,15.0,15.3,15.3,15.2,14.6,14.3,13.8,13.6},
    {14.5,14.8,15.0,15.3,16.0,16.0,15.7,14.3,13.8,13.6},
    {14.5,14.8,15.0,15.3,16.0,16.0,15.7,14.3,13.8,13.6},
    {14.5,14.8,15.0,15.3,16.0,16.0,15.7,14.3,13.8,13.6},
    {14.5,14.8,15.0,15.3,15.3,15.2,14.6,14.3,13.8,13.6},
    {14.5,14.8,15.0,15.3,15.3,15.2,14.6,14.3,13.8,13.6},
    {14.5,14.8,15.0,15.3,15.3,15.2,14.6,14.3,13.8,13.6},
    {14.5,14.8,15.0,15.3,15.3,15.2,14.6,14.3,13.8,13.6},
    {14.5,14.8,15.0,15.3,15.3,15.2,14.6,14.3,13.8,13.6}   // maximum pressure
    };
  long injectorBasePulseTimes[numTableRows][numTableCols];

  //double startupModifierTable[numPoints];
};

#endif
