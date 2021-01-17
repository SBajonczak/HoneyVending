#include <Arduino.h>
#include <EEPROM.h>  // Zwischenspeicher
#include <U8g2lib.h> /* aus dem Bibliotheksverwalter */
#include "CoinDevice.h"
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
int impulsCount = 0;
int selectedBox = 0;
int AcutallyOpenedBox = 0;
double totalInsertedCoinValue = 0;
int i = 0;
// Maximale Zeichen ausgabe auf dem Display (Fontsize 12 = 13 Zeichen maximal in einer Zeile)
char displayOutput[30];

//Dies sind die Umlaute UTF Codes, wenn diese verwendet werden sollen.
// \xe4\xf6\xfc
// äöü

// Für den Fall das ein HELTEC Device verwendet wird ist diese Zeile zu aktivieren und die darunter liegende auszukommentieren.
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
    // Rotation
    U8G2_R0,
    // No Rotation required
    U8X8_PIN_NONE);

// Hilfsfunktion für das Anordnen von Textzeilen
#define LCDWidth u8g2.getDisplayWidth()
#define ALIGN_CENTER(t) ((LCDWidth - (u8g2.getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (LCDWidth - u8g2.getUTF8Width(t))
#define ALIGN_LEFT 0

// The Coin device
CoinDevice coindevice(COIN_SELECTOR_PIN);


// Initialisier das Display
void InitDisplay()
{
  u8g2.setBusClock(800000); // experimental
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();
}

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

void PrintoutConfiguration(){
#if USE_EEPROM
  Serial.println("Using EEPROM");
#endif
}

void setup()
{
  coindevice.setup();

  PrintoutConfiguration();
  InitButtons();
  InitBoxSignals();
  u8g2.begin(); // setzt das Display auf Bereitschaft
  u8g2.clearBuffer();
  totalInsertedCoinValue = 0;
  pinMode(RELAY_CH1, OUTPUT);
  digitalWrite(RELAY_CH1, HIGH);

  Serial.begin(9600); // Setup serial at 9600 baud
  InitDisplay();

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

void PrintCredit()
{

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB12_tf);

  strcpy(displayOutput, "Preis: ");
  u8g2.drawStr(ALIGN_LEFT, 20, displayOutput);
  // Print Value
  strcpy(displayOutput, "");
  dtostrf(PriceForBox, 2, 2, &displayOutput[strlen(displayOutput)]);
  u8g2.drawStr(ALIGN_RIGHT(displayOutput), 20, displayOutput);

  u8g2.setFont(u8g2_font_helvB12_tf);
  strcpy(displayOutput, "Gegeben: ");
  u8g2.drawStr(ALIGN_LEFT, 40, displayOutput);
  // Print Value
  strcpy(displayOutput, "");
  dtostrf(totalInsertedCoinValue, 2, 2, &displayOutput[strlen(displayOutput)]);
  u8g2.drawStr(ALIGN_RIGHT(displayOutput), 40, displayOutput);

  u8g2.setFont(u8g2_font_helvB12_tf);
  strcpy(displayOutput, "Fach: ");
  u8g2.drawStr(ALIGN_LEFT, 60, displayOutput);

  if (selectedBox > 0)
  {
    // Print Value
    strcpy(displayOutput, "");
    dtostrf(selectedBox, 1, 0, &displayOutput[strlen(displayOutput)]);
    u8g2.drawStr(ALIGN_RIGHT(displayOutput), 60, displayOutput);
  }
  else
  {
    // Print Value
    strcpy(displayOutput, "Keins");
    u8g2.drawStr(ALIGN_RIGHT(displayOutput), 60, displayOutput);
  }
  u8g2.sendBuffer();
  delay(100);
}

void DisplayTakeProduct(int selectedBoxNumber)
{

  u8g2.clearBuffer();
  sprintf(displayOutput, "Fach %i ist offen ", selectedBoxNumber);
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.drawStr(ALIGN_LEFT, 20, displayOutput);

  sprintf(displayOutput, "Vielen Dank :)");
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.drawStr(ALIGN_LEFT, 40, displayOutput);
  u8g2.sendBuffer();
  delay(100);
}

void DisplaySelect()
{

  u8g2.clearBuffer();
  sprintf(displayOutput, "Bitte");
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.drawStr(ALIGN_LEFT, 20, displayOutput);

  sprintf(displayOutput, "W\xe4hlen");
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.drawStr(ALIGN_LEFT, 40, displayOutput);
  u8g2.sendBuffer();
  delay(100);
}

void DisplaySelectbox()
{

  u8g2.clearBuffer();
  sprintf(displayOutput, "Bitte");
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.drawStr(ALIGN_LEFT, 20, displayOutput);

  sprintf(displayOutput, "Box w\xe4hlen");
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.drawStr(ALIGN_LEFT, 40, displayOutput);
  u8g2.sendBuffer();
  delay(100);
}

void DisplayPleaseLockBox()
{

  u8g2.clearBuffer();
  sprintf(displayOutput, "Bitte Fach");
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.drawStr(ALIGN_LEFT, 20, displayOutput);

  sprintf(displayOutput, "Schliessen");
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.drawStr(ALIGN_LEFT, 40, displayOutput);
  u8g2.sendBuffer();
  delay(100);
  impulsCount = 0;
}

void OpenDoor(int relaisChannel, int selectedBox)
{
  AcutallyOpenedBox = selectedBox;
  DisplayTakeProduct(selectedBox);
  digitalWrite(RELAY_CH1, LOW); // switch on LED1
  delay(1000);
  digitalWrite(RELAY_CH1, HIGH); // switch on LED1
  impulsCount = 0;
}

bool BoxesClosed()
{
  Serial.print("BOx signal");
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
  }
}
void HandleCoinInsertions()
{
  float value = coindevice.GetInsertedCurrency();
  if (value > 0)
  {
    totalInsertedCoinValue += value;
    // Wert in den EEPROM schreiben
    EEPROM.put(EEPROM_INSERTED_AMOUNT_VALUE_INDEX, totalInsertedCoinValue);
  }
}

void loop()
{
  HandleCoinInsertions();
  if (!BoxesClosed())
  {
    DisplayPleaseLockBox();
  }
  else
  {
    HandleBoxSelection();

    if (totalInsertedCoinValue >= PriceForBox)
    {
      if (selectedBox == 0)
      {
        DisplaySelectbox();
      }
      else
      {
        selectedBox = 0;
        // Wert in den EEPROM schreiben
        EEPROM.put(EEPROM_INSERTED_AMOUNT_VALUE_INDEX, 0);
        totalInsertedCoinValue = 0;
        OpenDoor(RELAY_CH1, selectedBox);
      }
    }
    else
    {
      if (totalInsertedCoinValue > 0 || selectedBox > 0)
      {
        PrintCredit();
      }
      else
      {
        DisplaySelect();
      }
    }
  }
}