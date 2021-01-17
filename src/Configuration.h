#ifndef Configuration_H
#define Configuration_H
/*
    This libary contains the complete Pin Mapping and also the configuration

    The main purpose is to separate this config from the logic itself.


*/
// The Relais pin, to send a signal top open the door.
#define RELAY_CH1 3
// The coin drawner impulse pini
#define COIN_SELECTOR_PIN 0
// Thie button for opening the box itelf actually limited to one button. 
#define BUTTON_PLUS 4
#define BUTTON_INPUT_BOX_1 5

// The Lock that I use has an separate circuit to check, if the door was locket. 
// It will only break the circuit if the door war open, so I will use these pins for creating a check circuit..
#define BOX_LOCKED_VCC_SIGNAL 7
#define BOX_1_LOCKED_SIGNAL 6

// When enabled, the syystem restore the last state after a system shutdown.
// So this will restore the last inserted value again.
#define USE_EEPROM 0
// The Index value for the EEPROm Save Value.
#define EEPROM_INSERTED_AMOUNT_VALUE_INDEX 0



#endif