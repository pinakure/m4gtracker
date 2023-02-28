#ifndef _CALLBACKS_HPP
#define _CALLBACKS_HPP

#include "callback.hpp"

//------------------------------------------------------------------------------------------//
// TRK CALLBACKS 																			//
//------------------------------------------------------------------------------------------//

#define CURRENT_PATTERN 		VAR_SONG.PATTERNS[CFG::CURRENTCHANNEL].ORDER[VAR_CHANNEL[CFG::CURRENTCHANNEL].POSITION]
#define VARIABLE	(*(u8*) c->var)

#define CALLBACKDEC(a, i)		extern const Callback cb_##a##_0##i ;
#define CALLBACK(a)				extern const Callback cb_##a ;
CALLBACKDEC(ch0_cmd, 0);CALLBACKDEC(ch1_cmd, 0);CALLBACKDEC(ch2_cmd, 0);CALLBACKDEC(ch3_cmd, 0);CALLBACKDEC(ch4_cmd, 0);CALLBACKDEC(ch5_cmd, 0);
CALLBACKDEC(ch0_cmd, 1);CALLBACKDEC(ch1_cmd, 1);CALLBACKDEC(ch2_cmd, 1);CALLBACKDEC(ch3_cmd, 1);CALLBACKDEC(ch4_cmd, 1);CALLBACKDEC(ch5_cmd, 1);
CALLBACKDEC(ch0_cmd, 2);CALLBACKDEC(ch1_cmd, 2);CALLBACKDEC(ch2_cmd, 2);CALLBACKDEC(ch3_cmd, 2);CALLBACKDEC(ch4_cmd, 2);CALLBACKDEC(ch5_cmd, 2);
CALLBACKDEC(ch0_cmd, 3);CALLBACKDEC(ch1_cmd, 3);CALLBACKDEC(ch2_cmd, 3);CALLBACKDEC(ch3_cmd, 3);CALLBACKDEC(ch4_cmd, 3);CALLBACKDEC(ch5_cmd, 3);
CALLBACKDEC(ch0_cmd, 4);CALLBACKDEC(ch1_cmd, 4);CALLBACKDEC(ch2_cmd, 4);CALLBACKDEC(ch3_cmd, 4);CALLBACKDEC(ch4_cmd, 4);CALLBACKDEC(ch5_cmd, 4);
CALLBACKDEC(ch0_cmd, 5);CALLBACKDEC(ch1_cmd, 5);CALLBACKDEC(ch2_cmd, 5);CALLBACKDEC(ch3_cmd, 5);CALLBACKDEC(ch4_cmd, 5);CALLBACKDEC(ch5_cmd, 5);
CALLBACKDEC(ch0_cmd, 6);CALLBACKDEC(ch1_cmd, 6);CALLBACKDEC(ch2_cmd, 6);CALLBACKDEC(ch3_cmd, 6);CALLBACKDEC(ch4_cmd, 6);CALLBACKDEC(ch5_cmd, 6);
CALLBACKDEC(ch0_cmd, 7);CALLBACKDEC(ch1_cmd, 7);CALLBACKDEC(ch2_cmd, 7);CALLBACKDEC(ch3_cmd, 7);CALLBACKDEC(ch4_cmd, 7);CALLBACKDEC(ch5_cmd, 7);
CALLBACKDEC(ch0_cmd, 8);CALLBACKDEC(ch1_cmd, 8);CALLBACKDEC(ch2_cmd, 8);CALLBACKDEC(ch3_cmd, 8);CALLBACKDEC(ch4_cmd, 8);CALLBACKDEC(ch5_cmd, 8);
CALLBACKDEC(ch0_cmd, 9);CALLBACKDEC(ch1_cmd, 9);CALLBACKDEC(ch2_cmd, 9);CALLBACKDEC(ch3_cmd, 9);CALLBACKDEC(ch4_cmd, 9);CALLBACKDEC(ch5_cmd, 9);
CALLBACKDEC(ch0_cmd, A);CALLBACKDEC(ch1_cmd, A);CALLBACKDEC(ch2_cmd, A);CALLBACKDEC(ch3_cmd, A);CALLBACKDEC(ch4_cmd, A);CALLBACKDEC(ch5_cmd, A);
CALLBACKDEC(ch0_cmd, B);CALLBACKDEC(ch1_cmd, B);CALLBACKDEC(ch2_cmd, B);CALLBACKDEC(ch3_cmd, B);CALLBACKDEC(ch4_cmd, B);CALLBACKDEC(ch5_cmd, B);
CALLBACKDEC(ch0_cmd, C);CALLBACKDEC(ch1_cmd, C);CALLBACKDEC(ch2_cmd, C);CALLBACKDEC(ch3_cmd, C);CALLBACKDEC(ch4_cmd, C);CALLBACKDEC(ch5_cmd, C);
CALLBACKDEC(ch0_cmd, D);CALLBACKDEC(ch1_cmd, D);CALLBACKDEC(ch2_cmd, D);CALLBACKDEC(ch3_cmd, D);CALLBACKDEC(ch4_cmd, D);CALLBACKDEC(ch5_cmd, D);
CALLBACKDEC(ch0_cmd, E);CALLBACKDEC(ch1_cmd, E);CALLBACKDEC(ch2_cmd, E);CALLBACKDEC(ch3_cmd, E);CALLBACKDEC(ch4_cmd, E);CALLBACKDEC(ch5_cmd, E);
CALLBACKDEC(ch0_cmd, F);CALLBACKDEC(ch1_cmd, F);CALLBACKDEC(ch2_cmd, F);CALLBACKDEC(ch3_cmd, F);CALLBACKDEC(ch4_cmd, F);CALLBACKDEC(ch5_cmd, F);

CALLBACKDEC(ch0_val, 0);CALLBACKDEC(ch1_val, 0);CALLBACKDEC(ch2_val, 0);CALLBACKDEC(ch3_val, 0);CALLBACKDEC(ch4_val, 0);CALLBACKDEC(ch5_val, 0);
CALLBACKDEC(ch0_val, 1);CALLBACKDEC(ch1_val, 1);CALLBACKDEC(ch2_val, 1);CALLBACKDEC(ch3_val, 1);CALLBACKDEC(ch4_val, 1);CALLBACKDEC(ch5_val, 1);
CALLBACKDEC(ch0_val, 2);CALLBACKDEC(ch1_val, 2);CALLBACKDEC(ch2_val, 2);CALLBACKDEC(ch3_val, 2);CALLBACKDEC(ch4_val, 2);CALLBACKDEC(ch5_val, 2);
CALLBACKDEC(ch0_val, 3);CALLBACKDEC(ch1_val, 3);CALLBACKDEC(ch2_val, 3);CALLBACKDEC(ch3_val, 3);CALLBACKDEC(ch4_val, 3);CALLBACKDEC(ch5_val, 3);
CALLBACKDEC(ch0_val, 4);CALLBACKDEC(ch1_val, 4);CALLBACKDEC(ch2_val, 4);CALLBACKDEC(ch3_val, 4);CALLBACKDEC(ch4_val, 4);CALLBACKDEC(ch5_val, 4);
CALLBACKDEC(ch0_val, 5);CALLBACKDEC(ch1_val, 5);CALLBACKDEC(ch2_val, 5);CALLBACKDEC(ch3_val, 5);CALLBACKDEC(ch4_val, 5);CALLBACKDEC(ch5_val, 5);
CALLBACKDEC(ch0_val, 6);CALLBACKDEC(ch1_val, 6);CALLBACKDEC(ch2_val, 6);CALLBACKDEC(ch3_val, 6);CALLBACKDEC(ch4_val, 6);CALLBACKDEC(ch5_val, 6);
CALLBACKDEC(ch0_val, 7);CALLBACKDEC(ch1_val, 7);CALLBACKDEC(ch2_val, 7);CALLBACKDEC(ch3_val, 7);CALLBACKDEC(ch4_val, 7);CALLBACKDEC(ch5_val, 7);
CALLBACKDEC(ch0_val, 8);CALLBACKDEC(ch1_val, 8);CALLBACKDEC(ch2_val, 8);CALLBACKDEC(ch3_val, 8);CALLBACKDEC(ch4_val, 8);CALLBACKDEC(ch5_val, 8);
CALLBACKDEC(ch0_val, 9);CALLBACKDEC(ch1_val, 9);CALLBACKDEC(ch2_val, 9);CALLBACKDEC(ch3_val, 9);CALLBACKDEC(ch4_val, 9);CALLBACKDEC(ch5_val, 9);
CALLBACKDEC(ch0_val, A);CALLBACKDEC(ch1_val, A);CALLBACKDEC(ch2_val, A);CALLBACKDEC(ch3_val, A);CALLBACKDEC(ch4_val, A);CALLBACKDEC(ch5_val, A);
CALLBACKDEC(ch0_val, B);CALLBACKDEC(ch1_val, B);CALLBACKDEC(ch2_val, B);CALLBACKDEC(ch3_val, B);CALLBACKDEC(ch4_val, B);CALLBACKDEC(ch5_val, B);
CALLBACKDEC(ch0_val, C);CALLBACKDEC(ch1_val, C);CALLBACKDEC(ch2_val, C);CALLBACKDEC(ch3_val, C);CALLBACKDEC(ch4_val, C);CALLBACKDEC(ch5_val, C);
CALLBACKDEC(ch0_val, D);CALLBACKDEC(ch1_val, D);CALLBACKDEC(ch2_val, D);CALLBACKDEC(ch3_val, D);CALLBACKDEC(ch4_val, D);CALLBACKDEC(ch5_val, D);
CALLBACKDEC(ch0_val, E);CALLBACKDEC(ch1_val, E);CALLBACKDEC(ch2_val, E);CALLBACKDEC(ch3_val, E);CALLBACKDEC(ch4_val, E);CALLBACKDEC(ch5_val, E);
CALLBACKDEC(ch0_val, F);CALLBACKDEC(ch1_val, F);CALLBACKDEC(ch2_val, F);CALLBACKDEC(ch3_val, F);CALLBACKDEC(ch4_val, F);CALLBACKDEC(ch5_val, F);

CALLBACKDEC(ch0_vol, 0);CALLBACKDEC(ch1_vol, 0);CALLBACKDEC(ch2_vol, 0);CALLBACKDEC(ch3_vol, 0);CALLBACKDEC(ch4_vol, 0);CALLBACKDEC(ch5_vol, 0);
CALLBACKDEC(ch0_vol, 1);CALLBACKDEC(ch1_vol, 1);CALLBACKDEC(ch2_vol, 1);CALLBACKDEC(ch3_vol, 1);CALLBACKDEC(ch4_vol, 1);CALLBACKDEC(ch5_vol, 1);
CALLBACKDEC(ch0_vol, 2);CALLBACKDEC(ch1_vol, 2);CALLBACKDEC(ch2_vol, 2);CALLBACKDEC(ch3_vol, 2);CALLBACKDEC(ch4_vol, 2);CALLBACKDEC(ch5_vol, 2);
CALLBACKDEC(ch0_vol, 3);CALLBACKDEC(ch1_vol, 3);CALLBACKDEC(ch2_vol, 3);CALLBACKDEC(ch3_vol, 3);CALLBACKDEC(ch4_vol, 3);CALLBACKDEC(ch5_vol, 3);
CALLBACKDEC(ch0_vol, 4);CALLBACKDEC(ch1_vol, 4);CALLBACKDEC(ch2_vol, 4);CALLBACKDEC(ch3_vol, 4);CALLBACKDEC(ch4_vol, 4);CALLBACKDEC(ch5_vol, 4);
CALLBACKDEC(ch0_vol, 5);CALLBACKDEC(ch1_vol, 5);CALLBACKDEC(ch2_vol, 5);CALLBACKDEC(ch3_vol, 5);CALLBACKDEC(ch4_vol, 5);CALLBACKDEC(ch5_vol, 5);
CALLBACKDEC(ch0_vol, 6);CALLBACKDEC(ch1_vol, 6);CALLBACKDEC(ch2_vol, 6);CALLBACKDEC(ch3_vol, 6);CALLBACKDEC(ch4_vol, 6);CALLBACKDEC(ch5_vol, 6);
CALLBACKDEC(ch0_vol, 7);CALLBACKDEC(ch1_vol, 7);CALLBACKDEC(ch2_vol, 7);CALLBACKDEC(ch3_vol, 7);CALLBACKDEC(ch4_vol, 7);CALLBACKDEC(ch5_vol, 7);
CALLBACKDEC(ch0_vol, 8);CALLBACKDEC(ch1_vol, 8);CALLBACKDEC(ch2_vol, 8);CALLBACKDEC(ch3_vol, 8);CALLBACKDEC(ch4_vol, 8);CALLBACKDEC(ch5_vol, 8);
CALLBACKDEC(ch0_vol, 9);CALLBACKDEC(ch1_vol, 9);CALLBACKDEC(ch2_vol, 9);CALLBACKDEC(ch3_vol, 9);CALLBACKDEC(ch4_vol, 9);CALLBACKDEC(ch5_vol, 9);
CALLBACKDEC(ch0_vol, A);CALLBACKDEC(ch1_vol, A);CALLBACKDEC(ch2_vol, A);CALLBACKDEC(ch3_vol, A);CALLBACKDEC(ch4_vol, A);CALLBACKDEC(ch5_vol, A);
CALLBACKDEC(ch0_vol, B);CALLBACKDEC(ch1_vol, B);CALLBACKDEC(ch2_vol, B);CALLBACKDEC(ch3_vol, B);CALLBACKDEC(ch4_vol, B);CALLBACKDEC(ch5_vol, B);
CALLBACKDEC(ch0_vol, C);CALLBACKDEC(ch1_vol, C);CALLBACKDEC(ch2_vol, C);CALLBACKDEC(ch3_vol, C);CALLBACKDEC(ch4_vol, C);CALLBACKDEC(ch5_vol, C);
CALLBACKDEC(ch0_vol, D);CALLBACKDEC(ch1_vol, D);CALLBACKDEC(ch2_vol, D);CALLBACKDEC(ch3_vol, D);CALLBACKDEC(ch4_vol, D);CALLBACKDEC(ch5_vol, D);
CALLBACKDEC(ch0_vol, E);CALLBACKDEC(ch1_vol, E);CALLBACKDEC(ch2_vol, E);CALLBACKDEC(ch3_vol, E);CALLBACKDEC(ch4_vol, E);CALLBACKDEC(ch5_vol, E);
CALLBACKDEC(ch0_vol, F);CALLBACKDEC(ch1_vol, F);CALLBACKDEC(ch2_vol, F);CALLBACKDEC(ch3_vol, F);CALLBACKDEC(ch4_vol, F);CALLBACKDEC(ch5_vol, F);

CALLBACKDEC(ch0_ins, 0);CALLBACKDEC(ch1_ins, 0);CALLBACKDEC(ch2_ins, 0);CALLBACKDEC(ch3_ins, 0);CALLBACKDEC(ch4_ins, 0);CALLBACKDEC(ch5_ins, 0);
CALLBACKDEC(ch0_ins, 1);CALLBACKDEC(ch1_ins, 1);CALLBACKDEC(ch2_ins, 1);CALLBACKDEC(ch3_ins, 1);CALLBACKDEC(ch4_ins, 1);CALLBACKDEC(ch5_ins, 1);
CALLBACKDEC(ch0_ins, 2);CALLBACKDEC(ch1_ins, 2);CALLBACKDEC(ch2_ins, 2);CALLBACKDEC(ch3_ins, 2);CALLBACKDEC(ch4_ins, 2);CALLBACKDEC(ch5_ins, 2);
CALLBACKDEC(ch0_ins, 3);CALLBACKDEC(ch1_ins, 3);CALLBACKDEC(ch2_ins, 3);CALLBACKDEC(ch3_ins, 3);CALLBACKDEC(ch4_ins, 3);CALLBACKDEC(ch5_ins, 3);
CALLBACKDEC(ch0_ins, 4);CALLBACKDEC(ch1_ins, 4);CALLBACKDEC(ch2_ins, 4);CALLBACKDEC(ch3_ins, 4);CALLBACKDEC(ch4_ins, 4);CALLBACKDEC(ch5_ins, 4);
CALLBACKDEC(ch0_ins, 5);CALLBACKDEC(ch1_ins, 5);CALLBACKDEC(ch2_ins, 5);CALLBACKDEC(ch3_ins, 5);CALLBACKDEC(ch4_ins, 5);CALLBACKDEC(ch5_ins, 5);
CALLBACKDEC(ch0_ins, 6);CALLBACKDEC(ch1_ins, 6);CALLBACKDEC(ch2_ins, 6);CALLBACKDEC(ch3_ins, 6);CALLBACKDEC(ch4_ins, 6);CALLBACKDEC(ch5_ins, 6);
CALLBACKDEC(ch0_ins, 7);CALLBACKDEC(ch1_ins, 7);CALLBACKDEC(ch2_ins, 7);CALLBACKDEC(ch3_ins, 7);CALLBACKDEC(ch4_ins, 7);CALLBACKDEC(ch5_ins, 7);
CALLBACKDEC(ch0_ins, 8);CALLBACKDEC(ch1_ins, 8);CALLBACKDEC(ch2_ins, 8);CALLBACKDEC(ch3_ins, 8);CALLBACKDEC(ch4_ins, 8);CALLBACKDEC(ch5_ins, 8);
CALLBACKDEC(ch0_ins, 9);CALLBACKDEC(ch1_ins, 9);CALLBACKDEC(ch2_ins, 9);CALLBACKDEC(ch3_ins, 9);CALLBACKDEC(ch4_ins, 9);CALLBACKDEC(ch5_ins, 9);
CALLBACKDEC(ch0_ins, A);CALLBACKDEC(ch1_ins, A);CALLBACKDEC(ch2_ins, A);CALLBACKDEC(ch3_ins, A);CALLBACKDEC(ch4_ins, A);CALLBACKDEC(ch5_ins, A);
CALLBACKDEC(ch0_ins, B);CALLBACKDEC(ch1_ins, B);CALLBACKDEC(ch2_ins, B);CALLBACKDEC(ch3_ins, B);CALLBACKDEC(ch4_ins, B);CALLBACKDEC(ch5_ins, B);
CALLBACKDEC(ch0_ins, C);CALLBACKDEC(ch1_ins, C);CALLBACKDEC(ch2_ins, C);CALLBACKDEC(ch3_ins, C);CALLBACKDEC(ch4_ins, C);CALLBACKDEC(ch5_ins, C);
CALLBACKDEC(ch0_ins, D);CALLBACKDEC(ch1_ins, D);CALLBACKDEC(ch2_ins, D);CALLBACKDEC(ch3_ins, D);CALLBACKDEC(ch4_ins, D);CALLBACKDEC(ch5_ins, D);
CALLBACKDEC(ch0_ins, E);CALLBACKDEC(ch1_ins, E);CALLBACKDEC(ch2_ins, E);CALLBACKDEC(ch3_ins, E);CALLBACKDEC(ch4_ins, E);CALLBACKDEC(ch5_ins, E);
CALLBACKDEC(ch0_ins, F);CALLBACKDEC(ch1_ins, F);CALLBACKDEC(ch2_ins, F);CALLBACKDEC(ch3_ins, F);CALLBACKDEC(ch4_ins, F);CALLBACKDEC(ch5_ins, F);

CALLBACKDEC(ch0_key, 0);CALLBACKDEC(ch1_key, 0);CALLBACKDEC(ch2_key, 0);CALLBACKDEC(ch3_key, 0);CALLBACKDEC(ch4_key, 0);CALLBACKDEC(ch5_key, 0);
CALLBACKDEC(ch0_key, 1);CALLBACKDEC(ch1_key, 1);CALLBACKDEC(ch2_key, 1);CALLBACKDEC(ch3_key, 1);CALLBACKDEC(ch4_key, 1);CALLBACKDEC(ch5_key, 1);
CALLBACKDEC(ch0_key, 2);CALLBACKDEC(ch1_key, 2);CALLBACKDEC(ch2_key, 2);CALLBACKDEC(ch3_key, 2);CALLBACKDEC(ch4_key, 2);CALLBACKDEC(ch5_key, 2);
CALLBACKDEC(ch0_key, 3);CALLBACKDEC(ch1_key, 3);CALLBACKDEC(ch2_key, 3);CALLBACKDEC(ch3_key, 3);CALLBACKDEC(ch4_key, 3);CALLBACKDEC(ch5_key, 3);
CALLBACKDEC(ch0_key, 4);CALLBACKDEC(ch1_key, 4);CALLBACKDEC(ch2_key, 4);CALLBACKDEC(ch3_key, 4);CALLBACKDEC(ch4_key, 4);CALLBACKDEC(ch5_key, 4);
CALLBACKDEC(ch0_key, 5);CALLBACKDEC(ch1_key, 5);CALLBACKDEC(ch2_key, 5);CALLBACKDEC(ch3_key, 5);CALLBACKDEC(ch4_key, 5);CALLBACKDEC(ch5_key, 5);
CALLBACKDEC(ch0_key, 6);CALLBACKDEC(ch1_key, 6);CALLBACKDEC(ch2_key, 6);CALLBACKDEC(ch3_key, 6);CALLBACKDEC(ch4_key, 6);CALLBACKDEC(ch5_key, 6);
CALLBACKDEC(ch0_key, 7);CALLBACKDEC(ch1_key, 7);CALLBACKDEC(ch2_key, 7);CALLBACKDEC(ch3_key, 7);CALLBACKDEC(ch4_key, 7);CALLBACKDEC(ch5_key, 7);
CALLBACKDEC(ch0_key, 8);CALLBACKDEC(ch1_key, 8);CALLBACKDEC(ch2_key, 8);CALLBACKDEC(ch3_key, 8);CALLBACKDEC(ch4_key, 8);CALLBACKDEC(ch5_key, 8);
CALLBACKDEC(ch0_key, 9);CALLBACKDEC(ch1_key, 9);CALLBACKDEC(ch2_key, 9);CALLBACKDEC(ch3_key, 9);CALLBACKDEC(ch4_key, 9);CALLBACKDEC(ch5_key, 9);
CALLBACKDEC(ch0_key, A);CALLBACKDEC(ch1_key, A);CALLBACKDEC(ch2_key, A);CALLBACKDEC(ch3_key, A);CALLBACKDEC(ch4_key, A);CALLBACKDEC(ch5_key, A);
CALLBACKDEC(ch0_key, B);CALLBACKDEC(ch1_key, B);CALLBACKDEC(ch2_key, B);CALLBACKDEC(ch3_key, B);CALLBACKDEC(ch4_key, B);CALLBACKDEC(ch5_key, B);
CALLBACKDEC(ch0_key, C);CALLBACKDEC(ch1_key, C);CALLBACKDEC(ch2_key, C);CALLBACKDEC(ch3_key, C);CALLBACKDEC(ch4_key, C);CALLBACKDEC(ch5_key, C);
CALLBACKDEC(ch0_key, D);CALLBACKDEC(ch1_key, D);CALLBACKDEC(ch2_key, D);CALLBACKDEC(ch3_key, D);CALLBACKDEC(ch4_key, D);CALLBACKDEC(ch5_key, D);
CALLBACKDEC(ch0_key, E);CALLBACKDEC(ch1_key, E);CALLBACKDEC(ch2_key, E);CALLBACKDEC(ch3_key, E);CALLBACKDEC(ch4_key, E);CALLBACKDEC(ch5_key, E);
CALLBACKDEC(ch0_key, F);CALLBACKDEC(ch1_key, F);CALLBACKDEC(ch2_key, F);CALLBACKDEC(ch3_key, F);CALLBACKDEC(ch4_key, F);CALLBACKDEC(ch5_key, F);

//------------------------------------------------------------------------------------------//
// PATT CALLBACKS 																			//
//------------------------------------------------------------------------------------------//
CALLBACKDEC(patterns_0, 0); CALLBACKDEC(patterns_1, 0); CALLBACKDEC(patterns_2, 0); CALLBACKDEC(patterns_3, 0); CALLBACKDEC(patterns_4, 0); CALLBACKDEC(patterns_5, 0); 
CALLBACKDEC(patterns_0, 1); CALLBACKDEC(patterns_1, 1); CALLBACKDEC(patterns_2, 1); CALLBACKDEC(patterns_3, 1); CALLBACKDEC(patterns_4, 1); CALLBACKDEC(patterns_5, 1); 
CALLBACKDEC(patterns_0, 2); CALLBACKDEC(patterns_1, 2); CALLBACKDEC(patterns_2, 2); CALLBACKDEC(patterns_3, 2); CALLBACKDEC(patterns_4, 2); CALLBACKDEC(patterns_5, 2); 
CALLBACKDEC(patterns_0, 3); CALLBACKDEC(patterns_1, 3); CALLBACKDEC(patterns_2, 3); CALLBACKDEC(patterns_3, 3); CALLBACKDEC(patterns_4, 3); CALLBACKDEC(patterns_5, 3); 
CALLBACKDEC(patterns_0, 4); CALLBACKDEC(patterns_1, 4); CALLBACKDEC(patterns_2, 4); CALLBACKDEC(patterns_3, 4); CALLBACKDEC(patterns_4, 4); CALLBACKDEC(patterns_5, 4); 
CALLBACKDEC(patterns_0, 5); CALLBACKDEC(patterns_1, 5); CALLBACKDEC(patterns_2, 5); CALLBACKDEC(patterns_3, 5); CALLBACKDEC(patterns_4, 5); CALLBACKDEC(patterns_5, 5); 
CALLBACKDEC(patterns_0, 6); CALLBACKDEC(patterns_1, 6); CALLBACKDEC(patterns_2, 6); CALLBACKDEC(patterns_3, 6); CALLBACKDEC(patterns_4, 6); CALLBACKDEC(patterns_5, 6); 
CALLBACKDEC(patterns_0, 7); CALLBACKDEC(patterns_1, 7); CALLBACKDEC(patterns_2, 7); CALLBACKDEC(patterns_3, 7); CALLBACKDEC(patterns_4, 7); CALLBACKDEC(patterns_5, 7); 
CALLBACKDEC(patterns_0, 8); CALLBACKDEC(patterns_1, 8); CALLBACKDEC(patterns_2, 8); CALLBACKDEC(patterns_3, 8); CALLBACKDEC(patterns_4, 8); CALLBACKDEC(patterns_5, 8); 
CALLBACKDEC(patterns_0, 9); CALLBACKDEC(patterns_1, 9); CALLBACKDEC(patterns_2, 9); CALLBACKDEC(patterns_3, 9); CALLBACKDEC(patterns_4, 9); CALLBACKDEC(patterns_5, 9); 
CALLBACKDEC(patterns_0, A); CALLBACKDEC(patterns_1, A); CALLBACKDEC(patterns_2, A); CALLBACKDEC(patterns_3, A); CALLBACKDEC(patterns_4, A); CALLBACKDEC(patterns_5, A); 
CALLBACKDEC(patterns_0, B); CALLBACKDEC(patterns_1, B); CALLBACKDEC(patterns_2, B); CALLBACKDEC(patterns_3, B); CALLBACKDEC(patterns_4, B); CALLBACKDEC(patterns_5, B); 
CALLBACKDEC(patterns_0, C); CALLBACKDEC(patterns_1, C); CALLBACKDEC(patterns_2, C); CALLBACKDEC(patterns_3, C); CALLBACKDEC(patterns_4, C); CALLBACKDEC(patterns_5, C); 
CALLBACKDEC(patterns_0, D); CALLBACKDEC(patterns_1, D); CALLBACKDEC(patterns_2, D); CALLBACKDEC(patterns_3, D); CALLBACKDEC(patterns_4, D); CALLBACKDEC(patterns_5, D); 
CALLBACKDEC(patterns_0, E); CALLBACKDEC(patterns_1, E); CALLBACKDEC(patterns_2, E); CALLBACKDEC(patterns_3, E); CALLBACKDEC(patterns_4, E); CALLBACKDEC(patterns_5, E); 
CALLBACKDEC(patterns_0, F); CALLBACKDEC(patterns_1, F); CALLBACKDEC(patterns_2, F); CALLBACKDEC(patterns_3, F); CALLBACKDEC(patterns_4, F); CALLBACKDEC(patterns_5, F); 

CALLBACK(pat_solo_0);
CALLBACK(pat_solo_1);
CALLBACK(pat_solo_2);
CALLBACK(pat_solo_3);
CALLBACK(pat_solo_4);
CALLBACK(pat_solo_5);
CALLBACK(pat_mute_0);
CALLBACK(pat_mute_1);
CALLBACK(pat_mute_2);
CALLBACK(pat_mute_3);
CALLBACK(pat_mute_4);
CALLBACK(pat_mute_5);

//------------------------------------------------------------------------------------------//
// INST CALLBACKS 																			//
//------------------------------------------------------------------------------------------//

CALLBACK(ins_index			);
CALLBACK(ins_name			);
CALLBACK(ins_type			);
CALLBACK(ins_volumefade		);
CALLBACK(ins_tspenvelope	);
CALLBACK(ins_tspmode		);
CALLBACK(ins_tspenable		);
CALLBACK(ins_volenable		);
CALLBACK(ins_volmode		);
CALLBACK(ins_volenvelope	);
CALLBACK(ins_dutycycle		);
CALLBACK(ins_length			);
CALLBACK(ins_sweepdir		);
CALLBACK(ins_sweepspeed		);
CALLBACK(ins_sweepsteps 	);
CALLBACK(ins_pwm_level		);
CALLBACK(ins_envelopedir	);
CALLBACK(ins_phase			);
CALLBACK(ins_am				);
CALLBACK(ins_mixpercent		);
CALLBACK(ins_wav_op1type	);
CALLBACK(ins_wav_op2type	);
CALLBACK(ins_wav_op3type	);
CALLBACK(ins_wav_op4type	);
CALLBACK(ins_wav_preset0	);
CALLBACK(ins_wav_preset1	);
CALLBACK(ins_wav_preset2	);
CALLBACK(ins_wav_preset3	);
CALLBACK(ins_wav_preset4	);
CALLBACK(ins_wav_preset5	);
CALLBACK(ins_algorithm		);
CALLBACK(ins_fm_mult		);
CALLBACK(ins_fm_op1type		);
CALLBACK(ins_fm_op2type		);
CALLBACK(ins_fm_op3type		);
CALLBACK(ins_fm_op4type		);
CALLBACK(ins_smp_start		);
CALLBACK(ins_smp_end		);
CALLBACK(ins_smp_frequency	);
CALLBACK(ins_smp_b			);
CALLBACK(ins_smp_s			);
CALLBACK(ins_smp_r			);
CALLBACK(ins_smp_synthmode	);
CALLBACK(ins_smp_loop		);
CALLBACK(ins_smp_kit		);
CALLBACK(ins_smp_adsr_0		);
CALLBACK(ins_smp_adsr_1		);
CALLBACK(ins_smp_adsr_2		);
CALLBACK(ins_smp_adsr_3		);

CALLBACKDEC(ins_table_transpose, 0); CALLBACKDEC(ins_table_volume, 0); CALLBACKDEC(ins_table_value1, 0); CALLBACKDEC(ins_table_value2, 0); CALLBACKDEC(ins_table_command1, 0); CALLBACKDEC(ins_table_command2, 0);
CALLBACKDEC(ins_table_transpose, 1); CALLBACKDEC(ins_table_volume, 1); CALLBACKDEC(ins_table_value1, 1); CALLBACKDEC(ins_table_value2, 1); CALLBACKDEC(ins_table_command1, 1); CALLBACKDEC(ins_table_command2, 1);
CALLBACKDEC(ins_table_transpose, 2); CALLBACKDEC(ins_table_volume, 2); CALLBACKDEC(ins_table_value1, 2); CALLBACKDEC(ins_table_value2, 2); CALLBACKDEC(ins_table_command1, 2); CALLBACKDEC(ins_table_command2, 2);
CALLBACKDEC(ins_table_transpose, 3); CALLBACKDEC(ins_table_volume, 3); CALLBACKDEC(ins_table_value1, 3); CALLBACKDEC(ins_table_value2, 3); CALLBACKDEC(ins_table_command1, 3); CALLBACKDEC(ins_table_command2, 3);
CALLBACKDEC(ins_table_transpose, 4); CALLBACKDEC(ins_table_volume, 4); CALLBACKDEC(ins_table_value1, 4); CALLBACKDEC(ins_table_value2, 4); CALLBACKDEC(ins_table_command1, 4); CALLBACKDEC(ins_table_command2, 4);
CALLBACKDEC(ins_table_transpose, 5); CALLBACKDEC(ins_table_volume, 5); CALLBACKDEC(ins_table_value1, 5); CALLBACKDEC(ins_table_value2, 5); CALLBACKDEC(ins_table_command1, 5); CALLBACKDEC(ins_table_command2, 5);
CALLBACKDEC(ins_table_transpose, 6); CALLBACKDEC(ins_table_volume, 6); CALLBACKDEC(ins_table_value1, 6); CALLBACKDEC(ins_table_value2, 6); CALLBACKDEC(ins_table_command1, 6); CALLBACKDEC(ins_table_command2, 6);
CALLBACKDEC(ins_table_transpose, 7); CALLBACKDEC(ins_table_volume, 7); CALLBACKDEC(ins_table_value1, 7); CALLBACKDEC(ins_table_value2, 7); CALLBACKDEC(ins_table_command1, 7); CALLBACKDEC(ins_table_command2, 7);
CALLBACKDEC(ins_table_transpose, 8); CALLBACKDEC(ins_table_volume, 8); CALLBACKDEC(ins_table_value1, 8); CALLBACKDEC(ins_table_value2, 8); CALLBACKDEC(ins_table_command1, 8); CALLBACKDEC(ins_table_command2, 8);
CALLBACKDEC(ins_table_transpose, 9); CALLBACKDEC(ins_table_volume, 9); CALLBACKDEC(ins_table_value1, 9); CALLBACKDEC(ins_table_value2, 9); CALLBACKDEC(ins_table_command1, 9); CALLBACKDEC(ins_table_command2, 9);
CALLBACKDEC(ins_table_transpose, A); CALLBACKDEC(ins_table_volume, A); CALLBACKDEC(ins_table_value1, A); CALLBACKDEC(ins_table_value2, A); CALLBACKDEC(ins_table_command1, A); CALLBACKDEC(ins_table_command2, A);
CALLBACKDEC(ins_table_transpose, B); CALLBACKDEC(ins_table_volume, B); CALLBACKDEC(ins_table_value1, B); CALLBACKDEC(ins_table_value2, B); CALLBACKDEC(ins_table_command1, B); CALLBACKDEC(ins_table_command2, B);
CALLBACKDEC(ins_table_transpose, C); CALLBACKDEC(ins_table_volume, C); CALLBACKDEC(ins_table_value1, C); CALLBACKDEC(ins_table_value2, C); CALLBACKDEC(ins_table_command1, C); CALLBACKDEC(ins_table_command2, C);
CALLBACKDEC(ins_table_transpose, D); CALLBACKDEC(ins_table_volume, D); CALLBACKDEC(ins_table_value1, D); CALLBACKDEC(ins_table_value2, D); CALLBACKDEC(ins_table_command1, D); CALLBACKDEC(ins_table_command2, D);
CALLBACKDEC(ins_table_transpose, E); CALLBACKDEC(ins_table_volume, E); CALLBACKDEC(ins_table_value1, E); CALLBACKDEC(ins_table_value2, E); CALLBACKDEC(ins_table_command1, E); CALLBACKDEC(ins_table_command2, E);
CALLBACKDEC(ins_table_transpose, F); CALLBACKDEC(ins_table_volume, F); CALLBACKDEC(ins_table_value1, F); CALLBACKDEC(ins_table_value2, F); CALLBACKDEC(ins_table_command1, F); CALLBACKDEC(ins_table_command2, F);

CALLBACKDEC(ins_tsp, 0); CALLBACKDEC(ins_vol, 0); CALLBACK(ins_wav_op1adsr_0); CALLBACK(ins_fm_op1adsr_0); CALLBACKDEC(ins_smpkit, 0);
CALLBACKDEC(ins_tsp, 1); CALLBACKDEC(ins_vol, 1); CALLBACK(ins_wav_op1adsr_1); CALLBACK(ins_fm_op1adsr_1); CALLBACKDEC(ins_smpkit, 1);
CALLBACKDEC(ins_tsp, 2); CALLBACKDEC(ins_vol, 2); CALLBACK(ins_wav_op1adsr_2); CALLBACK(ins_fm_op1adsr_2); CALLBACKDEC(ins_smpkit, 2);
CALLBACKDEC(ins_tsp, 3); CALLBACKDEC(ins_vol, 3); CALLBACK(ins_wav_op1adsr_3); CALLBACK(ins_fm_op1adsr_3); CALLBACKDEC(ins_smpkit, 3);
CALLBACKDEC(ins_tsp, 4); CALLBACKDEC(ins_vol, 4); CALLBACK(ins_wav_op2adsr_0); CALLBACK(ins_fm_op2adsr_0); CALLBACKDEC(ins_smpkit, 4);
CALLBACKDEC(ins_tsp, 5); CALLBACKDEC(ins_vol, 5); CALLBACK(ins_wav_op2adsr_1); CALLBACK(ins_fm_op2adsr_1); CALLBACKDEC(ins_smpkit, 5);
CALLBACKDEC(ins_tsp, 6); CALLBACKDEC(ins_vol, 6); CALLBACK(ins_wav_op2adsr_2); CALLBACK(ins_fm_op2adsr_2); CALLBACKDEC(ins_smpkit, 6);
CALLBACKDEC(ins_tsp, 7); CALLBACKDEC(ins_vol, 7); CALLBACK(ins_wav_op2adsr_3); CALLBACK(ins_fm_op2adsr_3); CALLBACKDEC(ins_smpkit, 7);
CALLBACKDEC(ins_tsp, 8); CALLBACKDEC(ins_vol, 8); CALLBACK(ins_wav_op3adsr_0); CALLBACK(ins_fm_op3adsr_0); CALLBACKDEC(ins_smpkit, 8);
CALLBACKDEC(ins_tsp, 9); CALLBACKDEC(ins_vol, 9); CALLBACK(ins_wav_op3adsr_1); CALLBACK(ins_fm_op3adsr_1); CALLBACKDEC(ins_smpkit, 9);
CALLBACKDEC(ins_tsp, A); CALLBACKDEC(ins_vol, A); CALLBACK(ins_wav_op3adsr_2); CALLBACK(ins_fm_op3adsr_2); CALLBACKDEC(ins_smpkit, A);
CALLBACKDEC(ins_tsp, B); CALLBACKDEC(ins_vol, B); CALLBACK(ins_wav_op3adsr_3); CALLBACK(ins_fm_op3adsr_3); CALLBACKDEC(ins_smpkit, B);
CALLBACKDEC(ins_tsp, C); CALLBACKDEC(ins_vol, C); CALLBACK(ins_wav_op4adsr_0); CALLBACK(ins_fm_op4adsr_0);
CALLBACKDEC(ins_tsp, D); CALLBACKDEC(ins_vol, D); CALLBACK(ins_wav_op4adsr_1); CALLBACK(ins_fm_op4adsr_1);
CALLBACKDEC(ins_tsp, E); CALLBACKDEC(ins_vol, E); CALLBACK(ins_wav_op4adsr_2); CALLBACK(ins_fm_op4adsr_2);
CALLBACKDEC(ins_tsp, F); CALLBACKDEC(ins_vol, F); CALLBACK(ins_wav_op4adsr_3); CALLBACK(ins_fm_op4adsr_3);

//------------------------------------------------------------------------------------------//
// CFG CALLBACKS 																			//
//------------------------------------------------------------------------------------------//
CALLBACK(cfg_menuindex	);
CALLBACK(cfg_interface	);
CALLBACK(cfg_font		);
CALLBACK(cfg_border		);
CALLBACK(cfg_showlogo	);
CALLBACK(cfg_startupsfx	);
CALLBACK(cfg_coloreditor);
CALLBACK(cfg_linkmode	);
CALLBACK(cfg_masterclock);
CALLBACK(cfg_midichan	);
CALLBACK(cfg_clocktempo );
CALLBACK(cfg_receivesong);
CALLBACK(cfg_sendsong	);
CALLBACK(cfg_autoload	);
CALLBACK(cfg_keyrate	);
CALLBACK(cfg_buttonset	);
CALLBACK(cfg_saveconfig	);
CALLBACK(cfg_loadconfig	);
CALLBACK(cfg_initconfig	);
CALLBACK(cfg_finetune	);
CALLBACK(cfg_prelisten	);
CALLBACK(cfg_transpose	);
CALLBACK(cfg_inputmode	);
CALLBACK(cfg_soundbias	);
CALLBACK(cfg_mixer		);
CALLBACK(cfg_prefetch	);
CALLBACK(cfg_backup		);
CALLBACK(cfg_revert		);
CALLBACK(cfg_memorytest	);
CALLBACK(cfg_format		);
CALLBACK(cfg_reset		);

//------------------------------------------------------------------------------------------//
// LIVE CALLBACKS 																			//
//------------------------------------------------------------------------------------------//
CALLBACK(live_chan1		);
CALLBACK(live_chan2		);
CALLBACK(live_mode		);
CALLBACK(live_octave	);
CALLBACK(live_transpose );
CALLBACK(live_quantize2 );
CALLBACK(live_midichan1	);
CALLBACK(live_midichan2	);
CALLBACK(live_retrig	);
CALLBACK(live_speed	 	);
CALLBACK(live_quantize1	);

CALLBACKDEC(live_left_VAL  , 0); CALLBACKDEC(live_left_VOL  , 0); CALLBACKDEC(live_left_INS  , 0); CALLBACKDEC(live_left_CMD  , 0); CALLBACKDEC(live_left_CHAN  , 0); CALLBACKDEC(live_left_KEY  , 0);
CALLBACKDEC(live_left_VAL  , 1); CALLBACKDEC(live_left_VOL  , 1); CALLBACKDEC(live_left_INS  , 1); CALLBACKDEC(live_left_CMD  , 1); CALLBACKDEC(live_left_CHAN  , 1); CALLBACKDEC(live_left_KEY  , 1);
CALLBACKDEC(live_left_VAL  , 2); CALLBACKDEC(live_left_VOL  , 2); CALLBACKDEC(live_left_INS  , 2); CALLBACKDEC(live_left_CMD  , 2); CALLBACKDEC(live_left_CHAN  , 2); CALLBACKDEC(live_left_KEY  , 2);
CALLBACKDEC(live_left_VAL  , 3); CALLBACKDEC(live_left_VOL  , 3); CALLBACKDEC(live_left_INS  , 3); CALLBACKDEC(live_left_CMD  , 3); CALLBACKDEC(live_left_CHAN  , 3); CALLBACKDEC(live_left_KEY  , 3);
CALLBACKDEC(live_left_VAL  , 4); CALLBACKDEC(live_left_VOL  , 4); CALLBACKDEC(live_left_INS  , 4); CALLBACKDEC(live_left_CMD  , 4); CALLBACKDEC(live_left_CHAN  , 4); CALLBACKDEC(live_left_KEY  , 4);
CALLBACKDEC(live_left_VAL  , 5); CALLBACKDEC(live_left_VOL  , 5); CALLBACKDEC(live_left_INS  , 5); CALLBACKDEC(live_left_CMD  , 5); CALLBACKDEC(live_left_CHAN  , 5); CALLBACKDEC(live_left_KEY  , 5);
CALLBACKDEC(live_left_VAL  , 6); CALLBACKDEC(live_left_VOL  , 6); CALLBACKDEC(live_left_INS  , 6); CALLBACKDEC(live_left_CMD  , 6); CALLBACKDEC(live_left_CHAN  , 6); CALLBACKDEC(live_left_KEY  , 6);
CALLBACKDEC(live_left_VAL  , 7); CALLBACKDEC(live_left_VOL  , 7); CALLBACKDEC(live_left_INS  , 7); CALLBACKDEC(live_left_CMD  , 7); CALLBACKDEC(live_left_CHAN  , 7); CALLBACKDEC(live_left_KEY  , 7);
CALLBACKDEC(live_left_VAL  , 8); CALLBACKDEC(live_left_VOL  , 8); CALLBACKDEC(live_left_INS  , 8); CALLBACKDEC(live_left_CMD  , 8); CALLBACKDEC(live_left_CHAN  , 8); CALLBACKDEC(live_left_KEY  , 8);
CALLBACKDEC(live_left_VAL  , 9); CALLBACKDEC(live_left_VOL  , 9); CALLBACKDEC(live_left_INS  , 9); CALLBACKDEC(live_left_CMD  , 9); CALLBACKDEC(live_left_CHAN  , 9); CALLBACKDEC(live_left_KEY  , 9);
CALLBACKDEC(live_left_VAL  , A); CALLBACKDEC(live_left_VOL  , A); CALLBACKDEC(live_left_INS  , A); CALLBACKDEC(live_left_CMD  , A); CALLBACKDEC(live_left_CHAN  , A); CALLBACKDEC(live_left_KEY  , A);
CALLBACKDEC(live_left_VAL  , B); CALLBACKDEC(live_left_VOL  , B); CALLBACKDEC(live_left_INS  , B); CALLBACKDEC(live_left_CMD  , B); CALLBACKDEC(live_left_CHAN  , B); CALLBACKDEC(live_left_KEY  , B);
CALLBACKDEC(live_left_VAL  , C); CALLBACKDEC(live_left_VOL  , C); CALLBACKDEC(live_left_INS  , C); CALLBACKDEC(live_left_CMD  , C); CALLBACKDEC(live_left_CHAN  , C); CALLBACKDEC(live_left_KEY  , C);
CALLBACKDEC(live_left_VAL  , D); CALLBACKDEC(live_left_VOL  , D); CALLBACKDEC(live_left_INS  , D); CALLBACKDEC(live_left_CMD  , D); CALLBACKDEC(live_left_CHAN  , D); CALLBACKDEC(live_left_KEY  , D);
CALLBACKDEC(live_left_VAL  , E); CALLBACKDEC(live_left_VOL  , E); CALLBACKDEC(live_left_INS  , E); CALLBACKDEC(live_left_CMD  , E); CALLBACKDEC(live_left_CHAN  , E); CALLBACKDEC(live_left_KEY  , E);
CALLBACKDEC(live_left_VAL  , F); CALLBACKDEC(live_left_VOL  , F); CALLBACKDEC(live_left_INS  , F); CALLBACKDEC(live_left_CMD  , F); CALLBACKDEC(live_left_CHAN  , F); CALLBACKDEC(live_left_KEY  , F);

CALLBACKDEC(live_right_VAL , 0); CALLBACKDEC(live_right_VOL , 0); CALLBACKDEC(live_right_INS , 0); CALLBACKDEC(live_right_CMD , 0); CALLBACKDEC(live_right_CHAN , 0); CALLBACKDEC(live_right_KEY , 0);
CALLBACKDEC(live_right_VAL , 1); CALLBACKDEC(live_right_VOL , 1); CALLBACKDEC(live_right_INS , 1); CALLBACKDEC(live_right_CMD , 1); CALLBACKDEC(live_right_CHAN , 1); CALLBACKDEC(live_right_KEY , 1);
CALLBACKDEC(live_right_VAL , 2); CALLBACKDEC(live_right_VOL , 2); CALLBACKDEC(live_right_INS , 2); CALLBACKDEC(live_right_CMD , 2); CALLBACKDEC(live_right_CHAN , 2); CALLBACKDEC(live_right_KEY , 2);
CALLBACKDEC(live_right_VAL , 3); CALLBACKDEC(live_right_VOL , 3); CALLBACKDEC(live_right_INS , 3); CALLBACKDEC(live_right_CMD , 3); CALLBACKDEC(live_right_CHAN , 3); CALLBACKDEC(live_right_KEY , 3);
CALLBACKDEC(live_right_VAL , 4); CALLBACKDEC(live_right_VOL , 4); CALLBACKDEC(live_right_INS , 4); CALLBACKDEC(live_right_CMD , 4); CALLBACKDEC(live_right_CHAN , 4); CALLBACKDEC(live_right_KEY , 4);
CALLBACKDEC(live_right_VAL , 5); CALLBACKDEC(live_right_VOL , 5); CALLBACKDEC(live_right_INS , 5); CALLBACKDEC(live_right_CMD , 5); CALLBACKDEC(live_right_CHAN , 5); CALLBACKDEC(live_right_KEY , 5);
CALLBACKDEC(live_right_VAL , 6); CALLBACKDEC(live_right_VOL , 6); CALLBACKDEC(live_right_INS , 6); CALLBACKDEC(live_right_CMD , 6); CALLBACKDEC(live_right_CHAN , 6); CALLBACKDEC(live_right_KEY , 6);
CALLBACKDEC(live_right_VAL , 7); CALLBACKDEC(live_right_VOL , 7); CALLBACKDEC(live_right_INS , 7); CALLBACKDEC(live_right_CMD , 7); CALLBACKDEC(live_right_CHAN , 7); CALLBACKDEC(live_right_KEY , 7);
CALLBACKDEC(live_right_VAL , 8); CALLBACKDEC(live_right_VOL , 8); CALLBACKDEC(live_right_INS , 8); CALLBACKDEC(live_right_CMD , 8); CALLBACKDEC(live_right_CHAN , 8); CALLBACKDEC(live_right_KEY , 8);
CALLBACKDEC(live_right_VAL , 9); CALLBACKDEC(live_right_VOL , 9); CALLBACKDEC(live_right_INS , 9); CALLBACKDEC(live_right_CMD , 9); CALLBACKDEC(live_right_CHAN , 9); CALLBACKDEC(live_right_KEY , 9);
CALLBACKDEC(live_right_VAL , A); CALLBACKDEC(live_right_VOL , A); CALLBACKDEC(live_right_INS , A); CALLBACKDEC(live_right_CMD , A); CALLBACKDEC(live_right_CHAN , A); CALLBACKDEC(live_right_KEY , A);
CALLBACKDEC(live_right_VAL , B); CALLBACKDEC(live_right_VOL , B); CALLBACKDEC(live_right_INS , B); CALLBACKDEC(live_right_CMD , B); CALLBACKDEC(live_right_CHAN , B); CALLBACKDEC(live_right_KEY , B);
CALLBACKDEC(live_right_VAL , C); CALLBACKDEC(live_right_VOL , C); CALLBACKDEC(live_right_INS , C); CALLBACKDEC(live_right_CMD , C); CALLBACKDEC(live_right_CHAN , C); CALLBACKDEC(live_right_KEY , C);
CALLBACKDEC(live_right_VAL , D); CALLBACKDEC(live_right_VOL , D); CALLBACKDEC(live_right_INS , D); CALLBACKDEC(live_right_CMD , D); CALLBACKDEC(live_right_CHAN , D); CALLBACKDEC(live_right_KEY , D);
CALLBACKDEC(live_right_VAL , E); CALLBACKDEC(live_right_VOL , E); CALLBACKDEC(live_right_INS , E); CALLBACKDEC(live_right_CMD , E); CALLBACKDEC(live_right_CHAN , E); CALLBACKDEC(live_right_KEY , E);
CALLBACKDEC(live_right_VAL , F); CALLBACKDEC(live_right_VOL , F); CALLBACKDEC(live_right_INS , F); CALLBACKDEC(live_right_CMD , F); CALLBACKDEC(live_right_CHAN , F); CALLBACKDEC(live_right_KEY , F);

//------------------------------------------------------------------------------------------//
// SONG CALLBACKS 																			//
//------------------------------------------------------------------------------------------//
CALLBACK(sng_load		);
CALLBACK(sng_save		);
CALLBACK(sng_purge		);
CALLBACK(sng_copy		);
CALLBACK(sng_erase		);
CALLBACK(sng_slot		);
CALLBACK(sng_artist		);
CALLBACK(sng_title		);
CALLBACK(sng_tempotap	);
CALLBACK(sng_patlength	);
CALLBACK(sng_transpose	);
CALLBACK(sng_bpm		);
CALLBACK(sng_groove		);

CALLBACKDEC(sng_groove, 0);
CALLBACKDEC(sng_groove, 1);
CALLBACKDEC(sng_groove, 2);
CALLBACKDEC(sng_groove, 3);
CALLBACKDEC(sng_groove, 4);
CALLBACKDEC(sng_groove, 5);
CALLBACKDEC(sng_groove, 6);
CALLBACKDEC(sng_groove, 7);
CALLBACKDEC(sng_groove, 8);
CALLBACKDEC(sng_groove, 9);
CALLBACKDEC(sng_groove, A);
CALLBACKDEC(sng_groove, B);
CALLBACKDEC(sng_groove, C);
CALLBACKDEC(sng_groove, D);
CALLBACKDEC(sng_groove, E);
CALLBACKDEC(sng_groove, F);

#undef CALLBACKDEC
#undef CALLBACK

void updateLive1();
void updateLive2();
void updateHlp();
void updateVis();
void updateSnk();
void updateInsWav();
void updateInsFmw();
void updateInsPwm();
void updateInsSmp();
void updateTable();
void updateSng();
void updateBehavior();
void updateLookNFeel();
void updateLinkMode();
void updateTracker();
void updateMemory();
void updateColorEditor();
void updateChannelMixer();
void updateMemorySongMap();
void updateMemoryTest();
void updateLinkStatus();
void updateCredits();
void updateChannel0();
void updateChannel1();
void updateChannel2();
void updateChannel3();
void updateChannel4();
void updateChannel5();
void updateHeader0();
void updateHeader1();
void updatePat();
void patGlobalUpdater();
void instLoad();
void cellSync();
void patternSync(u8 position);
void patternCopy(u8 channel);


void STRING					(bool big, u8 x, u8 y, u8 *data);

void PIXEL					(u8 x, u8 y, u16 color, u16 value);
void VISPOS2				(u8 x, u8 y, u16 color, u16 value);
void VISPOS1				(u8 x, u8 y, u16 color, u16 value);
void NOTE					(u8 x, u8 y, u16 color, u16 value);
void TABLEPOS				(u8 x, u8 y, u16 color, u16 value);
void COMMAND				(u8 x, u8 y, u16 color, u16 value);	
void VUMETER_V1				(u8 x, u8 y, u16 color, u16 value);	
void VUMETER_H1				(u8 x, u8 y, u16 color, u16 value);	
void VUMETER_H3				(u8 x, u8 y, u16 color, u16 value);
void VUMETER_V3				(u8 x, u8 y, u16 color, u16 value);
void TEXT					(u8 x, u8 y, u16 color, u16 value);
void BIGTEXT				(u8 x, u8 y, u16 color, u16 value);
void BIGDEC					(u8 x, u8 y, u16 color, u16 value);
void BIGHEX					(u8 x, u8 y, u16 color, u16 value);
void NIBBLE					(u8 x, u8 y, u16 color, u16 value);
void WAVE_SINGLE			(u8 x, u8 y, u16 color, u16 value);
void WAVE_DOUBLE			(u8 x, u8 y, u16 color, u16 value);
void PROGRESS				(u8 x, u8 y, u16 color, u16 value);
void STATUS					(u8 x, u8 y, u16 color, u16 value);
void HEXADECIMAL			(u8 x, u8 y, u16 color, u16 value);
void HEXADECIMAL_DOUBLE		(u8 x, u8 y, u16 color, u16 value);
void HEXADECIMAL_TWOTILES	(u8 x, u8 y, u16 color, u16 value);
void HEXADECIMAL_INSTRUMENT	(u8 x, u8 y, u16 color, u16 value);
void DECIMAL				(u8 x, u8 y, u16 color, u16 value);
void DECIMAL_DOUBLE			(u8 x, u8 y, u16 color, u16 value);
void DECIMAL_TWOTILES		(u8 x, u8 y, u16 color, u16 value);
void DECIMAL_DOUBLE_TWOTILES(u8 x, u8 y, u16 color, u16 value);
	
void instrument1Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument2Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument3Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument4Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument5Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument6Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument7Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument8Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument3Val		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument5Val		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument6Val		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument27Val	(Control*, bool bigstep, bool add, u32 *pointer);
void instrumentType 	(Control*, bool bigstep, bool add, u32 *pointer);
void instrumentIndex	(Control*, bool bigstep, bool add, u32 *pointer);

void loadWavPreset0		(Control*, bool bigstep, bool add, u32 *pointer);
void loadWavPreset1		(Control*, bool bigstep, bool add, u32 *pointer);
void loadWavPreset2		(Control*, bool bigstep, bool add, u32 *pointer);
void loadWavPreset3		(Control*, bool bigstep, bool add, u32 *pointer);
void loadWavPreset4		(Control*, bool bigstep, bool add, u32 *pointer);
void loadWavPreset5		(Control*, bool bigstep, bool add, u32 *pointer);
void solo0				(Control*, bool bigstep, bool add, u32 *pointer);
void solo1				(Control*, bool bigstep, bool add, u32 *pointer);
void solo2				(Control*, bool bigstep, bool add, u32 *pointer);
void solo3				(Control*, bool bigstep, bool add, u32 *pointer);
void solo4				(Control*, bool bigstep, bool add, u32 *pointer);
void solo5				(Control*, bool bigstep, bool add, u32 *pointer);
void mute0				(Control*, bool bigstep, bool add, u32 *pointer);
void mute1				(Control*, bool bigstep, bool add, u32 *pointer);
void mute2				(Control*, bool bigstep, bool add, u32 *pointer);
void mute3				(Control*, bool bigstep, bool add, u32 *pointer);
void mute4				(Control*, bool bigstep, bool add, u32 *pointer);
void mute5				(Control*, bool bigstep, bool add, u32 *pointer);
void colorEditor		(Control*, bool bigstep, bool add, u32 *pointer);
void receiveSong		(Control*, bool bigstep, bool add, u32 *pointer);
void sendSong			(Control*, bool bigstep, bool add, u32 *pointer);
void mixer				(Control*, bool bigstep, bool add, u32 *pointer);
void slotUsage			(Control*, bool bigstep, bool add, u32 *pointer);
void purgeSongs			(Control*, bool bigstep, bool add, u32 *pointer);
void memoryTest			(Control*, bool bigstep, bool add, u32 *pointer);
void reinitialize		(Control*, bool bigstep, bool add, u32 *pointer);
void formatMemory		(Control*, bool bigstep, bool add, u32 *pointer);
void reset				(Control*, bool bigstep, bool add, u32 *pointer);
void loadConfig			(Control*, bool bigstep, bool add, u32 *pointer);
void saveConfig			(Control*, bool bigstep, bool add, u32 *pointer);
void defaultConfig		(Control*, bool bigstep, bool add, u32 *pointer);
void loadSlot			(Control*, bool bigstep, bool add, u32 *pointer);
void saveSlot			(Control*, bool bigstep, bool add, u32 *pointer);
void purgeSlot			(Control*, bool bigstep, bool add, u32 *pointer);
void copySlot			(Control*, bool bigstep, bool add, u32 *pointer);
void eraseSlot			(Control*, bool bigstep, bool add, u32 *pointer);
void tempoTap			(Control*, bool bigstep, bool add, u32 *pointer);
void bpmUpdate			(Control*, bool bigstep, bool add, u32 *pointer);
void ALPHA14			(Control*, bool bigstep, bool add, u32 *pointer);
void ALPHA6				(Control*, bool bigstep, bool add, u32 *pointer);
/* Variable Operator callbacks */
void modify1Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void modify2Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void modify3Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void modify4Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void modify5Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void modify6Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void modify7Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void modify8Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void modifyTempo		(Control*, bool bigstep, bool add, u32 *pointer);
void modifyChar			(Control*, bool bigstep, bool add, u32 *pointer); // Append char received to the string, if room left
void modifyNote			(Control*, bool bigstep, bool add, u32 *pointer);
void modify3Val			(Control*, bool bigstep, bool add, u32 *pointer); // For variables which can take up do 3 different values
void modify5Val			(Control*, bool bigstep, bool add, u32 *pointer); // For variables which can take up do 5 different values
void modify6Val			(Control*, bool bigstep, bool add, u32 *pointer); // For variables which can take up to 6 different values
void modify27Val		(Control*, bool bigstep, bool add, u32 *pointer); 
void modifyNote			(Control*, bool bigstep, bool add, u32 *pointer);
void modifyCommand		(Control*, bool bigstep, bool add, u32 *pointer);
void modifyInst			(Control*, bool bigstep, bool add, u32 *pointer);
void modifyVolume		(Control*, bool bigstep, bool add, u32 *pointer);
void modifyValue		(Control*, bool bigstep, bool add, u32 *pointer);
void modifyBPM			(Control*, bool bigstep, bool add, u32 *pointer);
void instrument1Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument2Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrumentType		(Control*, bool bigstep, bool add, u32 *pointer);
void instrumentIndex	(Control*, bool bigstep, bool add, u32 *pointer);
void instrument3Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument4Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument5Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument6Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument7Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument8Bit		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument3Val		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument6Val		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument5Val		(Control*, bool bigstep, bool add, u32 *pointer);
void instrument27Val	(Control*, bool bigstep, bool add, u32 *pointer);
void pasteCommand		(Control*, bool bigstep, bool add, u32 *pointer);
void pasteInst			(Control*, bool bigstep, bool add, u32 *pointer);
void pasteNote			(Control*, bool bigstep, bool add, u32 *pointer);
void pasteVolume		(Control*, bool bigstep, bool add, u32 *pointer);
void pasteValue			(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH0_alter	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH1_alter	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH2_alter	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH3_alter	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH4_alter	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH5_alter	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH0_paste	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH1_paste	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH2_paste	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH3_paste	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH4_paste	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH5_paste	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH0_focus	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH1_focus	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH2_focus	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH3_focus	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH4_focus	(Control*, bool bigstep, bool add, u32 *pointer);
void patternCH5_focus	(Control*, bool bigstep, bool add, u32 *pointer);
void paste2Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void paste3Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void paste4Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void paste5Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void paste6Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void paste7Bit			(Control*, bool bigstep, bool add, u32 *pointer);
void paste8Bit			(Control*, bool bigstep, bool add, u32 *pointer);

#endif
