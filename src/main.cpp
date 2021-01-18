#include <Arduino.h>
#include <EEPROM.h> // Zwischenspeicher

#include "CoinDevice.h"
#include "DisplayMapper.h"
// Definiert die Pin Belegeung
#define RELAY_CH1 3
#define COIN_SELECTOR_PIN 0
#define BUTTON_PLUS 4
#define BUTTON_INPUT_BOX_1 5
#define BOX_LOCKED_VCC_SIGNAL 7
#define BOX_1_LOCKED_SIGNAL 6
#define EEPROM_INSERTED_AMOUNT_VALUE_INDEX 0

// Wer experimentiern will kann dieses Feature Aktivieren.
// Das sorgt dafür das bereits eingeworfene bzw erkennte Werte nach einem neustart noch vorhanden sind.

// Dies setzt den Preis fest, der geprüft werden soll
float PriceForBox = 7.5;

// Systemvariablen
int selectedBox = 0;
int AcutallyOpenedBox = 0;
double totalInsertedCoinValue = 0;
int i = 0;
// Maximale Zeichen ausgabe auf dem Display (Fontsize 12 = 13 Zeichen maximal in einer Zeile)

//Dies sind die Umlaute UTF Codes, wenn diese verwendet werden sollen.
// \xe4\xf6\xfc
// äöü

// Für den Fall das ein HELTEC Device verwendet wird ist diese Zeile zu aktivieren und die darunter liegende auszukommentieren.
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// // Display
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
//     // Rotation
//     U8G2_R0,
//     // No Rotation required
//     U8X8_PIN_NONE);

// Hilfsfunktion für das Anordnen von Textzeilen

// The Coin device
CoinDevice coindevice(COIN_SELECTOR_PIN);
DisplayMapper display(false);

// Hiermit werden die Buttons initiiert.
void InitButtons()
{
  digitalWrite(BUTTON_PLUS, HIGH);
  pinMode(BUTTON_PLUS, OUTPUT);
  pinMode(BUTTON_INPUT_BOX_1, 0x09); // input pulldown
}

void HandleOpenedBox()
{
  digitalWrite(RELAY_CH1, HIGH);
}

void InitBoxSignals()
{
  digitalWrite(BOX_LOCKED_VCC_SIGNAL, HIGH);
  pinMode(BOX_LOCKED_VCC_SIGNAL, OUTPUT);
  pinMode(BOX_1_LOCKED_SIGNAL, 0x09); // input pulldown
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Start");

  display.Setup();
  coindevice.setup();

  InitButtons();
  InitBoxSignals();
  totalInsertedCoinValue = 0;
  pinMode(RELAY_CH1, OUTPUT);
  digitalWrite(RELAY_CH1, HIGH);

#if USE_EEPROM
  float val;
  EEPROM.get(EEPROM_INSERTED_AMOUNT_VALUE_INDEX, val);
  Serial.println(val);
  if (!isnan(val))
  {
    totalInsertedCoinValue = val;
  }
#endif
  Serial.println("Ready..");
}

// void OpenDoor(int relaisChannel, int selectedBox)
// {
//   AcutallyOpenedBox = selectedBox;
//   DisplayTakeProduct(selectedBox);
//   digitalWrite(RELAY_CH1, LOW); // switch on LED1
//   delay(1000);
//   digitalWrite(RELAY_CH1, HIGH); // switch on LED1
// }

bool BoxesClosed()
{
  Serial.println(digitalRead(BOX_1_LOCKED_SIGNAL));
  if (digitalRead(BOX_1_LOCKED_SIGNAL) == HIGH)
  {
    return true;
  }
  HandleOpenedBox();
  return false;
}

void HandleBoxSelection()
{

  if (digitalRead(BUTTON_INPUT_BOX_1) == HIGH)
  {
    selectedBox = 1;
    display.SetCurrentPrice(7.5);
  }
}
void HandleCoinInsertions()
{
  float value = coindevice.GetInsertedCurrency();
  if (value > 0)
  {
    totalInsertedCoinValue += value;
    display.SetGivenCoinValue(totalInsertedCoinValue);
    // Wert in den EEPROM schreiben
    EEPROM.put(EEPROM_INSERTED_AMOUNT_VALUE_INDEX, totalInsertedCoinValue);
  }
}

void loop()
{
  HandleCoinInsertions();
  display.Print(TakeProduct);
  return;
  if (!BoxesClosed())
  {
    display.Print(PleaseClose);
  }
  else
  {
    // HandleBoxSelection();

    if (totalInsertedCoinValue >= PriceForBox)
    {
      if (selectedBox == 0)
      {
        display.Print(PleaseChoose);
      }
      else
      {
        selectedBox = 0;
        // Wert in den EEPROM schreiben
        EEPROM.put(EEPROM_INSERTED_AMOUNT_VALUE_INDEX, 0);
        totalInsertedCoinValue = 0;
        display.Print(TakeProduct);
        // OpenDoor(RELAY_CH1, selectedBox);
      }
    }
    else
    {
      if (totalInsertedCoinValue > 0 || selectedBox > 0)
      {
        switch (selectedBox)
        {
        case 1:
          display.PrintCredit("Lindenhonig");
          break;
        default:
          display.PrintCredit("");
          break;
        }
      }
      else
      {
        display.Print(PleaseChoose);
      }
    }
  }
}