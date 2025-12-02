#define com_red 9
#define com_green 10
#define com_blue 11
#define com_sw 12
byte sw = 0x00;

byte serial_mem[5];
bool startup = 0x00;
byte color = 0x00;
byte rgb_val[3] = {0x00, 0x00, 0xff};
byte rgb_pin[3] = {9, 10, 11};
byte led_pin[7] = {2, 3, 4, 5, 6, 7, 8};
byte blk_map = 0x00;
byte temp, temp2;
byte BLK = 0x00;
byte FP_val;
byte nw_sw;
int serial_to;
unsigned long int tim;
void setup() {
  Serial.begin(9600);
  DDRD |= B11111100;
  DDRB |= B00011111;
  PORTB |= B00001110;
  pinMode(17, 1);
  digitalWrite(17, 0);
  tim = millis();
}

void loop() {
  do {
    delay(5);     //wait 5ms so we can actualy see the colors
    RGB_DISP();   //render a color
    if ((millis() - tim) > 250) {
      //used to generate the blinking
      tim = millis();
      BLK = 0xff - BLK;

    }

  } while (Serial.available() < 4);
  FP_val = sw_read();
  for (byte i = 0; i < 4; i++) {
    serial_mem[i] = Serial.read();
  }

  Serial.flush();
  rgb_val[0] = serial_mem[0];
  rgb_val[1] = serial_mem[1];
  rgb_val[2] = serial_mem[2];
  blk_map = 0xff - serial_mem[3];
  Serial.write(FP_val);
}
