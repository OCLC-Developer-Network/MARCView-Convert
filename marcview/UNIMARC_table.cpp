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

// "not needed for FFF" means it refers to a format other than printed monographs. 
//	-- these have not been tested
// all lines for a single UNI tag should be together -- indicator-insert logic in MakeTree depends on this

// this table is in UNIMARC order so the output is written in that order

/*************** M21 tags not in FFF are commented out. this is so we can know when they appear
		in a source file and can test the results ************************/


/**************** MOST OF THE "NON-TESTED" ITEMS HAVE NOT EVEN BEEN ANALYZED OR MAPPED ************
*****************	                  they are the same as THDL                          ************
**************************************************************************************************/

static yTrans::yUNIMARC UNI_table[] =
{
//   MARC21  UNIMARC
	{"LDR", "LDR"},
	{"001", "001"},
	{"003", "000"},
	{"005", "005"},
	
	//{"011", "000"},  	// Not translated (OCLC) -- not in M21 format
	//{"012", "000"},  	// Not translated (OCLC) -- not in M21 format
	
	//{"019", "301"},	// (OCLC)
	{"019", "000"},	// no m21 019 in format
	
	{"020a", "010a"},	// must be split into 010a and b
	{"020c", "010d"},
	{"020z", "010z"},
	
	// not tested
	//{"022a", "011a"},
	//{"022y", "011z"},
	//{"022z", "011z"},
	
	//{"018a", "014a"},	// not tested -- applied only to j. articles
	
	/* 024 seems much more generic than 014, not sure it would be wise to convert
	{"024", "014"},*/
	
	{"010a", "020b", "$aUS+++"},
	//{"010b", "020a"},	// not tested, may need special processing
	//{"010z", "020z"},	// not tested
	
	//{"0152", "020a"},	// not tested -- uses natbibs.txt for code translation
	{"015a", "020b"},	
	// m21_015 doesn't have the concept of an invalid number, so uni_020$z will never be filled
	
	/* don't believe this should be translated -- it's a system number like the 001 
		-- for FFF, it's a duplicate of their 001 */
	{"016", "000"},
	
	//{"017b", "021a"},	// not tested -- requires code translation
	//{"017a", "021b"},	// not tested -- m21_017 doesn't have the concept of an invalid number,
					//		so uni_021$z will never be filled
	//{"017d", "000"},	// no equiv
	//{"017i", "000"},	// no equiv
	
	//{"074", "022"},		// not tested
	
	//{"030a", "040a"},	// not tested -- not needed for FFF
	//{"030z", "040z"},	// not tested -- not needed for FFF
	
	//{"028", "071"},	// not tested -- not needed for FFF (its for sound recordings, printed music, and videorecordings)
		
	//{"008", "100, 101, 102, 105, 106, 110, 115, 120, 125, 126, 128, and 135"},
	//	-- these are placeholders -- see notes in ConvertToUNIMARC
	{"ff1", "100a"},
	{"ff2", "101a"},

	{"041a", "101a"},
	//{"041h", "101b"},	// not tested -- no way to distinguish whether 041h goes to 101b or c
	{"041h", "101c"},
	//{"041b", "101d"},
	//{"041f", "101e"},
	//{"041b", "101j"},	// not tested
	//{"041e", "101h"},
	//{"041g", "101i"},	// not tested

	{"ff3", "102a"},
	{"ff4", "105a"},
	{"ff5", "106a"},
	
	{"042", "000"},	// authentication center, not needed for FFF
	
	//{"044a", "102a"},	// not tested -- codes need conversion
	
	//{"305", "000"},  	// Not translated (OCLC) -- not in M21 format
	//{"034", "123"},		// not tested
	
	{"035", "000"},  	// Not translated (OCLC)
	
	{"039", "000"},  	// in lacma file but not in m21 format
	
	//{"306a", "127a"},	// not needed for FFF
	
	//{"047", "128"},		// not tested -- not needed for FFF
	//{"048", "128"},		// not tested -- not needed for FFF
	{"049", "000"},  	// Not translated (OCLC)
	
	{"099", "000"},  	// local field
	
	// the letter after the _ is the byte/00 value
	// those commented out we are not yet handling for FFF
	//{"007_a", "000"},  	// Not translated (OCLC)
	{"007_c", "135a"},  		// electronic resource but not computer file -- UNI puts both this and 006 into 135
	//{"007_d", "000"},  	// Not translated (OCLC)
	//{"007_g", "115"},
	//{"007_h", "130"},
	{"007_k", "116a"},		// nonprojected graphic
	//{"007_m", "115"},
	{"007_s", "126a"},		// sound recording
	{"007_t", "106a"},		// text
	{"007_v", "115a"},		// videorecording
	
	//060419 adding for computer files only
	//{"006", "135a"},		// 006 used only to add to the 135 that the 007 creates, no need for a node
	
	{"245a", "200a"},
	{"245h", "200b"},
	{"245b", "200e"},	// except that parallel titles in 245b go to 200d (in MtM code)
	{"245c", "200f"},
	{"245n", "200h"},
	{"245p", "200i"},	
	
	//{"245h", "204a"},	// not tested -- duplicated above

	{"250a", "205a"},
	{"250b", "205f"},
	
	//{"255", "206"},		// not tested
	
	{"265", "000"},	// found in lacma file but not in m21 format
	
	//{"350", "000"},  	// Not translated (OCLC)
	//{"359", "000"},  	// Not translated (OCLC)
	//{"362", "207"},		// not tested
	
	//{"254", "208"},		// not tested
	
	//{"260", "071 and 210"), // (OCLC) -- 071 not needed for FFF
	{"260a", "210a"},		// if address follows in parens, it should be inserted into 210b
	{"260b", "210c"},
	{"260c", "210d"},
	{"260e", "210e"},		// if address follows in parens, it should be inserted into 210f
	{"260f", "210g"},
	{"260g", "210h"},
	
	//{"263a", "211a"},	// not tested
	
	{"300a", "215a"},
	{"300b", "215c"},
	{"300c", "215d"},
	{"300e", "215e"},
	{"300f", "215c"},
	{"300g", "215c"},
	
	/* LC map shows 
			uni_410 -> m21_760 
			uni_411 -> m21_762
			uni_225 -> m21_490
			"the conversion ... will never produce a MARC21 440"
			m21 400-411 not in format so they must have been made obsolete 
	{"440", "225 and 410"},	// (OCLC) the same data is copied to both fields
	{"490", "225"},			// (OCLC) */
	
	{"440a", "225a"},
	{"440n", "225h"},
	{"440p", "225h"},
	{"440v", "225v"},
	{"440x", "225x"},
	
	{"490a", "225a"},
	/* 	uni $d - i all go into m21 $a, so can't map. believe too complex to parse
			based on punctuation
	{"490", "225d"},
	{"490", "225e"},
	{"490", "225f"},
	{"490", "225h"},
	{"490", "225i"},	*/
	{"490v", "225v"},
	{"490x", "225x"},
	
	//{"256", "230"},		// not tested
	//{"257", "000"},  	// Not translated (OCLC)
		
	/* OCLC had these mappings, but LC doesn't use 261, 262
	{"261", "210"},
	{"262", "071 and 210"},*/

	//{"042a", "300a"},	// not tested
	//{"025a", "301a"},	// not tested
	//{"027a", "301a"},	// not tested
	//{"029", "301"},		// not tested -- no 029 in format
	//{"032a", "301a"},	// not tested
	//{"033", "000"},  	// Not translated (OCLC)
	//{"036", "301"},		// not tested
	//{"069", "301"},		// not tested
	//{"088", "301"},		// not tested
	//{"270a", "306a"},	// not tested
	//{"340", "307"},		// not tested
	//{"307a", "310a"},	// not needed for FFF
	//{"355", "310"},		// not tested
	//{"357", "310"},		// not tested
	//{"342", "315"},		// not tested
	//{"343", "315"},		// not tested
	//{"351", "315"},		// not tested
	//{"352", "315"},		// not tested
	//{"310", "326"},		// not tested
	//{"315", "000"},  	// Not translated (OCLC)
	//{"321", "326"},		// not tested
	//{"037", "345"},		// not tested
	//{"265b", "345a"},	// not tested
	//{"265a", "345b"},	// not tested
	//{"265f", "345c"},	// not tested
	//{"265c", "345d"},	// not tested


	/*060309 these notes that say "complex mapping not yet developed" actually
			mean that multiple m21 sfs map to a single uni sf. prob s dev a
			way of handling them generically */
	
	{"500a", "300a"},
	{"5005", "000"},
	//{"535a", "300a"},	// complex mapping not yet developed
	//{"541a", "300a"},	// complex mapping not yet developed
	//{"544a", "300a"},	// complex mapping not yet developed
	{"556a", "300a"},		// ignoring 556$z, but could be concat to $a
	{"561a", "300a"},		// ignoring 561$3, but could be concat to $a
	//{"583a", "300a"},	// complex mapping not yet developed
	//{"584a", "300a"},	// complex mapping not yet developed
	{"585a", "300a"},		// ignoring 585$3 and $5, but could be concat to $a
	{"586a", "300a"},		// ignoring 586$3, but could be concat to $a
	{"581a", "300a"},		// ignoring 581$z and $3, but could be concat to $a
	
	{"546a", "302a"},		// there is also a 546z, not yet mapped
	
	{"501a", "303a"},
	{"513a", "303a"},
	{"518a", "303a"},
	{"547a", "304a"},
	//{"545a", "304a"},	// complex mapping not yet developed
	{"525a", "305a"},
	//{"562a", "305a"},	// complex mapping not yet developed
	
	//{"506a", "310a"},	// complex mapping not yet developed
	//{"530a", "310a"},	// complex mapping not yet developed
	//{"540a", "310a"},	// complex mapping not yet developed
	
	{"580a", "311a"},
	{"522a", "313a"},
	//{"536a", "314a"},	// complex mapping not yet developed
	//{"537a", "000a"},  	// Not translated (OCLC) -- not in M21 format
	{"550a", "314a"},
	//{"507a", "315a"},	// graphical material
	
	// all sfs map to uni $a -- each to a separate uni 315 -- not tested
	{"514a", "315a"},
	{"514b", "315a"},
	{"514c", "315a"},
	{"514d", "315a"},
	{"514e", "315a"},
	{"514f", "315a"},
	{"514g", "315a"},
	{"514h", "315a"},
	{"514i", "315a"},
	{"514j", "315a"},
	{"514k", "315a"},
	{"514m", "315a"},
	{"514u", "315a"},
	// not mapping $z as it is a print constant
	
	{"515a", "315a"},
	{"551a", "315a"},
	//{"565a", "315a"},		// complex mapping not yet developed
	{"567a", "315a"},
	{"504a", "320a"},		// no place for 504b in uni
	
	//{"555a", "320 or 321a"},	// 321 seems to make more sense -- THDL
	
	// my mapping -- not tested
	{"510a", "321a"},
	{"510b", "321b"},
	{"510c", "000"},		// no 321 sf for this
	{"510x", "321x"},
	
	{"508a", "322a"},
	{"511a", "323a"},
	
	//{"534a", "324a"},		// complex mapping not yet developed
	
	// for reproductions only -- won't apply to FFF
	//{"533a", "325a"},		// requires all sfs to be put into a single $a -- THDL
	
	{"505a", "327a"},		// MtM has special code to put each item in its own $a
						//	-- for other subfields, waiting until needed
	
	{"502a", "328a"},
	//{"503a", "000a"},  	// Not translated (OCLC)

	// all sfs map to uni $a -- each to a separate uni 315 -- not tested
	{"520a", "330a"},
	{"520b", "330a"},
	{"520u", "330a"},
	
	//{"523a", "000a"},  	// Not translated (OCLC)
	{"524a", "332a"},		// no place for 524$2 or $3 in uni
	{"521a", "333a"},		// no place for 521b in uni
	{"516a", "336a"},
	
	{"538a", "337a"},
	
	//{"539a", "000a"},  	// Not translated (OCLC) -- not in m21 format
	//{"582a", "000a"},  	// Not translated (OCLC)
	
	{"590", "000"},  	// Not translated (OCLC)
	{"599", "000"},  	// Not translated (OCLC)
		
	// this is a dummy so that a 225 can be put into a 410
	{"4101", "4101"},
	
	// these convert to embedded 410's -- need some data in order to work on
	{"800", "000"},
	{"810", "000"},
	{"811", "000"},
	{"830", "000"},
	
	/* not relevant to FFF
	{"760", "410"},
	{"762", "411"},
	{"765", "454"},
	{"767", "453"},
	{"770", "421"},
	{"772", "422"},
	{"773", "464"},
	{"775", "451"},
	{"776", "452"},
	{"777", "423"},
	{"780", "430, 431, 432, 433, 434, 435, 436, or 437"},
	{"785", "440, 441, 442, 443, 444, 445, 446, 447, or 448"},
	{"786", "488"},
	{"787", "488"},
	*/

	//060419 these are the only 2 sfs that FFF asked for; there are lots more
	{"130a", "500a"},
	{"130l", "500m"},
	
	//060419 these are the only 4 sfs that FFF asked for; there are lots more
	{"240a", "500a"},
	{"240f", "500k"},
	{"240k", "500l"},
	{"240l", "500m"},
	
	{"730a", "500a"},
	{"730n", "500h"},
	{"730p", "500i"},
	{"730f", "500k"},
	{"730k", "500l"},
	{"730l", "500m"},
	{"730g", "500n"},
	{"730s", "500q"},
	{"730m", "500r"},
	{"730n", "500s"},
	{"730o", "500t"},
	{"730r", "500u"},

	// placeholders for holding title components of 6xx
	{"500_t", "500t"},
	{"500_x", "500x"},
	{"500_y", "500y"},
	{"500_z", "500z"},

	//{"243", "501"},		// not tested
	
	
	//{"246", "510, 512, 513, 514, 515, 516, 517, or 532"}, (OCLC)
	// -- numbers after the _ are the 2d ind values
	{"246_1a", "510a"},
	{"246_1b", "510e"},
	{"246_1f", "510j"},
	{"246_1g", "510n"},
	{"246_1h", "510n"},
	{"246_1i", "510n"},
	{"246_1n", "510h"},
	{"246_1p", "510i"},
	
	{"246_4a", "512a"},
	{"246_4b", "512e"},
	{"246_4f", "512j"},
	{"246_4g", "512n"},
	{"246_4h", "512n"},
	{"246_4i", "512n"},
	{"246_4n", "512h"},
	{"246_4p", "512i"},
	
	{"246_5a", "513a"},
	{"246_5b", "513e"},
	{"246_5f", "513j"},
	{"246_5g", "513n"},
	{"246_5h", "513n"},
	{"246_5i", "513n"},
	{"246_5n", "513h"},
	{"246_5p", "513i"},
	
	{"246_6a", "514a"},
	{"246_6b", "514e"},
	{"246_6f", "514j"},
	{"246_6g", "514n"},
	{"246_6h", "514n"},
	{"246_6i", "514n"},
	{"246_6n", "514h"},
	{"246_6p", "514i"},
	
	{"246_7a", "515a"},
	{"246_7b", "515e"},
	{"246_7f", "515j"},
	{"246_7g", "515n"},
	{"246_7h", "515n"},
	{"246_7i", "515n"},
	{"246_7n", "515h"},
	{"246_7p", "515i"},
	
	{"246_8a", "516a"},
	{"246_8b", "516e"},
	{"246_8f", "516j"},
	{"246_8g", "516n"},
	{"246_8h", "516n"},
	{"246_8i", "516n"},
	{"246_8n", "516h"},
	{"246_8p", "516i"},
	
	{"246_ba", "517a"},	// meaning blank 2d ind
	{"246_bb", "517e"},
	{"246_bf", "517j"},	// 517 has only $a and $e, so these others should become $e's
	{"246_bg", "517n"},
	{"246_bh", "517n"},
	{"246_bi", "517n"},
	{"246_bn", "517h"},
	{"246_bp", "517i"},
	
	{"246_0a", "517a"},
	{"246_0b", "517e"},
	{"246_0f", "517j"},	// 517 has only $a and $e, so these others should become $e's
	{"246_0g", "517n"},
	{"246_0h", "517n"},
	{"246_0i", "517n"},
	{"246_0n", "517h"},
	{"246_0p", "517i"},
	
	{"246_2a", "517a"},
	{"246_2b", "517e"},
	{"246_2f", "517j"},	// 517 has only $a and $e, so these others should become $e's
	{"246_2g", "517n"},
	{"246_2h", "517n"},
	{"246_2i", "517n"},
	{"246_2n", "517h"},
	{"246_2p", "517i"},
	
	{"246_3a", "517a"},
	{"246_3b", "517e"},
	{"246_3f", "517j"},	// 517 has only $a and $e, so these others should become $e's
	{"246_3g", "517n"},
	{"246_3h", "517n"},
	{"246_3i", "517n"},
	{"246_3n", "517h"},
	{"246_3p", "517i"},
	
	
	{"700t", "517a"},
	{"702t", "517a"},		//060509 dummy tag used for m21 700 if no m21 1xx
	{"710t", "517a"},	
	{"711t", "517a"},	
	
	//060316 all additional data for a 517 goes into repeatable $e's
	{"740a", "517a"},
	//{"740b", "517e"},		// not tested
	//{"740n", "517e"},		// not tested
	//{"740p", "517e"},		// not tested
	//{"740f", "517e"},		// not tested
	//{"740g", "517e"},		// not tested
	
	//{"247", "520"},		// not tested
	
	//{"222", "530"},		// not tested
	//{"210", "531"},		// not tested
	//{"242", "541"},		// not tested
	
	//{"211", "000"},  	// Not translated (OCLC)
	//{"212", "000"},  	// Not translated (OCLC)
	//{"214", "000"},  	// Not translated (OCLC)
	
	//{"045", "000"},  	// 122 and 661 (OCLC), not needed for FFF
	//{"046", "000"},  	// Not translated (OCLC)
	
	
	//************* NOTE FOR SUBJECTS -- UNIMARC REVERSES THE MEANINGS OF $y AND $z *******************/
	
	//{"600", "600, 602, or 604"},	// (OCLC)
	{"600a", "600a"},	// m21 $a must be split into uni $a and $b
	{"600c", "600c"},
	{"600b", "600d"},
	{"600q", "600g"},		// que to gee is correct
	{"600d", "600f"},
	{"600v", "600j"},
	{"600x", "600x"},
	{"600z", "600y"},
	{"600y", "600z"},
	{"6002", "6002"},
	
	//{"610", "601 or 604"},	(OCLC)
	{"610a", "601a"},				/* some data in parens could be an inverted element which should go
									to -> $g (remove parens), with anything following the inversion
									going to $h
								-- but other data in parens could be an addition to name or qualifier
										and should go to $c (remove parens)
								-- I don't see how MC could determine which is which, so we do neither */
	{"610b", "601b"},
	//{"610a", "601c"},	// not tested
	//{"610n", "601d"},	// not tested
	//{"610c", "601e"},	// not tested
	//{"610d", "601f"},	// not tested
	{"610v", "601j"},
	{"610x", "601x"},
	{"610z", "601y"},
	{"610y", "601z"},
	{"6102", "6012"},
	
	// not tested
	//{"611", "601 or 604"},	(OCLC)
	{"611a", "601a"},				/* some data in parens could be an inverted element which should go
									to -> $g (remove parens), with anything following the inversion
									going to $h
								-- but other data in parens could be an addition to name or qualifier
										and should go to $c (remove parens)
								-- I don't see how MC could determine which is which, so we do neither */
	//{"611e", "601b"},	// not tested
	//{"611a", "601c"},	// not tested
	//{"611n", "601d"},	// not tested
	{"611c", "601e"},
	{"611d", "601f"},
	{"611v", "601j"},
	//{"611x", "601x"},
	//{"611z", "601y"},
	//{"611y", "601z"},
	//{"6112", "6012"},

	// dummy tag for family name (600 1st ind == 3)
	{"602a", "602a"},
	{"602x", "602x"},
	{"602z", "602y"},
	{"602y", "602z"},
	{"6022", "6022"},
	
	// dummy tag for name/title subj
	{"6041", "6041"},
	
	{"630a", "605a"},
	{"630n", "605h"},
	{"630p", "605i"},
	{"630v", "605j"},
	{"630f", "605k"},
	{"630k", "605l"},
	{"630l", "605m"},
	{"630g", "605n"},
	{"630d", "605n"}, 	// date of treaty signing -- prob s go to $n -- conflict with 630g?
	{"630s", "605q"},
	{"630r", "605r"},
	{"630n", "605s"},
	{"630o", "605t"},
	{"630x", "605x"},
	{"630z", "605y"},
	{"630y", "605x"},
	{"6302", "6052"},
	
	{"650a", "606a"},
	//{"650b", "606a"},		// 1. not tested -- should be added to uni $a
	{"650v", "606j"},		// 2. uni manual says subfield order is unimportant, ...
	{"650x", "606x"},		// 		... so we leave $j before $x, altho in m21 $v ...
	{"650z", "606y"},		// 		... would follow $x, $y, and $z
	{"650y", "606z"},		// 3. $y and $z are reversed in meaning in UNIMARC
	{"6502", "6062"},
	
	/* do these only when required
	{"654", "606"},
	{"655", "606"},
	{"656", "606"},
	{"657", "606"},
	{"658", "606"},
	*/
	
	//{"754", "606"},	// OCLC says this (taxonomic identification) goes to 650, but I'd like some
						//		corroboration. not in m21 document
	//{"755", "606"},	(OCLC) -- not in m21 format
	
	{"651a", "607a"},
	{"651v", "607j"},
	{"651x", "607x"},
	{"651z", "607y"},
	{"651y", "607z"},
	{"6512", "6072"},
	
	//{"653", "610"},	// THDL

	//{"052", "615"},		// not tested
	//{"072", "615"},		// not tested
	
	{"752a", "620a"},
	{"752b", "620b"},
	{"752c", "620c"},
	{"752d", "620d"},
	
	{"753a", "626a"},
	{"753b", "626b"},
	{"753c", "626c"},
	
	{"043a", "660a"},

	//{"080", "675"},		// not tested
	
	{"082a", "676a"},
	{"0822", "676v"},
	
	{"092", "000"},		// local field

	{"050a", "680a"},
	{"050b", "680b"},
	
	//{"051", "680"},		// not tested
	
	//{"090", "680"},		// (OCLC)
	{"090", "000"},		// local field
	
	//{"055", "686"},		// not tested

	{"060a", "686a"},
	{"060b", "686b"},
	{"0602", "6862"},		// there is no m21 060$2, but this creates the uni 686$2 so we can fill it in MtM
	
	//{"061", "686"},		// not tested
	//{"066", "000"},  		// Not translated (OCLC)
	//{"070", "686"},		// not tested
	//{"071", "686"},		// not tested
	//{"084", "686"},		// not tested
	//{"086", "686"},		// not tested
	//{"096", "686"},		// not tested
	//{"098", "686"},		// not tested
	//{"099", "000"},  		// Not translated (OCLC)
	
	// see code for 
	{"100a", "700a"},		// forename must be split out into $b
	{"100q", "700g"},		// que to gee is correct
	{"100c", "700c"},
	//{"100b", "700d"},		// not tested
	{"100d", "700f"},
	{"100e", "000"},		// relator term, no map. $4 relator code could be mapped though
	//{"100u", "700p"},		// not tested
	{"1004", "7004"},
	
	// can't think where OCLC got 488
	//{"700", "488, 701, or 721"},
	{"700a", "701a"},		// forename must be split out into $b
	{"700q", "701g"},		// que to gee is correct
	{"700c", "701c"},
	//{"700b", "701d"},		// not tested
	{"700d", "701f"},
	{"700l", "517e"},		// this is correct; title-related elements are moved to a 517
	// for 700t see 517a
	//{"700u", "701p"},		// not tested
	{"7004", "7014"},

	//060509 dummy tag for writing 700 to 702 if no 1xx
	{"702a", "702a"},		// forename must be split out into $b
	{"702q", "702g"},		// que to gee is correct
	{"702c", "702c"},
	//{"702b", "702d"},		// not tested
	{"702d", "702f"},
	// for 702t see 517a
	//{"702u", "702p"},		// not tested
	{"7024", "7024"},


	{"110a", "710a"},				/* some data in parens could be an inverted element which should go
									to -> $g (remove parens), with anything following the inversion
									going to $h
								-- but other data in parens could be an addition to name or qualifier
										and should go to $c (remove parens)
								-- I don't see how MC could determine which is which, so we do neither */
	{"110b", "710b"},	// not tested
	//{"110e", "710"},	// not converted -- no place to put it
	//{"110a", "710c"},	// not tested
	//{"110n", "710d"},	// not tested
	//{"110c", "710e"},	// not tested
	//{"110d", "710f"},	// not tested
	//{"110u", "710p"},	// not tested
	{"1104", "7104"},
	
	{"111a", "710a"},				/* some data in parens could be an inverted element which should go
									to -> $g (remove parens), with anything following the inversion
									going to $h
								-- but other data in parens could be an addition to name or qualifier
										and should go to $c (remove parens)
								-- I don't see how MC could determine which is which, so we do neither */
	{"111e", "710b"},
	//{"111a", "710c"},	// not tested
	{"111n", "710d"},
	{"111c", "710e"},
	{"111d", "710f"},
	//{"111u", "710p"},	// not tested
	{"1114", "7104"},

	//{"710", "488 or 711"},	// don't see why OCLC mapped to 488
	// most of this not tested, as FFF has only $a -- see Procedure 2 in spiral
	{"710a", "711a"},		/* some data in parens could be an inverted element which should go
									to -> $g (remove parens), with anything following the inversion
									going to $h
								-- but other data in parens could be an addition to name or qualifier
										and should go to $c (remove parens)
								-- I don't see how MC could determine which is which, so we do neither */
	{"710b", "711b"},	// not tested
	//{"710e", "711b"},	// not tested
	//{"710a", "711c"},	// not tested
	{"710n", "711d"},
	//{"710c", "711e"},	// not tested
	{"710d", "711f"},	
	// for 710t see 517a
	//{"710u", "711p"},	// not tested
	{"7104", "7114"},
	{"7105", "000"}, 
	
	//{"711", "488 or 711"},	// don't see why OCLC mapped to 488
	
	{"711a", "711a"},				/* some data in parens could be an inverted element which should go
									to -> $g (remove parens), with anything following the inversion
									going to $h
								-- but other data in parens could be an addition to name or qualifier
										and should go to $c (remove parens)
								-- I don't see how MC could determine which is which, so we do neither */
	{"711e", "711b"},	// not tested
	//{"711a", "711c"},	// not tested
	{"711n", "711d"},
	{"711c", "711e"},
	{"711d", "711f"},
	// for 711t see 517a
	//{"711u", "711p"},	// not tested
	{"7114", "7114"},
	
	// dummy tag for family name (100 1st ind == 3)
	//{"100_3a", "720a"},	// not tested
	//{"100_3d", "720f"},	// not tested
	//{"100_34", "7204"},	// not tested
	
	/* can't think where OCLC got this
	{"040", "100"},*/
	{"040a", "801b"},	// these four each set the 801 2d ind differently
	//{"040b", "801b"},	// they are not actually segments but separate 801 fields
	//{"040c", "801b"},	// -- we can't list these here or they end up as separate
	//{"040d", "801b"},	// ... fields in the linked list -- handled in code
	// this is optional and causes trouble {"005", "801c"},
	// this is optional and causes trouble {"040e", "801g"},
	
	//{"850", "000"},  // Not translated (OCLC)
	//{"851", "000"},  // Not translated (OCLC)
	//{"852", "000"},  // Not translated (OCLC)
	//{"856", "956"},		// THDL
	//{"871", "000"},  // Not translated (OCLC) -- not in m21 format
	//{"886", "986"},		// THDL
	
	{"902", "000"},
	{"905", "000"},
	{"906", "000"},
	{"907", "000"},
	{"908", "000"},
	{"909", "000"},
	{"910", "000"},
	{"911", "000"},
	{"941", "000"},
	{"942", "000"},
	{"945", "000"},
	{"949", "000"},
	{"950", "000"},
	{"955", "000"},
	{"960", "000"},
	{"981", "000"},
	{"998", "000"},
	{"999", "000"}
};
