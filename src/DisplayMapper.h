#ifndef DisplayMapper_H
#define DisplayMapper_H

#include "DisplayMapper.h"
#include <U8g2lib.h> /* aus dem Bibliotheksverwalter */


enum DisplayMode
{
    PleaseChoose,
    PleaseClose,
    TakeProduct
};

class DisplayMapper
{
public:
    DisplayMapper(bool verbose);
    void Print(DisplayMode);
    void SetGivenCoinValue(float value);
    void SetCurrentPrice(float price);
    void Setup();
    void PrintCredit(char *product);

private:
    DisplayMode _currentMode;
    float _CurrentCoinValue;
    float _Price;
    bool _verbose;
    char displayOutput[30];
    void DisplayPleaseLockBox();
    void PrintPleaseChoose();
    void DisplayTakeProduct();
};
#endif