#define NET_ID 03 //network ID, change as needed !!MAKE SURE NO TWO DEVICES HAVE THE SAME!!
uint16_t volt_raw, cur_raw; //raw values of the readings
float volt, cur; //final values
byte serial_mem[5]; //serial coms memeory, "standard" for the project
// ID, command/data, 3x data
byte inv = 2, inv_dat;
byte grid = 3, grid_dat;
byte gen = 4, gen_dat;
byte out = 5, out_dat;
byte raw, dat;


void setup() {
  Serial.begin(9600);
  pinMode(inv, 0X02);
  pinMode(grid, 0X02);
  pinMode(gen, 0X02);
  pinMode(out, 0X02);
}

void loop() {
  do {
    raw = 0;
    inv_dat = digitalRead(inv);
    grid_dat = digitalRead(grid);
    gen_dat = digitalRead(gen);
    out_dat = digitalRead(out);


    raw |= inv_dat;
    raw |= grid_dat << 1;
    raw |= gen_dat << 2;
    raw |= out_dat << 3;
    dat = raw;
    delay(5);
  } while (Serial.available() < 5);
  for (byte i = 0; i < 5; i++) {
    serial_mem[i] = Serial.read();
  }
  Serial.flush(); //empty the buffer.
  //in case there was a lost byte this just empties the buffer at the cost of two packets (not a big deal)
  if (serial_mem[0] = NET_ID) {
    switch (serial_mem[1]) {
      case 1:
        Serial.write(dat);
        break;
      case 2:
        Serial.write(inv_dat);
        break;
      case 3:
        Serial.write(grid_dat);
        break;
      case 4:
        Serial.write(gen_dat);
        break;
      case 5:
        Serial.write(out_dat);
        break;
    }
  }




}
