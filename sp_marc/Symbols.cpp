/*
Copyright 1992-2009 Systems Planning (www.systemsplanning.com)

Licensed under the Apache License, Version 2.0 (the "License"); 
you may not use this file except in compliance with the License. 
You may obtain a copy of the License at 

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software 
distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
See the License for the specific language governing permissions and 
limitations under the License.
*/

/*
970107	symbols capable of being converted from ansel to latin-1
981014	seems to be a safe assumption that chars not initialized are 0
000211	ansel symbols not here are just dropped out of latin or plain conversion
*/

#if !defined (SYMBOLS_H)
#include "SYMBOLS.HPP"
#endif

#if !defined(SP_MARC_H)
#include "SP_MARC.HPP"	// for error numbers
#endif

#include "wcm.hpp"	// C++ types
#include <stdio.h>
#include <direct.h>


symbol_rt	Symbols [ZNUMSYMS];


charconvert_rt	MARC2UTF [zmarctoutf_size];
charconvert_rt	UTF2MARC [zutftomarc_size];
decomp_rt		DECOMP [zdecomp_size];


void InitializeSymbols ()
{


//*********** THE RULE: ALWAYS ADD SYMBOLS SO THAT THEY ARE IN ASCENDING
//				ORDER BY THEIR ANSEL VALUES ****************

int i = 0;

//polish L uppercase
Symbols [i].ansel [0] = 161;
Symbols [i].latin [0] = 'L';
Symbols [i].plain [0] = 'L';
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 129;
i++;

//slash O (scandinavian O)
Symbols [i].ansel [0] = 162;
Symbols [i].latin [0] = 216;
Symbols [i].plain [0] = 'O';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 152;
i++;

//crossbar D
Symbols [i].ansel [0] = 163;
Symbols [i].latin [0] = 208;
Symbols [i].plain [0] = 'D';
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 144;
i++;

//uppercase thorn
Symbols [i].ansel [0] = 164;
Symbols [i].latin [0] = 222;
Symbols [i].plain [0] = 'T';
Symbols [i].plain [1] = 'H';
Symbols [i].unimarc [0] = 236;
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 158;
i++;

//digraph AE
Symbols [i].ansel [0] = 165;
Symbols [i].latin [0] = 198;
Symbols [i].plain [0] = 'A';
Symbols [i].plain [1] = 'E';
Symbols [i].unimarc [0] = 225;
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 134;
i++;

//digraph OE
Symbols [i].ansel [0] = 166;
Symbols [i].latin [0] = 140;
Symbols [i].plain [0] = 'O';
Symbols [i].plain [1] = 'E';
Symbols [i].unimarc [0] = 234;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 146;
i++;

//miagkiy znak
Symbols [i].ansel [0] = 167;
Symbols [i].latin [0] = 180;
Symbols [i].plain [0] = 0;		
Symbols [i].utf8 [0] = 202;
Symbols [i].utf8 [1] = 185;
i++;

//middle dot
Symbols [i].ansel [0] = 168;
Symbols [i].latin [0] = 183;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 194;
Symbols [i].utf8 [1] = 183;
i++;

//musical flat
Symbols [i].ansel [0] = 169;
Symbols [i].latin [0] = 'f';
Symbols [i].latin [1] = 'l';
Symbols [i].latin [2] = 'a';
Symbols [i].latin [3] = 't';
Symbols [i].plain [0] = 0;
Symbols [i].unimarc [0] = 172;
Symbols [i].utf8 [0] = 226;
Symbols [i].utf8 [1] = 153;
Symbols [i].utf8 [2] = 173;
i++;

//registered patent
Symbols [i].ansel [0] = 170;
Symbols [i].latin [0] = 174;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 194;
Symbols [i].utf8 [1] = 174;
i++;

//plus/minus
Symbols [i].ansel [0] = 171;
Symbols [i].latin [0] = 177;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 194;
Symbols [i].utf8 [1] = 177;
i++;

//hook O
Symbols [i].ansel [0] = 172;
Symbols [i].latin [0] = 'O';
Symbols [i].plain [0] = 'O';
Symbols [i].unimarc [0] = 206;
Symbols [i].unimarc [1] = 'O';
Symbols [i].utf8 [0] = 198;
Symbols [i].utf8 [1] = 160;
i++;

//hook U
Symbols [i].ansel [0] = 173;
Symbols [i].latin [0] = 'U';
Symbols [i].plain [0] = 'U';
Symbols [i].unimarc [0] = 206;
Symbols [i].unimarc [1] = 'U';
Symbols [i].utf8 [0] = 198;
Symbols [i].utf8 [1] = 175;
i++;


//alif
// in March 2005 LC changed the mapping of the alif to 202 188. we keep the older
//		mapping as well for converting older UTF-8 records. NOTE THAT THE NEW
//		PREFERRED MAPPING COMES FIRST SO IT WILL BE THE FIRST ENCOUNTERED WHEN
//		GOING FROM MARC TO UTF
Symbols [i].ansel [0] = 174;
Symbols [i].latin [0] = 146;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 202;
Symbols [i].utf8 [1] = 188;
i++;
//alif (older mapping)
Symbols [i].ansel [0] = 174;
Symbols [i].latin [0] = 146;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 202;	// older mapping ...
Symbols [i].utf8 [1] = 190;	//... see note above
i++;


//alpha -- 981204 shown as Reserved on USMARC website, where did I get it?
/*070925 it's just not legal
Symbols [i].ansel [0] = 175;*/
Symbols [i].latin [0] = 'a';
Symbols [i].plain [0] = 'a';
Symbols [i].utf8 [0] = 201;	//we're using LATIN SMALL LETTER ALPHA 0251 here, ...
Symbols [i].utf8 [1] = 145;	//... not GREEK SMALL LETTER ALPHA 03B1 -- change if users have problems displaying
i++;

//ain or ayn
Symbols [i].ansel [0] = 176;
Symbols [i].latin [0] = 145;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 202;
Symbols [i].utf8 [1] = 187;
i++;

//polish l lowercase
Symbols [i].ansel [0] = 177;
Symbols [i].latin [0] = 'l';
Symbols [i].plain [0] = 'l';
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 130;
i++;

//slash o (scandinavian o)
Symbols [i].ansel [0] = 178;
Symbols [i].latin [0] = 248;
Symbols [i].plain [0] = 'o';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 184;
i++;

//crossbar d
Symbols [i].ansel [0] = 179;
Symbols [i].latin [0] = 'd';
Symbols [i].plain [0] = 'd';
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 145;
i++;

//lowercase thorn
Symbols [i].ansel [0] = 180;
Symbols [i].latin [0] = 254;
Symbols [i].plain [0] = 't';
Symbols [i].plain [1] = 'h';
Symbols [i].unimarc [0] = 252;
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 190;
i++;

//diagraph ae
Symbols [i].ansel [0] = 181;
Symbols [i].latin [0] = 230;
Symbols [i].plain [0] = 'a';
Symbols [i].plain [1] = 'e';
Symbols [i].unimarc [0] = 241;
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 166;
i++;

//digraph oe
Symbols [i].ansel [0] = 182;
Symbols [i].latin [0] = 156;
Symbols [i].plain [0] = 'o';
Symbols [i].plain [1] = 'e';
Symbols [i].unimarc [0] = 250;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 147;
i++;

//tverdyi znak
Symbols [i].ansel [0] = 183;
Symbols [i].latin [0] = 180;
Symbols [i].latin [1] = 180;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 202;
Symbols [i].utf8 [1] = 186;
i++;

//turkish i
Symbols [i].ansel [0] = 184;
Symbols [i].latin [0] = 'i';
Symbols [i].plain [0] = 'i';
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 177;
i++;

//british pound
Symbols [i].ansel [0] = 185;
Symbols [i].latin [0] = 163;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 194;
Symbols [i].utf8 [1] = 163;
i++;

//eth
Symbols [i].ansel [0] = 186;
Symbols [i].latin [0] = 240;
Symbols [i].plain [0] = 'd';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 176;
i++;

//dagger -- 981204 shown as Reserved on USMARC website
Symbols [i].ansel [0] = 187;
Symbols [i].latin [0] = 134;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 226;
Symbols [i].utf8 [1] = 128;
Symbols [i].utf8 [2] = 160;
i++;

//hook o
Symbols [i].ansel [0] = 188;
Symbols [i].latin [0] = 'o';
Symbols [i].plain [0] = 'o';
Symbols [i].unimarc [0] = 206;
Symbols [i].unimarc [1] = 'o';
Symbols [i].utf8 [0] = 198;
Symbols [i].utf8 [1] = 161;
i++;

//hook u
Symbols [i].ansel [0] = 189;
Symbols [i].latin [0] = 'u';
Symbols [i].plain [0] = 'u';
Symbols [i].unimarc [0] = 206;
Symbols [i].unimarc [1] = 'u';
Symbols [i].utf8 [0] = 198;
Symbols [i].utf8 [1] = 176;
i++;

//degrees
Symbols [i].ansel [0] = 192;
Symbols [i].latin [0] = 176;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 194;
Symbols [i].utf8 [1] = 176;
i++;

//script l
Symbols [i].ansel [0] = 193;
Symbols [i].latin [0] = 'l';
Symbols [i].plain [0] = 'l';
Symbols [i].utf8 [0] = 226;
Symbols [i].utf8 [1] = 132;
Symbols [i].utf8 [2] = 147;
i++;

//phonograph copyright
Symbols [i].ansel [0] = 194;
Symbols [i].latin [0] = 'p';
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 226;
Symbols [i].utf8 [1] = 132;
Symbols [i].utf8 [2] = 151;
i++;

//copyright
Symbols [i].ansel [0] = 195;
Symbols [i].latin [0] = 169;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 194;
Symbols [i].utf8 [1] = 169;
i++;

//musical sharp
Symbols [i].ansel [0] = 196;
Symbols [i].latin [0] = '#';
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 226;
Symbols [i].utf8 [1] = 153;
Symbols [i].utf8 [2] = 175;
i++;

//inverted question
Symbols [i].ansel [0] = 197;
Symbols [i].latin [0] = '¿';	//BF
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 194;
Symbols [i].utf8 [1] = 191;
i++;

//inverted exclamation
Symbols [i].ansel [0] = 198;
Symbols [i].latin [0] = '¡';	//A1
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 194;
Symbols [i].utf8 [1] = 161;
i++;

//pseudo question (hook)
Symbols [i].ansel [0] = 224;
Symbols [i].latin [0] = 0;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 137;
i++;

//grave
Symbols [i].ansel [0] = 225;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 128;
i++;

//grave A
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] =  65;
Symbols [i].latin [0] = 192;
Symbols [i].plain [0] =  65;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'A';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 128;
i++;

//grave E
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] =  69;
Symbols [i].latin [0] = 200;
Symbols [i].plain [0] =  69;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'E';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 136;
i++;

//grave I
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] =  73;
Symbols [i].latin [0] = 204;
Symbols [i].plain [0] =  73;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'I';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 140;
i++;

//grave O
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] =  79;
Symbols [i].latin [0] = 210;
Symbols [i].plain [0] =  79;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'O';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 146;
i++;

//grave U
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] =  85;
Symbols [i].latin [0] = 217;
Symbols [i].plain [0] =  85;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'U';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 153;
i++;
       
//grave a
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] =  97;
Symbols [i].latin [0] = 224;
Symbols [i].plain [0] =  97;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'a';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 160;
i++;

//grave e
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] = 101;
Symbols [i].latin [0] = 232;
Symbols [i].plain [0] = 101;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'e';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 168;
i++;

//grave i
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] = 105;
Symbols [i].latin [0] = 236;
Symbols [i].plain [0] = 105;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'i';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 172;
i++;

//grave o
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] = 111;
Symbols [i].latin [0] = 242;
Symbols [i].plain [0] = 111;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'o';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 178;
i++;

//grave u
Symbols [i].ansel [0] = 225;
Symbols [i].ansel [1] = 117;
Symbols [i].latin [0] = 249;
Symbols [i].plain [0] = 117;
Symbols [i].unimarc [0] = 193;
Symbols [i].unimarc [1] = 'u';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 185;
i++;

//acute
Symbols [i].ansel [0] = 226;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 129;
i++;

//acute A
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] =  65;
Symbols [i].latin [0] = 193;
Symbols [i].plain [0] =  65;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'A';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 129;
i++;

//acute E
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] =  69;
Symbols [i].latin [0] = 201;
Symbols [i].plain [0] =  69;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'E';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 137;
i++;

//acute I
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] =  73;
Symbols [i].latin [0] = 205;
Symbols [i].plain [0] =  73;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'I';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 141;
i++;

//acute O
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] =  79;
Symbols [i].latin [0] = 211;
Symbols [i].plain [0] =  79;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'O';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 147;
i++;

//acute U
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] =  85;
Symbols [i].latin [0] = 218;
Symbols [i].plain [0] =  85;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'U';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 154;
i++;

//acute Y
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] =  89;
Symbols [i].latin [0] = 221;
Symbols [i].plain [0] =  89;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'Y';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 157;
i++;

//acute a
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] =  97;
Symbols [i].latin [0] = 225;
Symbols [i].plain [0] =  97;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'a';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 161;
i++;

//acute e
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] = 101;
Symbols [i].latin [0] = 233;
Symbols [i].plain [0] = 101;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'e';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 169;
i++;

//acute i
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] = 105;
Symbols [i].latin [0] = 237;
Symbols [i].plain [0] = 105;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'i';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 173;
i++;

//acute n, added 071026
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] = 110;
Symbols [i].latin [0] = 110;
Symbols [i].plain [0] = 110;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 132;
i++;

//acute o
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] = 111;
Symbols [i].latin [0] = 243;
Symbols [i].plain [0] = 111;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'o';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 179;
i++;

//acute u
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] = 117;
Symbols [i].latin [0] = 250;
Symbols [i].plain [0] = 117;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'u';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 186;
i++;

//acute y
Symbols [i].ansel [0] = 226;
Symbols [i].ansel [1] = 121;
Symbols [i].latin [0] = 253;
Symbols [i].plain [0] = 121;
Symbols [i].unimarc [0] = 194;
Symbols [i].unimarc [1] = 'y';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 189;
i++;

//circumflex
Symbols [i].ansel [0] = 227;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 130;
i++;

//circumflex A
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] =  65;
Symbols [i].latin [0] = 194;
Symbols [i].plain [0] =  65;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'A';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 130;
i++;

//circumflex E
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] =  69;
Symbols [i].latin [0] = 202;
Symbols [i].plain [0] =  69;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'E';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 138;
i++;

//circumflex I
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] =  73;
Symbols [i].latin [0] = 206;
Symbols [i].plain [0] =  73;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'I';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 142;
i++;

//circumflex O
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] =  79;
Symbols [i].latin [0] = 212;
Symbols [i].plain [0] =  79;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'O';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 148;
i++;

//circumflex U
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] =  85;
Symbols [i].latin [0] = 219;
Symbols [i].plain [0] =  85;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'U';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 155;
i++;

//circumflex a
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] =  97;
Symbols [i].latin [0] = 226;
Symbols [i].plain [0] =  97;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'a';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 162;
i++;

//circumflex e
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] = 101;
Symbols [i].latin [0] = 234;
Symbols [i].plain [0] = 101;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'e';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 170;
i++;

//circumflex i
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] = 105;
Symbols [i].latin [0] = 238;
Symbols [i].plain [0] = 105;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'i';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 174;
i++;

//circumflex o
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] = 111;
Symbols [i].latin [0] = 244;
Symbols [i].plain [0] = 111;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'o';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 180;
i++;

//circumflex u
Symbols [i].ansel [0] = 227;
Symbols [i].ansel [1] = 117;
Symbols [i].latin [0] = 251;
Symbols [i].plain [0] = 117;
Symbols [i].unimarc [0] = 195;
Symbols [i].unimarc [1] = 'u';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 187;
i++;

//tilde
Symbols [i].ansel [0] = 228;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 131;
i++;

//tilde A
Symbols [i].ansel [0] = 228;
Symbols [i].ansel [1] =  65;
Symbols [i].latin [0] = 195;
Symbols [i].plain [0] =  65;
Symbols [i].unimarc [0] = 196;
Symbols [i].unimarc [1] = 'A';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 131;
i++;

//tilde N
Symbols [i].ansel [0] = 228;
Symbols [i].ansel [1] =  78;
Symbols [i].latin [0] = 209;
Symbols [i].plain [0] =  78;
Symbols [i].unimarc [0] = 196;
Symbols [i].unimarc [1] = 'N';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 145;
i++;

//tilde O
Symbols [i].ansel [0] = 228;
Symbols [i].ansel [1] =  79;
Symbols [i].latin [0] = 213;
Symbols [i].plain [0] =  79;
Symbols [i].unimarc [0] = 196;
Symbols [i].unimarc [1] = 'O';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 149;
i++;

//tilde a
Symbols [i].ansel [0] = 228;
Symbols [i].ansel [1] =  97;
Symbols [i].latin [0] = 227;
Symbols [i].plain [0] =  97;
Symbols [i].unimarc [0] = 196;
Symbols [i].unimarc [1] = 'a';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 163;
i++;

//tilde n
Symbols [i].ansel [0] = 228;
Symbols [i].ansel [1] = 110;
Symbols [i].latin [0] = 241;
Symbols [i].plain [0] = 110;
Symbols [i].unimarc [0] = 196;
Symbols [i].unimarc [1] = 'n';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 177;
i++;

//tilde o
Symbols [i].ansel [0] = 228;
Symbols [i].ansel [1] = 111;
Symbols [i].latin [0] = 245;
Symbols [i].plain [0] = 111;
Symbols [i].unimarc [0] = 196;
Symbols [i].unimarc [1] = 'o';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 181;
i++;

// combining macron
Symbols [i].ansel [0] = 229;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 132;
i++;

//macron A
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 65;
Symbols [i].latin [0] = 65;
Symbols [i].plain [0] = 65;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 128;
i++;

//macron E
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 69;
Symbols [i].latin [0] = 69;
Symbols [i].plain [0] = 69;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 146;
i++;

//macron I
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 73;
Symbols [i].latin [0] = 73;
Symbols [i].plain [0] = 73;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 170;
i++;

//macron O
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 79;
Symbols [i].latin [0] = 79;
Symbols [i].plain [0] = 79;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 140;
i++;

//macron U
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 85;
Symbols [i].latin [0] = 85;
Symbols [i].plain [0] = 85;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 170;
i++;

//macron a
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 97;
Symbols [i].latin [0] = 97;
Symbols [i].plain [0] = 97;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 129;
i++;

//macron e
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 101;
Symbols [i].latin [0] = 101;
Symbols [i].plain [0] = 101;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 147;
i++;

//macron i
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 105;
Symbols [i].latin [0] = 105;
Symbols [i].plain [0] = 105;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 171;
i++;

//macron o
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 111;
Symbols [i].latin [0] = 111;
Symbols [i].plain [0] = 111;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 141;
i++;

//macron u
Symbols [i].ansel [0] = 229;
Symbols [i].ansel [1] = 117;
Symbols [i].latin [0] = 117;
Symbols [i].plain [0] = 117;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 171;
i++;

//breve
Symbols [i].ansel [0] = 230;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 134;
i++;

//breve A
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 65;
Symbols [i].latin [0] = 65;
Symbols [i].plain [0] = 65;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 130;
i++;

//breve E
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 69;
Symbols [i].latin [0] = 69;
Symbols [i].plain [0] = 69;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 148;
i++;

//breve G
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 71;
Symbols [i].latin [0] = 71;
Symbols [i].plain [0] = 71;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 158;
i++;

//breve I
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 73;
Symbols [i].latin [0] = 73;
Symbols [i].plain [0] = 73;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 172;
i++;

//breve O
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 79;
Symbols [i].latin [0] = 79;
Symbols [i].plain [0] = 79;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 142;
i++;

//breve U
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 85;
Symbols [i].latin [0] = 85;
Symbols [i].plain [0] = 85;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 172;
i++;

//breve a
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 97;
Symbols [i].latin [0] = 97;
Symbols [i].plain [0] = 97;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 131;
i++;

//breve e
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 101;
Symbols [i].latin [0] = 101;
Symbols [i].plain [0] = 101;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 149;
i++;

//breve g
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 103;
Symbols [i].latin [0] = 103;
Symbols [i].plain [0] = 103;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 159;
i++;

//breve i
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 105;
Symbols [i].latin [0] = 105;
Symbols [i].plain [0] = 105;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 173;
i++;

//breve o
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 111;
Symbols [i].latin [0] = 111;
Symbols [i].plain [0] = 111;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 143;
i++;

//breve u
Symbols [i].ansel [0] = 230;
Symbols [i].ansel [1] = 117;
Symbols [i].latin [0] = 117;
Symbols [i].plain [0] = 117;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 173;
i++;

//dot above
Symbols [i].ansel [0] = 231;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 135;
i++;

//dot above C
Symbols [i].ansel [0] = 231;
Symbols [i].ansel [1] = 67;
Symbols [i].latin [0] = 67;
Symbols [i].plain [0] = 67;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 138;
i++;

//dot above E
Symbols [i].ansel [0] = 231;
Symbols [i].ansel [1] = 69;
Symbols [i].latin [0] = 69;
Symbols [i].plain [0] = 69;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 150;
i++;

//dot above G
Symbols [i].ansel [0] = 231;
Symbols [i].ansel [1] = 71;
Symbols [i].latin [0] = 71;
Symbols [i].plain [0] = 71;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 160;
i++;

//dot above I
Symbols [i].ansel [0] = 231;
Symbols [i].ansel [1] = 73;
Symbols [i].latin [0] = 73;
Symbols [i].plain [0] = 73;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 176;
i++;

//dot above Z
Symbols [i].ansel [0] = 231;
Symbols [i].ansel [1] = 90;
Symbols [i].latin [0] = 90;
Symbols [i].plain [0] = 90;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 187;
i++;

//dot above c
Symbols [i].ansel [0] = 231;
Symbols [i].ansel [1] = 99;
Symbols [i].latin [0] = 99;
Symbols [i].plain [0] = 99;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 139;
i++;

//dot above e
Symbols [i].ansel [0] = 231;
Symbols [i].ansel [1] = 101;
Symbols [i].latin [0] = 101;
Symbols [i].plain [0] = 101;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 151;
i++;

//dot above g
Symbols [i].ansel [0] = 231;
Symbols [i].ansel [1] = 103;
Symbols [i].latin [0] = 103;
Symbols [i].plain [0] = 103;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 161;
i++;

//dot above z
Symbols [i].ansel [0] = 231;
Symbols [i].ansel [1] = 122;
Symbols [i].latin [0] = 122;
Symbols [i].plain [0] = 122;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 188;
i++;

// UNIMARC has both a diaresis (200) and an umlaut (201) -- handled in ConvertUnimarc

//umlaut
Symbols [i].ansel [0] = 232;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 136;
i++;

//umlaut A
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] =  65;
Symbols [i].latin [0] = 196;
Symbols [i].plain [0] =  65;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'A';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 132;
i++;

//umlaut E
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] =  69;
Symbols [i].latin [0] = 203;
Symbols [i].plain [0] =  69;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'E';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 139;
i++;

//umlaut I
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] =  73;
Symbols [i].latin [0] = 207;
Symbols [i].plain [0] =  73;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'I';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 143;
i++;

//umlaut O
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] =  79;
Symbols [i].latin [0] = 214;
Symbols [i].plain [0] =  79;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'O';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 150;
i++;

//umlaut U
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] =  85;
Symbols [i].latin [0] = 220;
Symbols [i].plain [0] =  85;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'U';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 156;
i++;

//umlaut Y
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] =  89;
Symbols [i].latin [0] =  89;
Symbols [i].plain [0] =  89;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'U';
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 184;
i++;

//umlaut a
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] =  97;
Symbols [i].latin [0] = 228;
Symbols [i].plain [0] =  97;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'a';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 164;
i++;

//umlaut e
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] = 101;
Symbols [i].latin [0] = 235;
Symbols [i].plain [0] = 101;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'e';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 171;
i++;

//umlaut i
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] = 105;
Symbols [i].latin [0] = 239;
Symbols [i].plain [0] = 105;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'i';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 175;
i++;

//umlaut o
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] = 111;
Symbols [i].latin [0] = 246;
Symbols [i].plain [0] = 111;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'o';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 182;
i++;

//umlaut u
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] = 117;
Symbols [i].latin [0] = 252;
Symbols [i].plain [0] = 117;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'u';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 188;
i++;

//umlaut y
Symbols [i].ansel [0] = 232;
Symbols [i].ansel [1] = 121;
Symbols [i].latin [0] = 255;
Symbols [i].plain [0] = 121;
Symbols [i].unimarc [0] = 201;
Symbols [i].unimarc [1] = 'y';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 191;
i++;

//caron (hacek)
Symbols [i].ansel [0] = 233;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 140;
i++;

//caron (hacek) C
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 67;
Symbols [i].latin [0] = 67;
Symbols [i].plain [0] = 67;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 140;
i++;

//caron (hacek) D
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 68;
Symbols [i].latin [0] = 68;
Symbols [i].plain [0] = 68;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 142;
i++;

//caron (hacek) E
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 69;
Symbols [i].latin [0] = 69;
Symbols [i].plain [0] = 69;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 154;
i++;

//caron (hacek) L
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 76;
Symbols [i].latin [0] = 76;
Symbols [i].plain [0] = 76;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 189;
i++;

//caron (hacek) N
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 78;
Symbols [i].latin [0] = 78;
Symbols [i].plain [0] = 78;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 135;
i++;

//caron (hacek) R
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 82;
Symbols [i].latin [0] = 82;
Symbols [i].plain [0] = 82;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 152;
i++;

//caron (hacek) S
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 83;
Symbols [i].latin [0] = 83;
Symbols [i].plain [0] = 83;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 160;
i++;

//caron (hacek) T
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 84;
Symbols [i].latin [0] = 84;
Symbols [i].plain [0] = 84;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 164;
i++;

//caron (hacek) Z
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 90;
Symbols [i].latin [0] = 90;
Symbols [i].plain [0] = 90;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 189;
i++;

//caron (hacek) c
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 99;
Symbols [i].latin [0] = 99;
Symbols [i].plain [0] = 99;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 141;
i++;

//caron (hacek) d
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 100;
Symbols [i].latin [0] = 100;
Symbols [i].plain [0] = 100;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 143;
i++;

//caron (hacek) e
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 101;
Symbols [i].latin [0] = 101;
Symbols [i].plain [0] = 101;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 155;
i++;

//caron (hacek) l
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 108;
Symbols [i].latin [0] = 108;
Symbols [i].plain [0] = 108;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 190;
i++;

//caron (hacek) n
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 110;
Symbols [i].latin [0] = 110;
Symbols [i].plain [0] = 110;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 136;
i++;

//caron (hacek) r
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 114;
Symbols [i].latin [0] = 114;
Symbols [i].plain [0] = 114;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 153;
i++;

//caron (hacek) s
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 115;
Symbols [i].latin [0] = 115;
Symbols [i].plain [0] = 115;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 161;
i++;

//caron (hacek) t
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 116;
Symbols [i].latin [0] = 116;
Symbols [i].plain [0] = 116;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 165;
i++;

//caron (hacek) z
Symbols [i].ansel [0] = 233;
Symbols [i].ansel [1] = 122;
Symbols [i].latin [0] = 122;
Symbols [i].plain [0] = 122;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 190;
i++;

//circle above
Symbols [i].ansel [0] = 234;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 138;
i++;

//circle above A
Symbols [i].ansel [0] = 234;
Symbols [i].ansel [1] =  65;
Symbols [i].latin [0] = 197;
Symbols [i].plain [0] =  65;
Symbols [i].unimarc [0] = 202;
Symbols [i].unimarc [1] = 'A';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 133;
i++;

//circle above U
Symbols [i].ansel [0] = 234;
Symbols [i].ansel [1] =  85;
Symbols [i].latin [0] =  85;
Symbols [i].plain [0] =  85;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 174;
i++;

//circle above a
Symbols [i].ansel [0] = 234;
Symbols [i].ansel [1] =  97;
Symbols [i].latin [0] = 229;
Symbols [i].plain [0] =  97;
Symbols [i].unimarc [0] = 202;
Symbols [i].unimarc [1] = 'a';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 165;
i++;

//circle above u
Symbols [i].ansel [0] = 234;
Symbols [i].ansel [1] = 117;
Symbols [i].latin [0] = 117;
Symbols [i].plain [0] = 117;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 175;
i++;

// double ligature 1st half
Symbols [i].ansel [0] = 235;
Symbols [i].utf8 [0] = 239;
Symbols [i].utf8 [1] = 184;
Symbols [i].utf8 [2] = 160;
i++;

// double ligature 2d half
Symbols [i].ansel [0] = 236;
Symbols [i].utf8 [0] = 239;
Symbols [i].utf8 [1] = 184;
Symbols [i].utf8 [2] = 161;
i++;

//high comma off center
Symbols [i].ansel [0] = 237;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 149;
i++;

//double acute
Symbols [i].ansel [0] = 238;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 139;
i++;

//double acute O
Symbols [i].ansel [0] = 238;
Symbols [i].ansel [1] = 79;
Symbols [i].latin [0] = 79;
Symbols [i].plain [0] = 79;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 144;
i++;

//double acute U
Symbols [i].ansel [0] = 238;
Symbols [i].ansel [1] = 85;
Symbols [i].latin [0] = 85;
Symbols [i].plain [0] = 85;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 176;
i++;

//double acute o
Symbols [i].ansel [0] = 238;
Symbols [i].ansel [1] = 111;
Symbols [i].latin [0] = 111;
Symbols [i].plain [0] = 111;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 145;
i++;

//double acute u
Symbols [i].ansel [0] = 238;
Symbols [i].ansel [1] = 117;
Symbols [i].latin [0] = 117;
Symbols [i].plain [0] = 117;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 177;
i++;

//candrabindu
Symbols [i].ansel [0] = 239;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 144;
i++;

//cedilla
Symbols [i].ansel [0] = 240;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 167;
i++;

//cedilla C
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] =  67;
Symbols [i].latin [0] = 199;
Symbols [i].plain [0] =  67;
Symbols [i].unimarc [0] = 208;
Symbols [i].unimarc [1] = 'C';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 135;
i++;

//cedilla G
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 71;
Symbols [i].latin [0] = 71;
Symbols [i].plain [0] = 71;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 162;
i++;

//cedilla K
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 75;
Symbols [i].latin [0] = 75;
Symbols [i].plain [0] = 75;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 182;
i++;

//cedilla L
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 76;
Symbols [i].latin [0] = 76;
Symbols [i].plain [0] = 76;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 187;
i++;

//cedilla N
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 78;
Symbols [i].latin [0] = 78;
Symbols [i].plain [0] = 78;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 133;
i++;

//cedilla R
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 82;
Symbols [i].latin [0] = 82;
Symbols [i].plain [0] = 82;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 150;
i++;

//cedilla S
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 83;
Symbols [i].latin [0] = 83;
Symbols [i].plain [0] = 83;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 158;
i++;

//cedilla T
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 84;
Symbols [i].latin [0] = 84;
Symbols [i].plain [0] = 84;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 162;
i++;


//cedilla c
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] =  99;
Symbols [i].latin [0] = 231;
Symbols [i].plain [0] =  99;
Symbols [i].unimarc [0] = 208;
Symbols [i].unimarc [1] = 'c';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 167;
i++;

//cedilla g
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 103;
Symbols [i].latin [0] = 103;
Symbols [i].plain [0] = 103;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 163;
i++;

//cedilla k
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 107;
Symbols [i].latin [0] = 107;
Symbols [i].plain [0] = 107;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 183;
i++;

//cedilla l
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 108;
Symbols [i].latin [0] = 108;
Symbols [i].plain [0] = 108;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 188;
i++;

//cedilla n
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 110;
Symbols [i].latin [0] = 110;
Symbols [i].plain [0] = 110;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 134;
i++;

//cedilla r
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 114;
Symbols [i].latin [0] = 114;
Symbols [i].plain [0] = 114;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 151;
i++;

//cedilla s
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 115;
Symbols [i].latin [0] = 115;
Symbols [i].plain [0] = 115;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 159;
i++;

//cedilla t
Symbols [i].ansel [0] = 240;
Symbols [i].ansel [1] = 116;
Symbols [i].latin [0] = 116;
Symbols [i].plain [0] = 116;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 163;
i++;

//right hook (ogonek)
Symbols [i].ansel [0] = 241;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 168;
i++;

//right hook (ogonek) A
Symbols [i].ansel [0] = 241;
Symbols [i].ansel [1] = 65;
Symbols [i].latin [0] = 65;
Symbols [i].plain [0] = 65;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 132;
i++;

//right hook (ogonek) E
Symbols [i].ansel [0] = 241;
Symbols [i].ansel [1] = 69;
Symbols [i].latin [0] = 69;
Symbols [i].plain [0] = 69;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 152;
i++;

//right hook (ogonek) I
Symbols [i].ansel [0] = 241;
Symbols [i].ansel [1] = 73;
Symbols [i].latin [0] = 73;
Symbols [i].plain [0] = 73;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 174;
i++;

//right hook (ogonek) U
Symbols [i].ansel [0] = 241;
Symbols [i].ansel [1] = 85;
Symbols [i].latin [0] = 85;
Symbols [i].plain [0] = 85;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 178;
i++;

//right hook (ogonek) a
Symbols [i].ansel [0] = 241;
Symbols [i].ansel [1] = 97;
Symbols [i].latin [0] = 97;
Symbols [i].plain [0] = 97;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 133;
i++;

//right hook (ogonek) e
Symbols [i].ansel [0] = 241;
Symbols [i].ansel [1] = 101;
Symbols [i].latin [0] = 101;
Symbols [i].plain [0] = 101;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 153;
i++;

//right hook (ogonek) i
Symbols [i].ansel [0] = 241;
Symbols [i].ansel [1] = 105;
Symbols [i].latin [0] = 105;
Symbols [i].plain [0] = 105;
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 175;
i++;

//right hook (ogonek) u
Symbols [i].ansel [0] = 241;
Symbols [i].ansel [1] = 117;
Symbols [i].latin [0] = 117;
Symbols [i].plain [0] = 117;
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 179;
i++;

//dot below
Symbols [i].ansel [0] = 242;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 163;
i++;

//double dot below
Symbols [i].ansel [0] = 243;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 164;
i++;

//circle below
Symbols [i].ansel [0] = 244;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 165;
i++;

//981016 double underscore -- the ANSEL symbol to use for a non-ANSEL diacritic
// -- for this reason, no entries in this table for 245 plus letters -- that is,
//		we want the 245 to be ignored when converting to latin or plain
//050130 i think double underscore can be a modifier, but even so there would be no latin or plain equivalents
Symbols [i].ansel [0] = 245;
//050130 this shouldn't have been here, since we were not supposed to convert it//Symbols [i].latin [0] = '_';
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 179;
i++;

// combining underscore/low-line (non-spacing)
Symbols [i].ansel [0] = 246;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 178;
i++;

//left hook (comma below)
Symbols [i].ansel [0] = 247;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 166;
i++;

//right cedilla (combining left half ring below)
Symbols [i].ansel [0] = 248;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 156;
i++;

//upadhmaniya (breve below)
Symbols [i].ansel [0] = 249;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 174;
i++;

//double tilde 1st half
Symbols [i].ansel [0] = 250;
Symbols [i].utf8 [0] = 239;
Symbols [i].utf8 [1] = 184;
Symbols [i].utf8 [2] = 162;
i++;

//double tilde 2d half
Symbols [i].ansel [0] = 251;
Symbols [i].utf8 [0] = 239;
Symbols [i].utf8 [1] = 184;
Symbols [i].utf8 [2] = 163;
i++;

//high comma centered (psili)
Symbols [i].ansel [0] = 254;
Symbols [i].utf8 [0] = 204;
Symbols [i].utf8 [1] = 147;
i++;

//German ss (eszett)
Symbols [i].ansel [0] = 199;
Symbols [i].latin [0] = 223;
Symbols [i].plain [0] = 's';
Symbols [i].plain [1] = 's';
Symbols [i].utf8 [0] = 195;
Symbols [i].utf8 [1] = 159;
Symbols [i].unimarc [0] = 251;
i++;

//euro
Symbols [i].ansel [0] = 200;
Symbols [i].latin [0] = 128;
Symbols [i].plain [0] = 0;
Symbols [i].utf8 [0] = 226;
Symbols [i].utf8 [1] = 130;
Symbols [i].utf8 [1] = 172;
i++;

//digraph IJ (unimarc)
Symbols [i].latin [0] = 'I';
Symbols [i].latin [1] = 'J';
Symbols [i].plain [0] = 'I';
Symbols [i].plain [1] = 'J';
Symbols [i].unimarc [0] = 230;
i++;

//digraph ij (unimarc)
Symbols [i].latin [0] = 'i';
Symbols [i].latin [1] = 'j';
Symbols [i].plain [0] = 'i';
Symbols [i].plain [1] = 'j';
Symbols [i].unimarc [0] = 246;
i++;

//uppercase stroke h
Symbols [i].latin [0] = 'H';
Symbols [i].plain [0] = 'H';
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 166;
i++;

//lowercase stroke h
Symbols [i].latin [0] = 'h';
Symbols [i].plain [0] = 'h';
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 167;
i++;

//modifier letter left half-ring
Symbols [i].latin [0] = 96;
Symbols [i].utf8 [0] = 202;
Symbols [i].utf8 [1] = 191;
i++;

//capital S with acute
Symbols [i].latin [0] = 'S';
Symbols [i].plain [0] = 'S';
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 154;
i++;

//small k with line below
Symbols [i].latin [0] = 'k';
Symbols [i].plain [0] = 'k';
Symbols [i].utf8 [0] = 225;
Symbols [i].utf8 [1] = 184;
Symbols [i].utf8 [2] = 181;
i++;

//small h with breve below
Symbols [i].latin [0] = 'h';
Symbols [i].plain [0] = 'h';
Symbols [i].utf8 [0] = 225;
Symbols [i].utf8 [1] = 184;
Symbols [i].utf8 [2] = 171;
i++;

//small n with dot below
Symbols [i].latin [0] = 'n';
Symbols [i].plain [0] = 'n';
Symbols [i].utf8 [0] = 225;
Symbols [i].utf8 [1] = 185;
Symbols [i].utf8 [2] = 135;
i++;

//small t with dot below
Symbols [i].latin [0] = 't';
Symbols [i].plain [0] = 't';
Symbols [i].utf8 [0] = 225;
Symbols [i].utf8 [1] = 185;
Symbols [i].utf8 [2] = 173;
i++;

//small l (el) with dot below
Symbols [i].latin [0] = 'l';
Symbols [i].plain [0] = 'l';
Symbols [i].utf8 [0] = 225;
Symbols [i].utf8 [1] = 184;
Symbols [i].utf8 [2] = 183;
i++;

//small n with dot above
Symbols [i].latin [0] = 'n';
Symbols [i].plain [0] = 'n';
Symbols [i].utf8 [0] = 225;
Symbols [i].utf8 [1] = 185;
Symbols [i].utf8 [2] = 133;
i++;

//small n with line below
Symbols [i].latin [0] = 'n';
Symbols [i].plain [0] = 'n';
Symbols [i].utf8 [0] = 225;
Symbols [i].utf8 [1] = 185;
Symbols [i].utf8 [2] = 136;
i++;

//small c with acute
Symbols [i].latin [0] = 'c';
Symbols [i].plain [0] = 'c';
Symbols [i].utf8 [0] = 196;
Symbols [i].utf8 [1] = 135;
i++;

//small s with acute
Symbols [i].latin [0] = 's';
Symbols [i].plain [0] = 's';
Symbols [i].utf8 [0] = 197;
Symbols [i].utf8 [1] = 155;
i++;
}


int InitializeCodetables ()
{
	int Result = E_PERFECTSUCCESS;
				

	//060115 don't load if already loaded
	//	-- look at an arbitrary one, but not near the beginning because 
	//			it may be null on purpose
	if (UTF2MARC[1000].utf8[0])
		return Result;


	int i;
	long kk;
	char* tablenames [] = {"MARC2UTF", "UTF2MARC"};
	long table_len;
	
	// pointers to first element in each table
	charconvert_rt* tables [] = {MARC2UTF, UTF2MARC};
	
	FILE* FH;
	char t_line [80];
	WString tablename, t_string;
	WStringArray tlinearray;
	charconvert_rt* thistable;
	
	// loop to load multiple tables
	for (i = 0; i < (sizeof (tables) / sizeof (thistable)); i++)
	{
		tablename = tablenames [i];
	
		if (i == 0)
			table_len = sizeof (MARC2UTF);
		else if (i == 1)
			table_len = sizeof (UTF2MARC);

		tablename.Concat (".txt");
		
		thistable = tables [i];		
		memset (thistable, 0, table_len);

		/* find default dir -- debugging
		char *cwd;
		cwd = getcwd( NULL, 0 );
		*/
		
		if ((FH = fopen (tablename, "r")) == NULL)
		{
			return E_FILENOTOPEN;
		}
		else
		{
			kk = 0;
			
			// loop to load a single table
			while (!feof (FH))
			{
				fgets (t_line, sizeof (t_line), FH);
				
				if (!feof(FH))
				{
					t_string = t_line;
					tlinearray = t_string.Parse (",\r\n");
					
					if (tlinearray.GetCount () < 4)
						return E_CONVERSION;
	
					strcpy ((thistable + kk)->marc, tlinearray [0].GetText ());
					strcpy ((thistable + kk)->utf8, tlinearray [1].GetText ());
					if (tlinearray[2].GetCharacter(0) == '0')
						strcpy ((thistable + kk)->combining, "0");
					else
						strcpy ((thistable + kk)->combining, "1");
					strcpy ((thistable + kk)->charset, tlinearray [3].GetText ());
										
					kk++;
				}
			}
		}
		
		fclose (FH);
	}
	
	
	//051231 LOAD DECOMPOSITION TABLE
	
	tablename = "DECOMP.TXT";

	table_len = sizeof (DECOMP);
	memset (DECOMP, 0, table_len);
	
	if ((FH = fopen (tablename, "r")) == NULL)
	{
		return E_FILENOTOPEN;
	}
	else
	{
		kk = 0;
		while (!feof (FH))
		{
			fgets (t_line, sizeof (t_line), FH);
			
			if (!feof(FH))
			{
				t_string = t_line;
				tlinearray = t_string.Parse (",\r\n");
				
				if (tlinearray.GetCount () < 2)
					return E_CONVERSION;
	
				strcpy ((DECOMP + kk)->utf8, tlinearray [0].GetText ());
				strcpy ((DECOMP + kk)->decomp, tlinearray [1].GetText ());
									
				kk++;
			}
		}
	}
	
	fclose (FH);
	
	return Result;
}
