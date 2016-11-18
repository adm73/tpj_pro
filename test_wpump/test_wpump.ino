
// WATER PUMP
int PIN_WPUMP_ENABLE = 12; // enable
int PIN_WPUMP_IN = 11;    // in
int PIN_WPUMP_RELAY = 13;    // for relay
int PIN_WPUMP_SPEED = A0;    // speed


void serial_myinit()
{
  Serial.begin(9600);
}

void enable_wpump(void)
{
  digitalWrite(PIN_WPUMP_IN,  HIGH);
  digitalWrite(PIN_WPUMP_RELAY, HIGH);
}

void disable_wpump(void)
{
  digitalWrite(PIN_WPUMP_IN,  LOW);
  digitalWrite(PIN_WPUMP_RELAY, LOW);
}

void wpump_motor_myinit(void)
{
  pinMode(PIN_WPUMP_ENABLE, OUTPUT); // pin 12
  pinMode(PIN_WPUMP_IN, OUTPUT); // pin 11
  
  // for Relay
  pinMode(PIN_WPUMP_RELAY, OUTPUT); // pin 13
  
  int speed = analogRead(PIN_WPUMP_SPEED) / 4;
  analogWrite(PIN_WPUMP_ENABLE, speed);
  
  Serial.print("speed = ");
  Serial.print(speed);
  Serial.print("\n");
  
}
void setup() {
  // put your setup code here, to run once:
  
  serial_myinit();
  
  wpump_motor_myinit();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // enable_wpump();
  disable_wpump();

}
