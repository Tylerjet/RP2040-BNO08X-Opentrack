#include <arduino.h>
#include "../.pio/libdeps/pico/SparkFun BNO08x Cortex Based IMU/src/SparkFun_BNO08x_Arduino_Library.h"
#include "../.pio/libdeps/pico/Adafruit TinyUSB Library/src/Adafruit_TinyUSB.h"

BNO08x myIMU;

#define BNO08X_CS 1
#define BNO08X_INT 5
#define BNO08X_RST 6

int xAxis_ = 0;
int yAxis_ = 0;
int rzAxis_ = 0;

float roll = 0;
float yaw = 0;
float pitch = 0;

const int smoothing = 1023;  // used to map roll/pitch/yaw to an integer value

// Define a TinyUSB HID joystick object
Adafruit_USBD_HID usb_hid;

// USB HID report descriptor for a gamepad
uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_GAMEPAD()
};

// Report payload defined in src/class/hid/hid.h
// - For Gamepad Button Bit Mask see  hid_gamepad_button_bm_t
// - For Gamepad Hat    Bit Mask see  hid_gamepad_hat_t
hid_gamepad_report_t gp;

void setReports(void) {
  Serial.println("Setting desired reports");
  if (myIMU.enableGameRotationVector(1) == true) {
    Serial.println(F("Game Rotation vector enabled"));
    Serial.println(F("Output in form i, j, k, real, accuracy"));
  } else {
    Serial.println("Could not enable rotation vector");
  }
  delay(100);  // Allow enough time for BNO080 to accept the new configuration
}

void setup() {

  if (!TinyUSBDevice.isInitialized()) {
    TinyUSBDevice.begin(0);
  }

  Serial.begin(115200);

  if (myIMU.beginSPI(BNO08X_CS, BNO08X_INT, BNO08X_RST) == false) {
    Serial.println("BNO08x not detected. Check your jumpers and the hookup guide. Freezing...");
    while (1)
      ;
  }

  // Setup HID
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.begin();

  // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  if (myIMU.setCalibrationConfig(SH2_CAL_ACCEL || SH2_CAL_GYRO || SH2_CAL_MAG) == true) {  // all three sensors
    Serial.println(F("Calibration Command Sent Successfully"));
  } else {
    Serial.println("Could not send Calibration Command. Freezing...");
    while (1) delay(10);
  }

  setReports();

  // Set initial report to Center
  gp.x = 0;                                // Center x-axis
  gp.y = 0;                                // Center y-axis
  gp.rz = 0;                               // Center rz-axis
  usb_hid.sendReport(0, &gp, sizeof(gp));  // Send centered report
}

void loop() {
#ifdef TINYUSB_NEED_POLLING_TASK
  // Manual call tud_task since it isn't called by Core's background
  TinyUSBDevice.task();
#endif
  delay(10);
  if (myIMU.wasReset()) {
    Serial.print("Sensor was reset ");
    setReports();
  }

  if (myIMU.getSensorEvent() == true) {
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_GAME_ROTATION_VECTOR) {
      // Get raw values from the sensor
      roll = (myIMU.getRoll()) * 180.0 / PI;    // Convert roll to degrees
      pitch = (myIMU.getPitch()) * 180.0 / PI;  // Convert pitch to degrees
      yaw = (myIMU.getYaw()) * 180.0 / PI;      // Convert yaw / heading to degrees

      // Convert roll/pitch/yaw into integers for sending to joystick axis values
      xAxis_ = map(yaw, -180, 180, -254, 254);
      yAxis_ = map(pitch, -180, 180, -127, 127);
      rzAxis_ = map(roll, -75, 75, -127, 127);

      // Prepare joystick report
      gp.x = xAxis_;    // x-axis
      gp.y = yAxis_;    // y-axis
      gp.rz = rzAxis_;  // rz-axis

      // Debug output
      Serial.print(roll, 1);
      Serial.print(F(","));
      Serial.print(pitch, 1);
      Serial.print(F(","));
      Serial.print(yaw, 1);
      Serial.println();

      // Send the joystick HID report
      usb_hid.sendReport(0, &gp, sizeof(gp));

      delay(10);
    }
  }
}
