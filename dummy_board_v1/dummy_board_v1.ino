//green leds - PORTD B001111xx
#define G1 2
#define G2 3
#define G3 4
#define G4 5

//RED AND SWITCH - PORTC B00001111
#define C1_A A0   //14
#define C2_A A1   //15
#define C3_A A2   //16
#define C4_A A3   //17

//control
// PORTD B11000000
#define RES 6 //B01000000
#define SEL 7 //B10000000
//PORTC x0110000
#define LED_EN A4 //Bx0010000
#define SW_PRS A5 //Bx0100000

#define SW_EN 12  ////PORTB Bx0010000

//data - PORTB Bx0001111
#define D1 8
#define D2 9
#define D3 10
#define D4 11

//memory
byte GAME_MEM = B00000000; //used a byte to save memory, its used as two 4 bit GAME_MEM
byte SWITCH_MEM = B00000000;  //in reality, two bits are used for each switch, it just makes it easier to save to the registers later
byte CTRL_BUS = B00001100;  // used to buffer inputs for the flow of the program
//CTRL_BUS = B|000|sw_out|sw_en|res|sel|led_en|
//CTRL_BUS is fixed for now

//temp memory
byte SHIFT_MEM = B00000000;
int VR = 0;
//extras and configs
#define threshold 100  //about 0.5V
void setup() {
  Serial.begin(250000);

  //to be replaced with direct pin manipulation later
  pinMode(G1, OUTPUT);  pinMode(G2, OUTPUT);  pinMode(G3, OUTPUT);  pinMode(G4, OUTPUT);
  pinMode(C1_A, OUTPUT);  pinMode(C2_A, OUTPUT);  pinMode(C3_A, OUTPUT);  pinMode(C4_A, OUTPUT);
  pinMode(D1, OUTPUT);  pinMode(D2, OUTPUT);  pinMode(D3, OUTPUT);  pinMode(D4, OUTPUT);
  pinMode(RES, INPUT);  pinMode(SEL, INPUT);  pinMode(SW_PRS, OUTPUT);  pinMode(LED_EN, INPUT); pinMode(SW_EN, INPUT);
}
void loop() {
  Serial.println();
  CTRL_READ(); //taken out for now
  SW_READ();
  GAME_SAVE ();
  R_LED_ON ();

  
  DEBUG_DUMP("CTRL_BUS:  ", CTRL_BUS);
  DEBUG_DUMP("SWITCH_MEM:  ", SWITCH_MEM);
  DEBUG_DUMP("GAME_MEM: ", GAME_MEM);
  DEBUG_DUMP("PORTC:    ", PORTC);

  delay(1000);

}

void CTRL_READ() {
  //reading the state of the control bus so we know what to do
  //this will be replaced by direct pin manipulation later (its a faster and more compact option)
  if (digitalRead(RES) == HIGH) {
    CTRL_BUS = CTRL_BUS | B00000100;
  } else {
    CTRL_BUS = CTRL_BUS & B11111011;
  }
  if (digitalRead(SEL) == HIGH) {
    CTRL_BUS = CTRL_BUS | B00000010;
  } else {
    CTRL_BUS = CTRL_BUS & B11111101;
  }
  if (digitalRead(LED_EN) == HIGH) {
    CTRL_BUS = CTRL_BUS | B00000001;
  } else {
    CTRL_BUS = CTRL_BUS & B11111110;
  }
  if (digitalRead(SW_EN) == HIGH) {
    CTRL_BUS = CTRL_BUS | B00001000;
  } else {
    CTRL_BUS = CTRL_BUS & B11110111;
  }
}

void SW_READ() {
  if ((CTRL_BUS & B0001000) == 0) { //check if switch enable is off
    PORTC = PORTC & B11011111;      //turn off the switch feedback pin and skip the switch reading
  }  else {
    DDRC = B00100000;   //SET sregister C to inputs except for LED output
    PORTC = B00001111;  //enable the pull up resistors
    VR = analogRead(C1_A);
    if (VR <= threshold) {
      SWITCH_MEM =  SWITCH_MEM | B00010001;
    }
    else {
      SWITCH_MEM =  SWITCH_MEM & B11101110;
    }
    VR = analogRead(C2_A);
    if (VR <= threshold) {
      SWITCH_MEM =  SWITCH_MEM | B00100010;
    }
    else {
      SWITCH_MEM =  SWITCH_MEM & B11011101;
    }
    VR = analogRead(C3_A);
    if (VR <= threshold) {
      SWITCH_MEM =  SWITCH_MEM | B01000100;
    }
    else {
      SWITCH_MEM =  SWITCH_MEM & B10111011;
    }
    VR = analogRead(C4_A);
    if (VR <= threshold) {
      SWITCH_MEM =  SWITCH_MEM | B10001000;
    }
    else {
      SWITCH_MEM =  SWITCH_MEM & B01110111;
    }
    PORTC = B00000000;
    if (SWITCH_MEM != 0) {
      PORTC = PORTC | B00100000;    //turn on the switch feedback pin
    }
    else {
      PORTC = PORTC & B11011111;    //turn off the switch feedback pin
    }
    R_LED_ON (); //turn the LED back on
  }
}
void GAME_SAVE () {
  SHIFT_MEM =  GAME_MEM | SWITCH_MEM;   //join the current game state with the switches
  if ((CTRL_BUS & B0000010) == 0) {     //select line is low (bank 2)
    SHIFT_MEM = SHIFT_MEM & B11110000;  //extract the bits we want
    GAME_MEM = GAME_MEM | SHIFT_MEM;    //insert the new state of the game into the game memory
    SHIFT_MEM = SHIFT_MEM >> 2;         //shift the bits to align with the output register
    PORTD = PORTD | SHIFT_MEM;          //updade the data output with the game memory
  }
  else {
    SHIFT_MEM = SHIFT_MEM & B00001111;  //extract the bits we want
    GAME_MEM = GAME_MEM | SHIFT_MEM;    //insert the new state of the game into the game memory
    //this chunk of code doesnt update any LEDs directly as these are shared with the switches
  }
}
void R_LED_ON () {
  if ((CTRL_BUS & B0000001) == 0) {     //check if LED enable is off
    PORTC = PORTC & B11110000;          //turn off the red LEDs
  }
  else {                                //this runs if the LED enable is on
    SHIFT_MEM = GAME_MEM;               //get the game board state
    SHIFT_MEM = SHIFT_MEM & B00001111;  //extract the needed bits
    SHIFT_MEM = SHIFT_MEM ^ B00001111;  //invert the state of the bits
    DDRC = DDRC | B00001111;            //set the pins as outputs
    PORTC = PORTC | SHIFT_MEM;          //set trun the LEDs back on
  }
}
void DEBUG_DUMP(const char* msg, byte bus) { //sends the binary of the registers to the serial console (thanks to a really cool friend)
  Serial.print(msg);
  for (int i = 0; i < 8; ++i)
  {
    Serial.write((bus & (1 << (7 - i))) != 0 ? '1' : '0');
  }
  Serial.println();
}
