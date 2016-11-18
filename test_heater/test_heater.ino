// Heater
int PIN_HEATER_OUTPUT = 22;

void heater_enable(void)
{
  // Active LOW is enable
  digitalWrite(PIN_HEATER_OUTPUT, LOW);
}

void heater_disable(void)
{
  // Active HIGH is disable
  digitalWrite(PIN_HEATER_OUTPUT, HIGH);
}

void heater_myinit(void)
{
  pinMode(PIN_HEATER_OUTPUT, OUTPUT);
}


void setup() {
  // put your setup code here, to run once:
  heater_myinit();
}

int is_enable = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if(is_enable == 0)
  {
    is_enable = 1;
    // heater_enable();
    heater_disable();
  }
  
  delay(1000);
}
