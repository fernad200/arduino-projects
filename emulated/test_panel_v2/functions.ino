void RGB_DISP() {
  if (color > 2) {
    color = 0;
  }
  //turn off ALL led pins
  PORTB |= 0X0E; //sets the RGB pins to high
  PORTD &= 0X03; //set all the position pins to low
  PORTB &= 0xFE; //there's one here too
  temp = rgb_val[color]; //get the color value
  temp &= (blk_map | BLK); //used for flashing
  temp2 = temp >> 6 & 0x01; //get the single bit we need here XD
  temp = temp << 2; //align the value to the register
  PORTD |= temp;  //write the values
  PORTB |= temp2; //write the values
  digitalWrite(rgb_pin[color], 0x00); //not optimized... yet
  color++;
}


byte sw_read() {
  byte sw = 0x00;
  DDRD = DDRD & 0x03;
  DDRB = DDRB & 0xFE;
  digitalWrite(com_sw, 0x01);
  for (byte i = 0; i < 7; i++) {
    sw = sw | digitalRead(led_pin[i]) << i;
  }
  digitalWrite(com_sw, 0x00);
  DDRD = DDRD | 0xFC;
  DDRB = DDRB | 0x01;
  Serial.println(sw);
  return sw;
}
