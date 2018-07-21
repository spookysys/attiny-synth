#include "myrand.hpp"
#if !defined(AVR)
#include <ctime>
#else
#include <avr/eeprom.h>
#endif

#if defined(AVR)

void EEPROM_write(unsigned char ucAddress, unsigned char ucData)
{
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE));
    /* Set Programming mode */
    EECR = (0<<EEPM1)|(0<<EEPM0);
    /* Set up address and data registers */
    EEARH = 0;
    EEARL = ucAddress;
    EEDR = ucData;
    /* Write logical one to EEMPE */
    EECR |= (1<<EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned char ucAddress)
{
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE));
    /* Set up address register */
    EEARH = 0;
    EEARL = ucAddress;
    /* Start eeprom read by writing EERE */
    EECR |= (1<<EERE);
    /* Return data from data register */
    return EEDR;
}
#endif

namespace myrand {
    States states;

    void srand()
    {
#if defined(AVR)
        union {
            uint8_t bytes[4];
            uint32_t dword;
        } tmp;
        tmp.bytes[0] = EEPROM_read(0);
        tmp.bytes[1] = EEPROM_read(1);
        tmp.bytes[2] = EEPROM_read(2);
        tmp.bytes[3] = EEPROM_read(3);
        tmp.dword++;
        EEPROM_write(0, tmp.bytes[0]);
        EEPROM_write(1, tmp.bytes[1]);
        EEPROM_write(2, tmp.bytes[2]);
        EEPROM_write(3, tmp.bytes[3]);
        states.state32 = tmp.dword;
#else
		states.state32 = time(nullptr);
#endif
    }
}

