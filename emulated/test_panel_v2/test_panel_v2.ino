#define com_red 9
#define com_green 10
#define com_blue 11
#define com_sw 12
#define NET_ID 0x20


byte serial_mem[5];
bool startup = 0x00;
byte color = 0x00;
byte rgb_val[3] = {0x40, 0x00, 0x00};
byte rgb_pin[3] = {9, 10, 11};
byte led_pin[7] = {2, 3, 4, 5, 6, 7, 8};
byte blk_map = 0xBf;
byte temp, temp2;
byte BLK = 0x00;
byte nw_sw;
int serial_to;
unsigned long int tim;
void setup() {
  Serial.begin(9600);
  DDRD |= B11111100;
  DDRB |= B00011111;
  PORTB |= B00001110;
  tim = millis();
}

void loop() {
  do {
    delay(5);
    RGB_DISP();
    if ((millis() - tim) < 500) {
      tim = millis();
      //      BLK = 0xff - BLK;
      serial_to++; //nasty way to time it but we know when we're entering here.
      if (serial_to >= 120) {
        rgb_val[0] = rgb_val[0] | 0x40;
      }
    }

  } while (Serial.available() < 5);
  serial_to = 0;
  for (byte i = 0; i < 5; i++) {
    serial_mem[i] = Serial.read();
  }
  Serial.flush();
  if (serial_mem[0] = NET_ID) {
    switch (serial_mem[1]) {
      case 1:
        rgb_val[0] = serial_mem[2];
        rgb_val[1] = serial_mem[3];
        rgb_val[2] = serial_mem[4];
        break;
      case 2:
        Serial.write(sw_read());
        break;
    }
  }

}
