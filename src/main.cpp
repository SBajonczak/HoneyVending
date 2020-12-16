#include <Arduino.h>

/*
  D v d B. ACTIVE LOW.
 */

#define RELAY_CH1 3
#define COIND_SELECTOR 2

int impulsCount = 0;
float totalInsertedCoinValue = 0;
int i = 0;

void incomingImpuls()
{
  impulsCount = impulsCount + 1;
  i = 0;
}

void setup()
{
  totalInsertedCoinValue = 0;
  pinMode(RELAY_CH1, OUTPUT);
  digitalWrite(RELAY_CH1, HIGH); // switch on LED1

  attachInterrupt(0, incomingImpuls, RISING);

  Serial.begin(9600); // Setup serial at 9600 baud

  delay(2000); // Don't start main loop until we're sure that the coin selector has started

  Serial.println("Ready..");
}

void loop()
{

  i = i + 1;
  Serial.print("i=");
  Serial.print(i);
  Serial.print(" Impulses:");
  Serial.print(impulsCount);
  Serial.print(" Total:");
  Serial.print(totalInsertedCoinValue);
  Serial.println("");
  delay(1);

  if (i >= 15)
  {

    switch (impulsCount)
    {
    case 1: // 50 cent
      totalInsertedCoinValue += 0.5;
      impulsCount = 0;
      break;

    case 2: // 1 euro
      impulsCount = 0;
      totalInsertedCoinValue += 1.0;
      break;
    case 3: // 2 euro
      impulsCount = 0;
      totalInsertedCoinValue += 2.0;
      break;
    }
  }
  if (totalInsertedCoinValue >= 7.5)
  {
    totalInsertedCoinValue = 0;
    digitalWrite(RELAY_CH1, LOW); // switch on LED1
    delay(3000);
    digitalWrite(RELAY_CH1, HIGH); // switch on LED1
  }
}
