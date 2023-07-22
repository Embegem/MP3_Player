#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10); // RX, TX

// DFP module commands
#define CMD_NEXT_SONG 0x01
#define CMD_VOLUME_UP 0x04
#define CMD_VOLUME_DOWN 0x05
#define CMD_SET_VOLUME 0x06
#define CMD_SLEEP_MODE 0x0A
#define CMD_WAKE_UP 0x0B

// DFP module parameters
#define DEVICE_NUMBER 0x02 // Change this if you have multiple devices
int x=0;
void sendCommand(uint8_t command, uint16_t parameter = 0) {
  // Command structure: 0x7E FF 06 command feedback parameter EF
  uint16_t checksum = -(DEVICE_NUMBER + command + (parameter >> 8) + (parameter & 0xFF));
  
  mySerial.write(0x7E);
  mySerial.write(0xFF);
  mySerial.write(0x06);
  mySerial.write(command);
  mySerial.write(DEVICE_NUMBER);
  mySerial.write(parameter >> 8);
  mySerial.write(parameter & 0xFF);
  mySerial.write((checksum >> 8) & 0xFF);
  mySerial.write(checksum & 0xFF);
  mySerial.write(0xEF);
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
  delay(1000); // Allow the module to initialize
  // Configure the DFP module
  sendCommand(CMD_SET_VOLUME, 20); // Set volume (0-30)
  sendCommand(CMD_WAKE_UP); // Wake up the module
}

void loop() {
  x= x++;
  Serial.println(x);
  switch(x){
    case 1: 
  playMP3("1.mp3");
  Serial.println("1");
  break;
  case 2:
  playMP3("2.mp3");
  Serial.println("2");
  break;
  case 3:
  playMP3("3.mp3");
  Serial.println("3");
  break;
  case 4:
  playMP3("4.mp3");
  Serial.println("4");
  break;
  playMP3("5.mp3");
  Serial.println("5");
  break;
  default:
  playMP3("6.mp3");
  Serial.println("6");
  break;
  // Add more playMP3() calls for additional songs
  }
  delay(20000); // Delay between songs
}

void playMP3(const char* filename) {
  // Calculate the file index from the filename
  uint16_t index = ((DEVICE_NUMBER - 1) << 8) | strlen(filename);
  
  // Send the play command
  sendCommand(CMD_NEXT_SONG, index);

  // Wait for the response
  while (mySerial.available()) {
    if (mySerial.read() == 0x3C) {
      // Song started playing
      Serial.print("Playing: ");
      Serial.println(filename);
      break;
    }
  }

  // Wait for the song to finish playing
  while (mySerial.available()) {
    if (mySerial.read() == 0x3E) {
      // Song finished playing
      Serial.println("Song finished");
      break;
    }
  }
}
