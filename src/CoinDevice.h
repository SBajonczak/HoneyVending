#ifndef CoinDevice_H
#define CoinDevice_H

class CoinDevice
{
public:
    // cto that gets the conntected impulse pin
    CoinDevice(int connectedPin);
    // Get the inserted Coin Value
    float GetInsertedCurrency();
    // Setup
    void setup();

private:
    int _counter;
    int _impulsCount;
    int _connectedPin;
};

#endif