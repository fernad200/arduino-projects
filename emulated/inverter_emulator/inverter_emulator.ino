#define NET_ID 02 //network ID, change as needed !!MAKE SURE NO TWO DEVICES HAVE THE SAME!!
uint16_t cur_raw; //raw values of the readings
float cur; //final values
byte RGB[3] = {10, 9, 11}; //rgb pins (cause they're there)
byte serial_mem[5]; //serial coms memeory, "standard" for the project
// ID, command/data, 3x data
void setup() {
  Serial.begin(9600); //start UART communication (usb too)

  //these arent used yet
  pinMode(RGB[0], OUTPUT);
  pinMode(RGB[1], OUTPUT);
  pinMode(RGB[2], OUTPUT);
}

void loop() {
  do {
    //read the ADC on the respective pins
    cur_raw = analogRead(A0);
    cur = 50 - ((cur_raw / 1023.0) * 50.0);
    delay(5);
  } while (Serial.available() < 5);
  //read the buffer
  for (byte i = 0; i < 5; i++) {
    serial_mem[i] = Serial.read();
  }
  Serial.flush(); //empty the buffer.
  //in case there was a lost byte this just empties the buffer at the cost of two packets (not a big deal)
  if (serial_mem[0] = NET_ID) {
    switch (serial_mem[1]) {
      case 1:
        Serial.print(cur);
        break;
    }
  }
}
