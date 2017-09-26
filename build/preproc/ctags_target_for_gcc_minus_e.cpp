# 1 "D:\\Programmi_Home\\Arduino\\Sketchs\\Sketchs\\ESP8266\\ESP8266\\ESP8266.ino"
# 1 "D:\\Programmi_Home\\Arduino\\Sketchs\\Sketchs\\ESP8266\\ESP8266\\ESP8266.ino"
# 2 "D:\\Programmi_Home\\Arduino\\Sketchs\\Sketchs\\ESP8266\\ESP8266\\ESP8266.ino" 2
# 3 "D:\\Programmi_Home\\Arduino\\Sketchs\\Sketchs\\ESP8266\\ESP8266\\ESP8266.ino" 2

/* TCP server/client example, that manages client connections, checks for messages
 *  when client is connected and parses commands. Connect to the ESP8266 IP using
 * a TCP client such as telnet, eg: telnet 192.168.0.X 2121
 *  
 *  ESP8266 should be AT firmware based on 1.5 SDK or later
 *
 * 2016 - J.Whittington - engineer.john-whittington.co.uk
 */
# 21 "D:\\Programmi_Home\\Arduino\\Sketchs\\Sketchs\\ESP8266\\ESP8266\\ESP8266.ino"
SoftwareSerial swSerial(4 /*  Connect this pin to TX on the esp8266*/, 6 /*  Connect this pin to RX on the esp8266*/);

// the last parameter sets the local echo option for the ESP8266 module..
SerialESP8266wifi wifi(Serial, Serial, 5 /* Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)*/, swSerial);

void processCommand(WifiMessage msg);

uint8_t wifi_started = false;

// TCP Commands
const char RST[] __attribute__((__progmem__)) = "RST";
const char IDN[] __attribute__((__progmem__)) = "*IDN?";

void setup() {

  // start debug serial
  swSerial.begin(9600);
  // start HW serial for ESP8266 (change baud depending on firmware)
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Starting wifi");

  swSerial.println("Starting wifi");
  wifi.setTransportToTCP();// this is also default
  wifi.endSendWithNewline(false); // Will end all transmissions with a newline and carrage return ie println.. default is true

  wifi_started = wifi.begin();
  if (wifi_started) {
    wifi.connectToAP("YourSSID", "YourPassword");
    wifi.startLocalServer("2121");
  } else {
    // ESP8266 isn't working..
  }
}

void loop() {

  static WifiConnection *connections;

  // check connections if the ESP8266 is there
  if (wifi_started)
    wifi.checkConnections(&connections);

  // check for messages if there is a connection
  for (int i = 0; i < 3; i++) {
    if (connections[i].connected) {
      // See if there is a message
      WifiMessage msg = wifi.getIncomingMessage();
      // Check message is there
      if (msg.hasData) {
        // process the command
        processCommand(msg);
      }
    }
  }
}

void processCommand(WifiMessage msg) {
  // return buffer
  char espBuf[128];
  // scanf holders
  int set;
  char str[16];

  // Get command and setting
  sscanf(msg.message,"%15s %d",str,&set);
  /* swSerial.print(str);*/
  /* swSerial.println(set);*/

  if ( !strcmp_P(str,IDN) ) {
    wifi.send(msg.channel,"ESP8266wifi Example");
  }
  // Reset system by temp enable watchdog
  else if ( !strcmp_P(str,RST) ) {
    wifi.send(msg.channel,"SYSTEM RESET...");
    // soft reset by reseting PC
    asm volatile ("  jmp 0");
  }
  // Unknown command
  else {
    wifi.send(msg.channel,"ERR");
  }
}

void my_func() {

}
