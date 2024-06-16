//eeprom address pins to arduino digitial pins
#define A0 34
#define A1 36
#define A2 38
#define A3 40
#define A4 42
#define A5 44
#define A6 46
#define A7 48
#define A8 30
#define A9 28
#define A10 24
#define A11 26
#define A12 50
#define A13 32
#define A14 52

//eeprom data pins to arduino digital pins
#define D0 37
#define D1 35
#define D2 33
#define D3 31
#define D4 29
#define D5 27
#define D6 25
#define D7 23

//eeprom control signal pins to arduino digital pins
#define WRITE_EN 2
#define OUT_EN 3
#define CHIP_EN 4

//address and data pin arrays
int addressPin[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14};
int dataPin[] = {D0, D1, D2, D3, D4, D5, D6, D7};

//set address and output enable
void setAddress(int address, bool outEN) {
  if (outEN) { //when we want to read from the EEPROM
    digitalWrite(OUT_EN, LOW);
  }
  else { //when we want to write to the EEPROM
    digitalWrite(OUT_EN, HIGH);
  }
  
  //set all address pins to output from the arduino
  for (int i = 0; i <= 14; i += 1) {
    pinMode(addressPin[i], OUTPUT);
  }

  for (int i = 0; i <= 14; i += 1) {
    digitalWrite(addressPin[i], address & 1);
    address = address >> 1;
  }
}

byte readEEPROM(int address){
  //set all data pins to input from the arduino
  for (int i = 0; i <= 7; i = i + 1) {
    pinMode(dataPin[i], INPUT);
  }


  delayMicroseconds(1); //delay for output delay

  byte data = 0;
  for (int i = 7; i >= 0; i -= 1){
    data = (data << 1) + digitalRead(dataPin[i]);
  }

  return data;
}

void writeEEPROM(int address, int data) {
  setAddress(address, false);

  //set all data pins to output from the arduino
  for (int i = 0; i <= 7; i = i + 1) {
    pinMode(dataPin[i], OUTPUT);
  }

  for (int i = 0; i <= 7; i += 1) {
    digitalWrite(dataPin[i], data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delay(5);
  digitalWrite(WRITE_EN, HIGH);
  delay(1);
  digitalWrite(OUT_EN, LOW);
}

void printContents() {
  for (int base = 0; base <= 255; base = base + 16) {
    byte data[16];
    for (int offset = 0; offset <= 16; offset = offset + 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:   %02x %02x %02x %02x %02x %02x %02x %02x     %02x %02x %02x %02x %02x %02x %02x %02x",
     base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
    
    Serial.println(buf);
  }
}

void setup() { 

  pinMode(WRITE_EN, OUTPUT);
  pinMode(OUT_EN, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  digitalWrite(OUT_EN, HIGH);

  Serial.begin(57600);

}

void loop() {

}