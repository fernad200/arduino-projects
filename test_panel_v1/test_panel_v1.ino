#define com_red 9
#define com_green 10
#define com_blue 11
#define com_sw 12

bool startup = 0x00;
byte color = 0x00;
byte rgb_val[3] = {0x40, 0x00, 0x00};
byte rgb_pin[3] = {9, 10, 11};
byte led_pin[7] = {2, 3, 4, 5, 6, 7, 8};
byte blk_map = 0xBf;
byte temp, temp2;
byte BLK = 0x00;
byte nw_sw;
byte serial_to;
unsigned long int tim;
void setup() {
  Serial.begin(9600);
  DDRD = DDRD | B11111100;
  DDRB = DDRB | B00011111;
  PORTB = PORTB | B00001110;
  tim = millis();
}

void loop() {
  do {
    delay(5);
    RGB_DISP();
  } while ((millis() - tim) < 500);
  tim = millis();
  BLK = 0xff - BLK;
  serial_to++;

  if (Serial.available() >= 4) {
    serial_to = 0;
    rgb_val[0] =  Serial.read();
    rgb_val[1] =  Serial.read();
    rgb_val[2] =  Serial.read();
    blk_map =  0xff - Serial.read();
    Serial.flush();
   byte t = sw_read();
    Serial.write(t);
  }
  if (serial_to >= 30) {
    rgb_val[0] = rgb_val[0] | 0x40;
    blk_map = blk_map & 0xbf;
  }
  if (serial_to >= 60) {
    serial_to = 0;
    rgb_val[0] = 0x40;
    rgb_val[1] =  0x00;
    rgb_val[2] =  0x00;
  }
}
