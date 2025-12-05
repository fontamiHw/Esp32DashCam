#include "appDefaultConfig.h"
#include "appGlobals.h"

// Function to get the selection options string for a given parameter name
String getSelectionOptions(const char* paramName) {
  String config = String(appConfig);
  String searchName = String(paramName) + "~";

  int paramPos = config.indexOf(searchName);
  if (paramPos == -1) {
    return "";  // Parameter not found
  }

  // Find the line containing this parameter
  int lineStart = paramPos;
  while (lineStart > 0 && config.charAt(lineStart - 1) != '\n') {
    lineStart--;
  }

  int lineEnd = config.indexOf('\n', paramPos);
  if (lineEnd == -1) {
    lineEnd = config.length();
  }

  String line = config.substring(lineStart, lineEnd);

  // Parse the line format: paramName~defaultValue~group~S:option1:option2:option3~description
  int tildaCount = 0;
  int lastTildaPos = -1;
  int currentPos = 0;

  // Find the 4th tilda (before the S: part)
  while (currentPos < line.length() && tildaCount < 3) {
    if (line.charAt(currentPos) == '~') {
      tildaCount++;
      if (tildaCount == 3) {
        lastTildaPos = currentPos;
      }
    }
    currentPos++;
  }

  if (lastTildaPos == -1) {
    return "";  // Invalid format
  }

  // Find the next tilda after the S: part
  int nextTildaPos = line.indexOf('~', lastTildaPos + 1);
  if (nextTildaPos == -1) {
    nextTildaPos = line.length();
  }

  String selectionPart = line.substring(lastTildaPos + 1, nextTildaPos);

  // Check if it starts with "S:" (case insensitive)
  if (selectionPart.length() > 2 &&
      (selectionPart.charAt(0) == 'S' || selectionPart.charAt(0) == 's') &&
      selectionPart.charAt(1) == ':') {
    return selectionPart.substring(2);  // Return everything after "S:"
  }

  return "";  // Not a selection parameter
}

// Function to get a specific selection option by index for a given parameter name
String getSelectionOption(const char* paramName, int index) {
  String options = getSelectionOptions(paramName);

  if (options.length() == 0) {
    return "";  // No selection options found
  }

  if (index < 0) {
    return "";  // Invalid index
  }

  // Split the options by ':' and return the option at the specified index
  int currentIndex = 0;
  int startPos = 0;

  while (currentIndex < index && startPos < options.length()) {
    int colonPos = options.indexOf(':', startPos);
    if (colonPos == -1) {
      return "";  // Index out of bounds
    }
    startPos = colonPos + 1;
    currentIndex++;
  }

  if (currentIndex != index) {
    return "";  // Index out of bounds
  }

  // Find the end of the current option
  int endPos = options.indexOf(':', startPos);
  if (endPos == -1) {
    endPos = options.length();  // Last option
  }

  return options.substring(startPos, endPos);
}

/*
Configuration Format Structure

Each line follows this pattern:
parameterName~defaultValue~group~type~description

Field Meanings:
1. Parameter Name - The unique identifier for the setting
2. Default Value - The initial/default value for this parameter
3. Group - Numeric group identifier (organizes parameters into categories)
4. Type - Defines the input type and validation:
   - S:option1:option2:option3 - Selection dropdown with predefined options
   - T - Text input field
   - N - Numeric input field
   - C - Checkbox (boolean)
   - A - Action button
   - na - Not available/hidden parameter
5. Description - Human-readable explanation of what the parameter does


Group Categories (based on the numeric codes):
- Group 0 - Network & WiFi settings
- Group 1 - Motion detection & timelapse
- Group 2 - Communication (SMTP, MQTT, external services)
- Group 3 - Hardware peripherals (sensors, pins, I/O)
- Group 4 - Remote control & motor settings
- Group 5 - Photogrammetry & stepper motor
- Group 6 - Servo control settings
- Group 7 - Audio configuration
- Group 8 - RTSP streaming settings
- Group 9 - Ethernet configuration
- Group 98 - Camera settings
- Group 99 - System/internal settings


// Selection parameter (dropdown)
netMode~0~99~S:WiFi:Ethernet:Eth+AP~Network interface selection
//      ^ ^  ^^ ^                    ^
//      | |  || |                    description
//      | |  || selection options (WiFi, Ethernet, Eth+AP)
//      | |  |group 99 (system)
//      | default value (0 = WiFi)
//      parameter name

// Text input parameter
ST_ip~~0~T~Static IP address
//    ^ ^ ^ ^
//    | | | description
//    | | text input type
//    | group 0 (network)
//    empty default value

// Checkbox parameter
allowAP~1~0~C~Allow simultaneous AP
//      ^ ^ ^ ^
//      | | | description
//      | | checkbox type
//      | group 0 (network)
//      default true (1)

// Numeric parameter
refreshVal~5~2~N~Web page refresh rate (secs)
//         ^ ^ ^ ^
//         | | | description
//         | | numeric input type
//         | group 2 (communication)
//         default value 5

// Action button
AtakePhotos~Start~5~A~Start photogrammetry
//          ^     ^ ^ ^
//          |     | | description
//          |     | action button type
//          |     group 5 (photogrammetry)
//          button label text
*/

/************** default app configuration **************/
const char* appConfig = R"~(
ST_SSID~~99~~na
netMode~0~99~S:WiFi:Ethernet:Eth+AP~Network interface selection
fsPort~21~99~~na
fsServer~~99~~na
ftpUser~~99~~na
fsWd~~99~~na
fsUse~~99~~na
smtp_port~465~99~~na
smtp_server~smtp.gmail.com~99~~na
smtp_login~~99~~na
smtp_email~~99~~na
Auth_Name~~99~~na
useHttps~~99~~na
useSecure~~99~~na
useFtps~~99~~na
extIP~~99~~na
restart~~99~~na
sdLog~0~99~~na
xclkMhz~20~98~~na
ae_level~-2~98~~na
aec~1~98~~na
aec2~1~98~~na
aec_value~204~98~~na
agc~1~98~~na
agc_gain~0~98~~na
autoUpload~0~98~~na
deleteAfter~0~98~~na
awb~1~98~~na
awb_gain~1~98~~na
bpc~1~98~~na
brightness~0~98~~na
colorbar~0~98~~na
contrast~0~98~~na
dcw~1~98~~na
enableMotion~1~98~~na
fps~20~98~~na
framesize~10~98~~na
gainceiling~0~98~~na
hmirror~0~98~~na
lampLevel~0~98~~na
lenc~1~98~~na
lswitch~10~98~~na
micGain~0~98~~na
ampVol~0~98~~na
minf~5~98~~na
motionVal~8~98~~na
quality~12~98~~na
raw_gma~1~98~~na
record~1~98~~na
saturation~0~98~~na
sharpness~0~98~~na
denoise~0~98~~na
special_effect~0~98~~na
timeLapseOn~0~98~~na
timezone~GMT0~98~~na
vflip~0~98~~na
wb_mode~0~98~~na
wpc~1~98~~na
ST_ip~~0~T~Static IP address
ST_gw~~0~T~Router IP address
ST_sn~255.255.255.0~0~T~Router subnet
ST_ns1~~0~T~DNS server
ST_ns2~~0~T~Alt DNS server
AP_Pass~~0~T~AP Password
AP_ip~~0~T~AP IP Address if not 192.168.4.1
AP_sn~~0~T~AP subnet
AP_gw~~0~T~AP gateway
allowAP~1~0~C~Allow simultaneous AP
doGetExtIP~1~0~C~Enable get external IP
wifiTimeoutSecs~30~0~N~WiFi connect timeout (secs)
logType~0~99~N~Output log selection
ntpServer~pool.ntp.org~0~T~NTP Server address
alarmHour~1~2~N~Hour of day for daily actions
refreshVal~5~2~N~Web page refresh rate (secs)
responseTimeoutSecs~10~2~N~Server response timeout (secs)
useUart~0~3~C~Use UART for Auxiliary connection
uartTxdPin~~3~N~UART TX pin
uartRxdPin~~3~N~UART RX pin
tlSecsBetweenFrames~600~1~N~Timelapse interval (secs)
tlDurationMins~720~1~N~Timelapse duration (mins)
tlPlaybackFPS~1~1~N~Timelapse playback FPS
maxFrames~20000~1~N~Max frames in recording
dashCamOn~0~98~~na
moveStartChecks~5~1~N~Checks per second for start motion
moveStopSecs~2~1~N~Non movement to stop recording (secs)
detectMotionFrames~5~1~N~Num changed frames to start motion
detectNightFrames~10~1~N~Min dark frames to indicate night
detectNumBands~10~1~N~Total num of detection bands
detectStartBand~3~1~N~Top band where motion is checked
detectEndBand~8~1~N~Bottom band where motion is checked
detectChangeThreshold~15~1~N~Pixel difference to indicate change
mlUse~0~1~C~Use Machine Learning
mlProbability~0.8~1~N~ML minimum positive probability 0.0 - 1.0
depthColor~0~1~C~Color depth for motion detection: Gray <> RGB
streamVid~0~8~C~Enable NVR Video stream: /sustain?video=1
streamAud~0~8~C~Enable NVR Audio stream: /sustain?audio=1
streamSrt~0~8~C~Enable NVR Subtitle stream: /sustain?srt=1
smtpUse~0~2~C~Enable email sending
smtpMaxEmails~10~2~N~Max daily alerts
sdMinCardFreeSpace~100~2~N~Min free MBytes on SD before action
sdFreeSpaceMode~1~2~S:No Check:Delete oldest:Ftp then delete~Action mode on SD min free
formatIfMountFailed~0~2~C~Format file system on failure
pirUse~0~3~C~Use PIR for detection
lampType~0~3~S:Manual:PIR~How lamp activated
SVactive~0~3~C~Enable servo use
pirPin~~3~N~Pin used for PIR
lampPin~~3~N~Pin used for Lamp
accINT~0~3~S:NO INT:INT1:INT2:INT1+INT2~Interrupt mode on Accellerometer (ADXL345)
accCS~0~3~S:IO4:IO33~Pin used for CS on Accellerometer (ADXL345)
servoPanPin~~6~N~Pin used for Pan Servo
servoTiltPin~~6~N~Pin used for Tilt Servo
ds18b20Pin~~3~N~Pin used for DS18B20 temperature sensor
AudActive~0~3~C~Show audio configuration
micSckPin~-1~7~N~Microphone I2S SCK pin
micSWsPin~-1~7~N~Microphone I2S WS, PDM CLK pin
micSdPin~-1~7~N~Microphone I2S SD, PDM DAT pin
mampBckIo~-1~7~N~Amplifier I2S BCLK (SCK) pin
mampSwsIo~-1~7~N~Amplifier I2S LRCLK (WS) pin
mampSdIo~-1~7~N~Amplifier I2S DIN pin
servoDelay~0~6~N~Delay between each 1 degree change (ms)
servoMinAngle~0~6~N~Set min angle for servo model
servoMaxAngle~180~6~N~Set max angle for servo model
servoMinPulseWidth~544~6~N~Set min pulse width for servo model (usecs)
servoMaxPulseWidth~2400~6~N~Set max pulse width for servo model (usecs)
servoCenter~90~6~N~Angle at which servo centered
voltDivider~2~3~N~Voltage divider resistor ratio
voltLow~3~3~N~Warning level for low voltage
voltInterval~5~3~N~Voltage check interval (mins)
voltPin~~3~N~ADC Pin used for battery voltage
voltUse~0~3~C~Use Voltage check
wakePin~~3~N~Pin used for to wake app from sleep
wakeUse~0~3~C~Deep sleep app during night
mqtt_active~0~2~C~Mqtt enabled
mqtt_broker~~2~T~Mqtt server ip to connect
mqtt_port~1883~2~N~Mqtt server port
mqtt_user~~2~T~Mqtt user name
mqtt_user_Pass~~2~T~Mqtt user password
mqtt_topic_prefix~homeassistant/~2~T~Mqtt topic path prefix
external_heartbeat_active~0~2~C~External Heartbeat Server enabled
external_heartbeat_domain~~2~T~Heartbeat receiver domain or IP (eg. www.espsee.com)
external_heartbeat_uri~~2~T~Heartbeat receiver URI (eg. /heartbeat/)
external_heartbeat_port~443~2~N~Heartbeat receiver port
external_heartbeat_token~~2~T~Heartbeat receiver auth token
usePing~1~0~C~Use ping
teleUse~0~3~C~Use telemetry recording
teleInterval~1~3~N~Telemetry collection interval (secs)
RCactive~0~3~C~Enable remote control
servoSteerPin~~4~N~Pin used for steering servo
motorRevPin~~4~N~Pin used for motor reverse / left track
motorFwdPin~~4~N~Pin used for motor forward / left track
motorRevPinR~~4~N~Pin used for right track reverse
motorFwdPinR~~4~N~Pin used for right track forward
lightsRCpin~~4~N~Pin used for RC lights output
heartbeatRC~5~4~N~RC connection heartbeat time (secs)
AuxIP~~3~T~Send RC / Servo / PG commands to auxiliary IP
stickXpin~~4~N~Pin used for joystick steering
stickYpin~~4~N~Pin used for joystick motor
stickzPushPin~~4~N~Pin used for joystick lights
stickUse~0~4~C~Use joystick
pwmFreq~50~4~N~RC Motor PWM frequency
maxSteerAngle~45~4~N~Max steering angle from straightahead
maxTurnSpeed~50~4~N~Max tracked turn speed differential
maxDutyCycle~100~4~N~Max motor duty cycle % (speed)
minDutyCycle~10~4~N~Min motor duty cycle % (stop)
allowReverse~1~4~C~Reverse motion required
autoControl~1~4~C~Stop motor or center steering if control inactive
waitTime~20~4~N~Min wait (ms) between RC updates to app
tgramUse~0~2~C~Use Telegram Bot
tgramToken~~2~T~Telegram Bot token
tgramChatId~~2~T~Telegram chat identifier
devHub~0~2~C~Show Camera Hub tab
buzzerUse~0~3~C~Use active buzzer
buzzerPin~~3~N~Pin used for active buzzer
buzzerDuration~~3~N~Duration of buzzer sound in secs
stepIN1pin~-1~5~N~Stepper IN1 pin number
stepIN2pin~-1~5~N~Stepper IN2 pin number
stepIN3pin~-1~5~N~Stepper IN3 pin number
stepIN4pin~-1~5~N~Stepper IN4 pin number
PGactive~0~3~C~Enable photogrammetry
numberOfPhotos~20~5~N~Number of photos
RPM~1~5~N~Turntable revolution speed as RPM
gearing~5.7~5~N~Turntable / motor gearing ratio
clockwise~1~5~C~Clockwise turntable if true
timeForFocus~0~5~N~Time allocated to auto focus (secs)
timeForPhoto~2~5~N~Time allocated to take photo (secs)
pinShutter~-1~5~N~Pin connected to camera shutter
pinFocus~-1~5~N~Pin connected to camera focus
extCam~0~5~C~Use external camera
AtakePhotos~Start~5~A~Start photogrammetry
BabortPhotos~Abort~5~A~Abort photogrammetry
relayPin~-1~3~N~Pin to switch relay
relayMode~0~3~S:Manual:Night~How relay activated
relaySwitch~0~3~C~Switch relay off / on
I2Csda~-1~3~N~I2C SDA pin if unshared
I2Cscl~-1~3~N~I2C SCL pin if unshared
RTSP_Name~~8~T~RTSP Auth Username
RTSP_Pass~~8~T~RTSP Auth Password
rtsp00Video~0~8~C~Enable RTSP Video
rtsp01Audio~0~8~C~Enable RTSP Audio
rtsp02Subtitles~0~8~C~Enable RTSP Subtitles
rtsp03Port~554~8~N~RTSP ServerPort
rtsp04VideoPort~5430~8~N~RTSP Video Port
rtsp05AudioPort~5432~8~N~RTSP Audio Port
rtsp06SubtitlesPort~5434~8~N~RTSP Subtitles Port
rtsp07Ip~239.255.0.1~8~T~RTSP Multicast IP
rtsp08MaxC~3~8~N~RTSP Multicast Max Connections
rtsp09TTL~1~8~N~RTSP Multicast Time-to-Live
ethCS~-1~9~N~Ethernet CS pin
ethInt~-1~9~N~Ethernet Interrupt pin
ethRst~-1~9~N~Ethernet Reset pin
ethSclk~-1~9~N~Ethernet SPI clock pin
ethMiso~-1~9~N~Ethernet SPI MISO pin
ethMosi~-1~9~N~Ethernet SPI MOSI pin
)~";
