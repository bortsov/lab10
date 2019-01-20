/*
 * gio_init.cpp
 *
 *  Created on: 25 сент. 2014 г.
 *      Author: bortsov
 */

#include "Gio.h"


namespace gio {

/*
 * Код инициализации взят из Halcogen
 */
void Gio::init ()
{
    /** bring GIO module out of reset */
    base.GCR0   = 1;
    base.ENACLR = 0xFFU;
    base.LVLCLR = 0xFFU;

    /** @b initialize @b Port @b A */

    /** - Port A output values */
    porta.DOUT = 0;

    /** - Port A direction */
    porta.DIR  = 0b00100000;

    /** - Port A open drain enable */
    porta.PDR  = 0;

    /** - Port A pullup / pulldown selection */
    porta.PSL  = 0;

    /** - Port A pullup / pulldown enable*/
    porta.PULDIS  = 0b00100000;

    /** @b initialize @b Port @b B */

    /** - Port B output values */
    portb.DOUT = 0b00000011;

    /** - Port B direction */
    portb.DIR  = 0b00000011;

    /** - Port B open drain enable */
    portb.PDR  = 0;

    /** - Port B pullup / pulldown selection */
    portb.PSL  = 0;

    /** - Port B pullup / pulldown enable*/
    portb.PULDIS  = 0b00000011;

    /** @b initialize @b interrupts */
    base.POL = 0;     /** - interrupt polarity */
    base.LVLSET = 0;  /** - interrupt level */
    base.FLG = 0xFFU; /** - clear all pending interrupts */
    base.ENASET = 0;  /** - enable interrupts */
}

} /* namespace gio */
