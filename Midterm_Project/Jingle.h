/*
 * Jingle.h
 *
 *  Created on: Oct 24, 2018
 *      Author: kendr
 */

#ifndef JINGLE_H_
#define JINGLE_H_

//in ms
#define dotted_half_note    1200
#define half_note           800
#define dotted_quarter_note 600
#define quarter_note        400
#define eighth_note         200
#define BREAK               100

#define Hd                  1174.7
#define Hd_Flat             1108.7
#define Hc                  1046.5
#define Hb                  987.77
#define Hb_Flat             932.33
#define a                   880.00
#define a_Flat              830.61
#define g                   783.99
#define f_Sharp             739.99
#define f                   689.46
#define e                   659.26
#define d_Sharp             622.25
#define b                   493.88
#define b_Flat              466.16

#define MAX_NOTE 32 // How many notes are in the song below

float music_note_sequence[][2] = {{0,       half_note}
                                  {b,       quarter_note}
                                  {e,       dotted_quarter_note}
                                  {g,       eighth_note}
                                  {f_Sharp, quarter_note}
                                  {e,       half_note}
                                  {Hb,      quarter_note}
                                  {a,       dotted_half_note}
                                  {f_Sharp, dotted_half_note}
                                  {e,       dotted_quarter_note}
                                  {g,       eighth_note}
                                  {f_Sharp, quarter_note}
                                  {d_Sharp, half_note}
                                  {f,       quarter_note}
                                  {b,       dotted_half_note}
                                  {0,       half_note}
                                  {b,       quarter_note}
                                  {e,       dotted_quarter_note}
                                  {g,       eighth_note}
                                  {f_Sharp, quarter_note}
                                  {e,       half_note}
                                  {Hb,      quarter_note}
                                  {Hd,      half_note}
                                  {Hd_Flat, quarter_note}
                                  {Hc,      half_note}
                                  {a_Flat,  quarter_note}
                                  {Hc,      dotted_quarter_note}
                                  {Hb,      eighth_note}
                                  {Hb_Flat, quarter_note}
                                  {b_Flat,  half_note}
                                  {g,       quarter_note}
                                  {e,       dotted_half_note}};

int note = 0;       //The note in the music sequence we are on
int breath = 0;     //Take a breath after each note.  This creates seperation

void SetupTimer32s();

#endif /* JINGLE_H_ */
