
const int push_detect_time = 100;

const int D2=2;
const int D3=3;
const int D4=4;
const int D5=5;

const int button_start_pin = D5;
const int button_reset_pin = D4;
const int button_plus_pin  = D2;
const int button_minus_pin = D3;

const int out_pin1 = 12;
const int out_pin2 = 11;
const int out_pin3 = 10;
const int out_pin4 = 9;
const int out_pin5 = 8;
const int out_pin6 = 7;

template<int PIN> class db {
  int last_value = HIGH;
  int last_change_milis = 0;
  int value = HIGH;
  bool did_read1 = true;
 public:
  
  bool is_edge() {
    if ((value == HIGH) && !did_read1) {
      did_read1 = true;
      return true;
    }
    return false;
  }

  void loop() {
    int val = digitalRead(PIN);
    if (val != last_value) {
      last_value = val;
      last_change_milis = millis();
    }
    if (val != value) {
      if (millis() - last_change_milis > push_detect_time) {
        value = val;
        did_read1 = false;
      }
    }
  }
};
void output(int val) {
  //writ(out_pin1, val&1);
  //writ(out_pin2, val&2);
  //writ(out_pin3, val&4);
  //writ(out_pin4, val&8);
  //writ(out_pin5, val&16);
  //writ(out_pin6, val&32);
  Serial.print("write value ");
  Serial.println(val, BIN);

  //0-63
  int val_dac = val*16;
  analogWrite(PIN_DAC0, val_dac );
}

int value = 32;
int pamet = 32;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(out_pin1, OUTPUT);
  //pinMode(out_pin2, OUTPUT);
  //pinMode(out_pin3, OUTPUT);
  //pinMode(out_pin4, OUTPUT);
  //pinMode(out_pin5, OUTPUT);
  //pinMode(out_pin6, OUTPUT);
  pinMode(PIN_DAC0, OUTPUT);
  analogWriteResolution(10);
  analogReference(AR_EXTERNAL);

  pinMode(button_start_pin, INPUT_PULLUP);
  pinMode(button_reset_pin, INPUT_PULLUP);
  pinMode(button_plus_pin, INPUT_PULLUP);
  pinMode(button_minus_pin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("ready");
  value = 32;
  pamet = 32;
  output(value);
  delay(1000);
  Serial.println("start");
  digitalWrite(LED_BUILTIN, HIGH);
}

db<button_start_pin> button_start;
db<button_reset_pin> button_reset;
db<button_plus_pin> button_plus;
db<button_minus_pin> button_minus;

void writ(int pin, int value) {
  if (value) digitalWrite(pin, HIGH);
  else digitalWrite(pin, LOW);
}

int pauza = 0;

void loop() {
  button_start.loop();
  button_reset.loop();
  button_plus.loop();
  button_minus.loop();

  //int analog_value = analogRead(A2);
  //Serial.println(analog_value, DEC);

  if (button_start.is_edge()) {
    Serial.println("button start");
    if (pauza == 0) {
      Serial.println("start ignoruji- nebylo stop");
    } else {
      value = pamet;
      output(value);
      pauza = 0;
    }
  }
  if (button_reset.is_edge()) {
    Serial.println("button reset");
    pamet = value;
    value = 32;
    output(value);
    pauza = 1;
  }
  if (button_plus.is_edge()) {
    Serial.println("button plus");
    if (value < 63) value = value+1;
    output(value);
    pauza = 0;
  }
  if (button_minus.is_edge()) {
    Serial.println("button minus");
    if (value > 0) value = value-1;
    output(value);
    pauza = 0;
  }
  
}
