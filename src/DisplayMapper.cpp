#include "DisplayMapper.h"
#include <Arduino.h>
#include <U8g2lib.h> /* aus dem Bibliotheksverwalter */
#include "BEE_1.h"

DisplayMode _currentMode;
float _CurrentCoinValue;
float _Price;

bool verbose;

// Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
    // Rotation
    U8G2_R0,
    // No Rotation required
    U8X8_PIN_NONE);
// \xe4\xf6\xfc
// äöü
#define LCDWidth u8g2.getDisplayWidth()

#define ALIGN_CENTER(t) ((LCDWidth - (u8g2.getUTF8Width(t))) / 2)

#define ALIGN_RIGHT(t) (LCDWidth - u8g2.getUTF8Width(t))


#define ALIGN_LEFT 0
#define ALIGN_LEFT_BESIDE_IMAGE 65

DisplayMapper::DisplayMapper(bool verbose)
{
}

void DisplayMapper::Setup()
{
    Serial.println("Setup Display");

    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setBusClock(800000); // experimental
    // u8g2.begin();
    u8g2.enableUTF8Print();
    // u8g2.clearBuffer();}
}

void DisplayMapper::DisplayPleaseLockBox()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB12_tf);
    u8g2.drawStr(ALIGN_LEFT, 20, "Bitte Fach");

    u8g2.setFont(u8g2_font_helvB12_tf);
    u8g2.drawStr(ALIGN_LEFT, 40, "Schliessen");
    u8g2.sendBuffer();
}

void DisplayMapper::PrintPleaseChoose()
{
    u8g2.clearBuffer();
    u8g2.firstPage();
    do
    {
        u8g2.drawXBMP(0, 0, bee_width, bee_height, bee);
        u8g2.setFont(u8g2_font_helvB12_tf);
   
        u8g2.drawStr(ALIGN_LEFT_BESIDE_IMAGE, 20, "Bitte");
        u8g2.drawStr(ALIGN_LEFT_BESIDE_IMAGE, 40, "Produkt");
        u8g2.drawStr(ALIGN_LEFT_BESIDE_IMAGE, 60, "W\xe4hlen");

    } while (u8g2.nextPage());
}

void DisplayMapper::DisplayTakeProduct()
{

    u8g2.clearBuffer();
    sprintf(displayOutput, "Bitte Produkt ");
    u8g2.setFont(u8g2_font_helvB12_tf);
    u8g2.drawStr(ALIGN_LEFT, 20, displayOutput);

    sprintf(displayOutput, "entnehmen");
    u8g2.setFont(u8g2_font_helvB12_tf);
    u8g2.drawStr(ALIGN_LEFT, 40, displayOutput);
    u8g2.sendBuffer();

    sprintf(displayOutput, "Vielen Dank!");
    u8g2.setFont(u8g2_font_helvB12_tf);
    u8g2.drawStr(ALIGN_LEFT, 60, displayOutput);
    u8g2.sendBuffer();
}

void DisplayMapper::Print(DisplayMode mode)
{
    switch (mode)
    {
    case PleaseChoose:
        this->PrintPleaseChoose();
        break;
    case PleaseClose:
        this->DisplayPleaseLockBox();
        break;
    case TakeProduct:
        this->DisplayTakeProduct();
        break;
    }
}

void DisplayMapper::PrintCredit(char *product)
{
    char displayOutput[30];
    u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_helvB12_tf);
    strcpy(displayOutput, "Gegeben: ");
    u8g2.drawStr(ALIGN_LEFT, 20, displayOutput);
    // Print Value
    strcpy(displayOutput, "");
    dtostrf(this->_CurrentCoinValue, 2, 2, &displayOutput[strlen(displayOutput)]);
    u8g2.drawStr(ALIGN_RIGHT(displayOutput), 20, displayOutput);

    if (product == "")
    {
        u8g2.setFont(u8g2_font_helvB08_tf);
        strcpy(displayOutput, "Bitte w\xe4hlen Sie");
        u8g2.drawStr(ALIGN_CENTER(displayOutput), 40, displayOutput);

        u8g2.setFont(u8g2_font_helvB08_tf);
        strcpy(displayOutput, "das Produkt");
        u8g2.drawStr(ALIGN_CENTER(displayOutput), 60, displayOutput);
        u8g2.sendBuffer();
    }
    else
    {
        u8g2.setFont(u8g2_font_helvB12_tf);
        strcpy(displayOutput, "zu geben: ");
        u8g2.drawStr(ALIGN_LEFT, 40, displayOutput);
        // Print Value
        strcpy(displayOutput, "");
        dtostrf(this->_Price, 2, 2, &displayOutput[strlen(displayOutput)]);
        u8g2.drawStr(ALIGN_RIGHT(displayOutput), 40, displayOutput);

        u8g2.setFont(u8g2_font_helvB08_tf);
        u8g2.drawStr(ALIGN_LEFT, 60, product);
        u8g2.sendBuffer();
    }
}

void DisplayMapper::SetGivenCoinValue(float value)
{
    this->_CurrentCoinValue = value;
}
void DisplayMapper::SetCurrentPrice(float price)
{

    this->_Price = price;
}
