#define pot  A0

int km_ph = 0;
int mx_limit  = 127;
int pot_value;
int adc;
boolean speed_limit = 0;
boolean zone_exit = 0;
#define motor1 10
#define motor2 11
#include <LiquidCrystal_I2C.h>
int motorInput;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  // put your setup code here, to run once:
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("   Welcome   ");
  delay(1000);
}

void loop()
{
  lcd.clear();
  Serial.print("Zone: ");
  Serial.print(" (");
  Serial.print(digitalRead(2));
  Serial.print(digitalRead(3));
  Serial.print(digitalRead(4));
  Serial.print(digitalRead(5));
  Serial.print(")");
  if ((digitalRead(2) == 1) && (digitalRead(3) == 0) && (digitalRead(4) == 1) && (digitalRead(5) == 1))
  {
    zone_exit = 1;
    mx_limit = 30;
    Serial.print("School zone");
    lcd.setCursor(0, 0);
    lcd.print("School zone");
  }

  else if ((digitalRead(2) == 1) && (digitalRead(3) == 1) && (digitalRead(4) == 0) && (digitalRead(5) == 1))
  {
    zone_exit = 1;
    mx_limit = 40;
    Serial.print("Hospital zone");
    lcd.setCursor(0, 0);
    lcd.print("Hospital zone");
  }

  else
  {
    zone_exit = 0;
    speed_limit = 0;
    Serial.print("No zone");
    mx_limit = 127;
    lcd.setCursor(0, 0);
    lcd.print("No zone");
  }

  //*********************************

  pot_value = analogRead(pot);

  if ((pot_value / 8) < mx_limit)
  {
    km_ph = (pot_value / 8);
  }

  else
  {
    if (speed_limit == 0)
    {
      km_ph = (pot_value / 8);
    }

    else
    {
      km_ph = mx_limit;
    }
  }
  if ( (km_ph - mx_limit) > 10)
  {
    int i;
    for (i = km_ph; i > mx_limit; i--)
    {
      motorInput = i;
      motorInput = map(i, 1, 127, 100, 255);
      Serial.println(i);
      analogWrite(motor1, motorInput);
      analogWrite(motor2, 0);
      lcd.setCursor(0, 1);
      lcd.print("Speed : ");
      lcd.print(km_ph);
      delay(50);
    }
    km_ph = i;
    speed_limit = 1;
  }
  if (zone_exit == 0) {
    if ( ((km_ph - mx_limit) < 10))
    {
      int i;
      for (i = km_ph; i <= (pot_value / 8) ; i++)
      {
        motorInput = i;
        motorInput = map(i, 1, 127, 100, 255);
        Serial.println(i);
        analogWrite(motor1, motorInput);
        analogWrite(motor2, 0);
        lcd.setCursor(0, 1);
        lcd.print("Speed : ");
        lcd.print(km_ph);
        delay(50);
      }
      km_ph = i;
      speed_limit = 1;
    }
  }
  Serial.print("\tKmph :");
  Serial.println(km_ph);
  lcd.setCursor(0, 1);
  lcd.print("Speed : ");
  lcd.print(km_ph);

  motorInput = map(km_ph, 1, 127, 100, 255);
  if (km_ph == 0) {
    motorInput = 0;
  }
  analogWrite(motor1, motorInput);
  analogWrite(motor2, 0);
  delay(500);
}
