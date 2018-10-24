/*
 * DoorBellSelection.c
 *
 *  Created on: Oct 22, 2018
 *      Author: kendr
 */

#include "msp.h"
#include "DoorBellSelection.h"
#include "TimerATimer.h"

void Song()
{
    //Line 1
    GetTimeOnForBell(0,       half_note);
    GetTimeOnForBell(b,       quarter_note);

    GetTimeOnForBell(e,       dotted_quarter_note);
    GetTimeOnForBell(g,       eighth_note);
    GetTimeOnForBell(f_Sharp, quarter_note);

    GetTimeOnForBell(e,       half_note);
    GetTimeOnForBell(Hb,      quarter_note);

    GetTimeOnForBell(a,       dotted_half_note);

    GetTimeOnForBell(f_Sharp, dotted_half_note);

    GetTimeOnForBell(e,       dotted_quarter_note);
    GetTimeOnForBell(g,       eighth_note);
    GetTimeOnForBell(f_Sharp, quarter_note);

    GetTimeOnForBell(d_Sharp, half_note);
    GetTimeOnForBell(f,       quarter_note);

    GetTimeOnForBell(b,       dotted_half_note);

    GetTimeOnForBell(0,       half_note);
    GetTimeOnForBell(b,       quarter_note);

    GetTimeOnForBell(e,       dotted_quarter_note);
    GetTimeOnForBell(g,       eighth_note);
    GetTimeOnForBell(f_Sharp, quarter_note);

    GetTimeOnForBell(e,       half_note);
    GetTimeOnForBell(Hb,      quarter_note);

    //Line 2
    GetTimeOnForBell(Hd,      half_note);
    GetTimeOnForBell(Hd_Flat, quarter_note);

    GetTimeOnForBell(Hc,      half_note);
    GetTimeOnForBell(a_Flat,  quarter_note);

    GetTimeOnForBell(Hc,      dotted_quarter_note);
    GetTimeOnForBell(Hb,      eighth_note);
    GetTimeOnForBell(Hb_Flat, quarter_note);

    GetTimeOnForBell(b_Flat,  half_note);
    GetTimeOnForBell(g,       quarter_note);

    GetTimeOnForBell(e,       dotted_half_note);
}
