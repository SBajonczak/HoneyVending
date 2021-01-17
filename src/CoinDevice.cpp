#include "CoinDevice.h"
#include <Arduino.h>


// Dies definiert die Impulse die vom Counter nach dem Anlernen kommen
// 50 Cent wurden so angelernt das ein Impuls kommt
// 1 Euro sendet der Münzer zwei Impulse
// 2 Euro sendet der Münzer drei Impulse
const int PULSE_AMOUNT_50_CENT = 1;
const int PULSE_AMOUNT_1_EURO = 2;
const int PULSE_AMOUNT_2_EURO = 3;

int _impulsCount;
int _connectedPin;
int _Counter;



// cto that gets the conntected impulse pin
CoinDevice::CoinDevice(int connectedPin)
{
    this->_connectedPin = connectedPin;
    this->setup();
}

// Get the inserted Coin Value
float CoinDevice::GetInsertedCurrency()
{
    Serial.print("Impuls ");
    Serial.print(this->_impulsCount);
    Serial.print(" counter ");
    Serial.print(this->_counter);
    Serial.println();
    this->_impulsCount++;
    delay(1);
    if (this->_counter >= 5)
    {
        switch (this->_impulsCount)
        {
        case PULSE_AMOUNT_50_CENT: // 50 cent
            // totalInsertedCoinValue += 0.5;
            this->_impulsCount = 0;
            return 0.5;
        case PULSE_AMOUNT_1_EURO: // 1 euro
            this->_impulsCount = 0;
            return 1;
        case PULSE_AMOUNT_2_EURO: // 2 euro
            this->_impulsCount = 0;
            return 2;
        }
    }
    return 0;
}

void gotImpulse()
{
    _impulsCount = _impulsCount + 1;
    _Counter = 0;
}

void CoinDevice::setup()
{
    attachInterrupt(this->_connectedPin, gotImpulse, RISING);
}