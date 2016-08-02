#ifndef Diagnostics_h
#define Diagnostics_h

#include <ros/ros.h>
#include <string>
#include <exception>

//struct to hold collected information
struct signalInfo {
  char mac[18];
  char ssid[33];
  int bitrate;
  int level;
  int quality;
  int noise;
};

class Diagnostics {
  
public:
  Diagnostics(std::string);
  ~Diagnostics();
  void publishWarningLogMessage(std::string);
  void publishErrorLogMessage(std::string);
  void publishInfoLogMessage(std::string);
  std::string getHumanFriendlyTime();
  
private:

  // These functions are called on a timer and check for problems with the sensors
  void sensorCheckTimerEventHandler(const ros::TimerEvent&);
  void checkIMU();
  void checkGPS();
  void checkSonar();
  void checkCamera();
  void checkWireless();
  
  bool checkGPSExists();
  bool checkCameraExists();

  // This function checks the rover published name against the simulated rover model files.
  // If the name of this rover does not appear in the models then assume we are a
  // simulated rover. Being a simulated rover means that certain diagnostic checks will
  // be bypassed.
  bool checkIfSimulatedRover();
  
  // Get wireless info
  int getSignalInfo(signalInfo *sigInfo, const char *iwname);

  // Takes the vendor and device IDs and searches the USB busses for a match
  bool checkUSBDeviceExists(uint16_t, uint16_t);
  
  ros::NodeHandle nodeHandle;
  ros::Publisher diagLogPublisher;
  std::string publishedName;

  
  float sensorCheckInterval = 10; // Check sensors every 10 seconds
  ros::Timer sensorCheckTimer;

  // Store some state about the current health of the rover
  bool cameraConnected = true;
  bool GPSConnected = true;
  bool simulated = false;
};

#endif // End Diagnostics_h
