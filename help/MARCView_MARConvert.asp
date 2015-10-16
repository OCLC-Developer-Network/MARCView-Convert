<%

'Copyright 1992-2009 Systems Planning (www.systemsplanning.com)
'
'Licensed under the Apache License, Version 2.0 (the "License"); 
'you may not use this file except in compliance with the License. 
'You may obtain a copy of the License at 
'
'    http://www.apache.org/licenses/LICENSE-2.0
'
'Unless required by applicable law or agreed to in writing, software 
'distributed under the License is distributed on an "AS IS" BASIS, 
'WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
'See the License for the specific language governing permissions and 
'limitations under the License.

product="MARConvert"
%>

<HTML>
<HEAD>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=windows-1252">
<TITLE>MARCView&#8482; &amp; MARConvert&#8482; Help</TITLE>

<link rel=stylesheet type='text/css' href='/style.css' media='screen,print'>
<link rel=stylesheet type='text/css' href='/printstyle.css' media='print'>

</head>

<body marginwidth=0 marginheight=0 topmargin=0 leftmargin=0>

<table class=banner cellspacing=0 cellpadding=8>
<tr class=noprint>
<td class=bannerleft>
<a href='/default.asp'><img src='/sp_logo.gif' border=0 alt='Click for Homepage'></a>
</td>

<td class=bannerright>

<% 
if (mid (page, 1, 5) = "/mweb") then 
%>
	<a href='/mweb/' style='color:black;font-weight:normal;text-decoration:none' 
	title='Click for MWeb Homepage'>
	<b>MWeb&#8482; Online Catalog Access</b>&nbsp;&nbsp;&nbsp;
	<img src="/mweb/mweblogosmall.gif" align=absmiddle border=0></a>
<% 
elseif (mid (page, 1, 6) = "/marc/") then 
%>
	<a href='/marc/' style='color:black;font-weight:normal;text-decoration:none' 
	title='Click for MARCView Homepage'>
	<b>MARCView&#8482;</b>&nbsp;&nbsp;&nbsp;
	<img src="/marc/marcview.gif" align=absmiddle border=0></a>
<% 
elseif (mid (page, 1, 12) = "/marconvert/") then 
%>
	<a href='/marconvert/' style='color:black;font-weight:normal;text-decoration:none' 
	title='Click for MARConvert Homepage'>
	<b>MARConvert&#8482;</b>&nbsp;&nbsp;&nbsp;
	<img src="/marconvert/marconvert.gif" align=absmiddle border=0></a>
<% 
else
%>
	<span style='text-align:center;width:100%'>Software for museums, libraries, and consortia</span>
<% 
end if 
%>

</td>
</tr>
</table>

<div id="pagetable">

<h1>
<IMG SRC="MARCView.gif" WIDTH=32 HEIGHT=32 align=absmiddle alt='icon'> <IMG SRC="MARConvert.gif" WIDTH=32 HEIGHT=32 align=absmiddle alt='icon'> MARCView&#8482; &amp; MARConvert&#8482; Help for Open Source Version</h1>

<p>
This page consolidates the MARCView Help page as well as the MARConvert Help pages for all clients. Edit it to suit your own needs.
</p>

<a name='contents'></a>
<h2>Contents</h2>

<h3>MARConvert functions</h3>

<table class=contents border>
<tr>
<td width=300 valign=top>

<p class=menuhead>Client AAA</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#errors'>Error Messages</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client BBB</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#edialog'>The Conversion Process</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client CCC</p>

<p class=menu><a href='#overview'>Overview</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#errors'>Error Messages</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client DDD</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#excel'>Converting from Excel</a></p>
<p class=menu><a href='#dialog'>Conversion Dialog</a></p>
<p class=menu><a href='#table'>Translation Table</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client EEE</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#input'>Input Files</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#table'>Translation Table</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#special'>Special Processing</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client FFF</p>

<p class=menu><a href='#overview'>Overview</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#notes'>Conversion Notes</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

</td>
<td width=300 valign=top>

<p class=menuhead>Client GGG</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#running'>Running <%=product%></a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#errors'>Error Codes</a></p>

<p class=menuhead>Client HHH</p>

<p class=menu><a href='#overview'>Overview</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#errors'>Error Messages</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client JJJ</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#edialog'>Export dialog</a></p>
<p class=menu><a href='#etable'>Translation Table</a></p>
<p class=menu><a href='#langtable'>Language Table</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#validation'>Validation</a></p>
<p class=menu><a href='#errors'>Error Codes</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client KKK</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#etable'>Translation Table</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#special'>Special Processing</a></p>
<p class=menu><a href='#errors'>Error Codes</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client LLL</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#valid'>Validation</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client MMM</p>

<p class=menu><a href='#overview'>Overview</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#bcp'>Using BCP</a></p>
<p class=menu><a href="#transtable">Translation Table</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

</td>
<td width=300 valign=top>

<p class=menuhead>Client NNN</p>

<p class=menu><a href='#overview'>Overview</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#notes'>Conversion Notes</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client PPP</p>

<p class=menu><a href='#overview'>Overview</a></p>
<p class=menu><a href='#output'>Output Options</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#bcp'>Using BCP</a></p>
<p class=menu><a href="#transtable">Translation Table</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client QQQ</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client RRR</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#input'>Input Files</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

<p class=menuhead>Client SSS</p>

<p class=menu><a href='#convert'>Overview</a></p>
<p class=menu><a href='#input'>Input Files</a></p>
<p class=menu><a href='#interactive'>Conversion: Interactive Mode</a></p>
<p class=menu><a href='#command'>Conversion: Command Mode</a></p>
<p class=menu><a href='#table'>Translation Table</a></p>
<p class=menu><a href='#charconv'>Character Conversion</a></p>
<p class=menu><a href='#special'>Special Processing</a></p>
<p class=menu><a href='#inifile'>MARCView.ini</a></p>

</td>

</tr>
</table>

<h3>MARCView functions</h3>

<table class=contents border>
<tr>
<td width=300 valign=top>

<p class=menuhead>File Viewing, Searching, and Printing</p>
<p class=menu><a href='#viewing'>Viewing a File</a></p>
<p class=menu><a href='#copying'>Copying Data</a></p>
<p class=menu><a href='#sendto'>Quick Access to MARCView Using Send-To</a></p>
<p class=menu><a href='#charset'>Display of Diacriticals and Special Characters</a></p>
<p class=menu><a href='#support'>MARCView User Support</a></p>
<p class=menu><a href='#technical'>Technical Details About MARCView</a></p>
<p class=menu><a href='#uninstall'>Uninstalling MARCView</a></p>
</td>
<td width='33%' valign=top>
<p class=menuhead>Command buttons</p>
<p class=menu><a href='#open'>View</a></li></p>
<p class=menu><a href='#first'>First</a></li></p>
<p class=menu><a href='#prev'>Prev</a></li></p>
<p class=menu><a href='#next'>Next</a></li></p>
<p class=menu><a href='#last'>Last</a></li></p>
<p class=menu><a href='#goto'>Goto</a></li></p>
<p class=menu><a href='#find'>Find</a></li></p>
<p class=menu><a href='#again'>Again</a></li></p>
<p class=menu><a href='#hex'>Hex</a></li></p>
<p class=menu><a href='#print'>Print</a></li></p>
<p class=menu><a href='#help'>Help</a></li></p>
<p class=menu><a href='#exit'>Exit</a></li></p>
</td>
<td width='33%' valign=top>
<p class=menuhead>Menu commands</p>
<p class=menu><a href='#open'>File/View</a></p>
<p class=menu><a href='#print'>File/Print</a></p>
<p class=menu><a href='#exit'>File/Exit</a></p>
<p class=menu><a href='#copying'>Edit/Copy</a></p>
<p class=menu><a href='#first'>Navigate/First</a></p>
<p class=menu><a href='#prev'>Navigate/Prev</a></p>
<p class=menu><a href='#next'>Navigate/Next</a></p>
<p class=menu><a href='#last'>Navigate/Last</a></p>
<p class=menu><a href='#goto'>Navigate/Goto</a></p>
<p class=menu><a href='#find'>Navigate/Find</a></p>
<p class=menu><a href='#again'>Navigate/Again</a></p>
<p class=menu><a href='#fonts'>Tools/Fonts</a></p>
<p class=menu><a href='#settings'>Tools/Settings</a></p>
<p class=menu><a href='#statistics'>Tools/Statistics</a></p>
<p class=menu><a href='#hex'>Tools/Hexview</a></p>
<p class=menu><a href='#help'>Help/Help</a></p>
<p class=menu><a href='#about'>Help/About MARCView</a></p>
</td>

</tr>
</table>

<h3>Experimental MARCSQL feature</h3>

<table class=contents border>
<tr>
<td width=300 valign=top>

</td>
<td width='33%' valign=top>
<p class=menuhead>Command buttons</p>
<p class=menu><a href='#sql'>SQL</a></li></p>

</td>
<td width='33%' valign=top>
<p class=menuhead>Menu commands</p>
<p class=menu><a href='#sql'>Navigate/SQL Search</a></p>
<p class=menu><a href='#reset'>Navigate/SQL Reset</a></p>

</td>

</tr>
</table>

<br><p><br>
<hr>
<a name="convert"></a>
<h1 align=center>Client AAA</h1>

<a name='export'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for AAA converts MARC records between the UTF-8 and MARC-8 character sets in either direction. The converted records are written to two different output files, depending on whether they have problem characters. The input file is left unchanged.
</P>

<P>
In addition to the MARC output files, MARConvert creates a report file which can be viewed or printed. Look at the report after converting to see if there were any problems.
</p>

<P>
There is no limit to the number of records in the input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes).
</P>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>
In Interactive Mode you will may convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the <b>Convert</b> button or the <b>File/Convert</b> menu item (they do exactly the same thing). (If you had been viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)
</P>

<P>After you click the <b>Convert</b> button or menu item, a "dialog box" appears for you to specify the character encoding (MARC-8 or UTF-8) of the input file, as well as the filenames you want for the conversion. When the dialog box appears, it shows the settings from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to choose the character encoding, then click the <b>Browse</b> button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.
</P>

<p class=helpnote>
Tip: Select the <b>Conversion</b> radio button before clicking <b>Browse</b>. This will generate the proper extension for the name of the output file.
</p>

<p>
Click the <b>OK</b> button when you are ready to start the conversion. When the run finishes, click the <b>OK</b> button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>
In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert. All files in a batch must be of the same type, either UTF-8 or MARC-8.
</p>


<h3>Create the Command File</h3>

<p>
To use the Command Mode, you must first create a <b>Command File</b>. This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File (included in the installation as a model):
</p>

<dir>
<pre>
MARConvert command file
overwrite	yes
report		"d:\marc\marconvert\AAA\MARConvert.rpt"
conversion	marc-to-utf
"d:\marc\marconvert\AAA\LC_utf8.m8"
"d:\marc\marconvert\AAA\LC_CJK_v19.i49.m8"
</pre>
</dir>

<p>
Notes on the example:
</p>

<ol>
<li>
The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.
</li>
<li>
The second line says that the program should overwrite output files and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more <b>tab characters</b> (not spaces) to separate the word "overwrite" from the "yes" or "no".
</li>
<li>
The third line says where the report file should be created and what it should be called. Use one or more <b>tab characters</b> (not spaces) to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.
</li>
<li>
The fourth line indicates whether this batch run will convert from MARC-8 to UTF-8, or from UTF-8 to MARC-8. Only the first character of the parameter is read, but it aids comprehension to use words rather than a single letter. The parameter may be upper or lower case. Use one or more <b>tab characters</b> (not spaces) to separate the word "conversion" from the parameter
</li>
<li>
The fifth and subsequent lines list the files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.
</li>
<li>
The output files will be created in the directory the input files are in, and will be given the same name as the input file but with the extension "UTF" or "M8", depending on which type of conversion is specified; the files of bad records will have the same name as the input file but with the extension "BAD". The output files for the example above will be named "LC_utf8.UTF", "LC_utf8.BAD", "LC_CJK_v19.i49.UTF", and "LC_CJK_v19.i49.BAD".
</li>
</ol>


<h3>Using Command Mode</h3>

<p>
Command Mode is used as follows:
</p>

<ol>
<li>
Open a Windows "Command Prompt" window. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)
</li>

<li>
You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: 
<pre>
     "c:\program files\marconvert\marconvert" command_file.txt
</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command_file.txt" and exists in the current directory. If the file "command_file.txt" is not in the directory from which you are entering the prompt, you may specify its path also, as in 
<pre>
     "c:\program files\marconvert\marconvert" "d:\mydata\command_file.txt"
</pre>
</li>

<li>
After entering these two items at the prompt, press the Enter key. When MARConvert begins to run, it will open a new window and show you its progress. When prompted, press the enter key again to close this window.
</li>
</ol>

<p class=helpnote>
Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.
</p>

<p class=helpnote>
Tip: If you use the same Command File every time, you can create a shortcut on your Windows desktop that will start MARConvert and read the Command File.
</p>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='charconv'></a>
<h2>Character Conversion</h2>

<p>
MARConvert for AAA converts records in the UTF-8 encoding to MARC-8 encoding, and vice versa. The determination of which direction to perform the conversion is made by the user; MARConvert does not attempt to guess which encoding is used in the input records.
</p>

<p>
MARConvert can convert the entire 16,000+ symbols in the MARC-8 repertoire. In addition, numerous precomposed symbols are handled.
</p>

<p>
Character conversion is performed using tables of equivalent encodings. The following sources were used for these tables:
</p>

<ul>
<li>
<i>Unicode Character Database</i>, version 4.1.0 (www.unicode.org/ucd/)
</li>
<li>
<i>MARC-8 to Unicode XML Mapping File</i>, downloaded 11/22/2005 (www.loc.gov/marc/specifications/codetables.xml)
</li>
<li>
For conversion of precomposed characters, Systems Planning's proprietary symbol conversion table 
</li>
</ul>

<p>
MARConvert uses precomposed characters whenever available. This may prevent round-tripping if the source file used uncomposed characters.
</p>

<p>
According to Gary Smith at OCLC, although it is technically possible to select what are normally G0 characters into the G1 set and vice versa, no one does this and no systems can handle this. Joan Aliprand, one of the Unicode editors, agrees. They feel it was a mistake to ever offer these options. In accordance with this unstated policy, the Codetables.xml file from the Library of Congress provides no mappings for this option. Therefore we have not implemented it. We assume and use G0 for basic character sets and G1 for extended character sets. EACC characters (Chinese, Japanese, and Korean) are assumed to be in the G0 selection.
</p>

<p>
For converting lowercase alpha, beta, and gamma from UTF-8 to MARC-8, we convert to Basic Greek rather than to Greek Symbols. Since there is no way to tell from the data which is wanted, this prevents repeated escaping between these two character sets in a field. This feature may prevent perfect round-tripping if the original MARC-8 file contains Greek Symbols.
</p>

<p>
When converting from UTF-8 to MARC-8 we have implemented the recommendations in Jack Cain's <i>Unicode to MARC8 Conversion: Exceptions to Decomposition of Precomposed Characters</i> (Dec. 13, 2005) except for the last two in Basic Arabic and all of the Cyrillic items. These are actually single characters and the Unicode database does not offer any decomposition for them. 
</p>

<p>
When converting from MARC-8 to UTF-8, if a Numeric Character Representation (NCR) is found of the form <b>&amp;#x1BC;</b>, it is converted to UTF-8.
</p>

<h3>Conversion problems</h3>

<p class=helpnote>
When displaying the results of a conversion, please note that MARConvert can display only Latin-1 characters. Thus it is not a true representation of the results of the conversion. Use MARConvert's <b>Hexview</b> (by clicking the <b>Hex</b> button) to see the actual contents of the records.

<br><br>
Also note that the display is affected by the <b>Character Set of Records</b> in <a href="#settings">Settings</a>.
</p>

<p>
When a character is encountered that cannot be found in the conversion tables, MARConvert inserts characters depending on the target encoding; for UTF-8, a double underscore (Unicode 2017); for MARC-8, the Numeric Character Reference of the character (such as &amp;#xA1B;). In addition, a warning message is logged to the report file and the record is added to the bad-record output file.
</p>

<p>
MARConvert cannot continue if it encounters a malformed MARC record, such as one where there is no Record Terminator. If this occurs frequently, ask us to have MARConvert skip such records and continue after them.
</p>

<p>
If a combining diacritic is not associated with a base character, MARConvert attempts to output the diacritic anyway. However, if the base character is the MARC Field Terminator or Subfield Delimiter, the diacritic is removed and a warning is logged.
</p>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='errors'></a>
<h2>Error Messages</h2>

<table class=embedded border>
<tr>
<th>Message</th>
<th>Discussion</th>
</tr>

<tr>
<td>
&amp;#xyyy; (UTF-8 zzz) not found in conversion tables
</td>
<td>
The specified UTF-8 character could not be found in the conversion tables
</td>
</tr>

<tr>
<td>
Illegal MARC-8 character yyy, codetable z
</td>
<td>
The specified MARC-8 character has not been defined in the specified code table
</td>
</tr>

<tr>
<td>
MARC-8 yyy not found in conversion tables, codetable z
</td>
<td>
The specified MARC-8 character could not be found in the conversion tables for the specified codetable. The codetable is the specification of whether the character is in Basic Latin, Extended Latin, Basic Greek, etc.
</td>
</tr>

<tr>
<td>
Program removed diacritics modifying yyy
</td>
<td>
The specified MARC-8 character, either a Field Terminator or a Subfield Delimiter, is not allowed to be modified by diacritics.
</td>
</tr>

<tr>
<td>
Record longer than 99,999 characters
</td>
<td>
Addition of the specified field would have caused the output record to be longer than 99,999 bytes, which is the MARC maximum
</td>
</tr>

<tr>
<td>
Status code z
</td>
<td>
A generic warning that a record could not be processed correctly. This message will probably never be seen. If it is, contact Systems Planning.
</td>
</tr>

<tr>
<td>
Too many modifiers for zzz, contact Systems Planning
</td>
<td>
More than 10 modifiers were found for the specified base character. If this is encountered, contact Systems Planning to have this limit raised.
</td>
</tr>

<tr>
<td>
WARNING: Converting xxx to yyy
</td>
<td>
In converting from UTF-8 to MARC-8, this message means that the symbol xxx could not be found in the LC code tables, and the Systems Planning symbol table was used instead. This can occur when LC changes the mapping of a symbol. For example, the MARC-8 ALIF (AE) mapping was recently changed from Unicode 02BE to Unicode 02BC. Since older UTF-8 records may contain the older 02BE, they are converted to MARC-8 AE for convenience.
</td>
</tr>

</table>
<br>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows directory. This file is shared with our MARCView&#8482; product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<hr>

<a name="convert"></a>
<h1 align=center>Client BBB</h1>

<a name='import'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for BBB converts MARC records in the MARC-8 character encoding to MARC records in the UTF-8 encoding. A report file is also produced that tells whether the conversion was successful and how many records were converted.
</P>

<p>
There is no limit to the number of records that may be in an input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes). The input file is not affected in any way by converting it; the converted records are put into a new file. 
</P>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='edialog'></a>
<h2>The Conversion Process</h2>
<p>
There are two modes of use, Interactive and Command.
</p>

<h3>Interactive Mode</h3>

<p>
In Interactive Mode you will may convert a single file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the "Convert" button or the File/Convert menu item (they do exactly the same thing). (If you were viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</P>

<P>Begin to fill in the Dialog Box by telling MARConvert what file to convert. When the Dialog Box appears, the filenames shown are those from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the Browse button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</P>

<p>
After conversion you may view the file in MARConvert by clicking the "View" button. MARConvert displays both MARC-8 and UTF-8 files. (Warning: Although the conversion process can handle any combination of characters and diacriticals, the viewer cannot, as it uses the Latin-1 character set for display; see <a href="#charset">Display of Diacriticals and Special Characters</a> for details).
</p>

<h3>Command Mode</h3>

<p>
In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
</p>

<p>
To use the Command Mode, you must first create a "Command File". This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File:
</p>

<pre>
     MARConvert parameter file, 3/13/04
     overwrite    yes
     report       "d:\marc\marconvert\BBB\MARConvert.rpt"
     "d:\marc\marconvert\BBB\brkrtest.mrc"
     "d:\marc\data\good\diacrit"
     "d:\marc\data\good\harvard.txt"
</pre>

<p>
Notes on the example:
</p>

<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>
<li>The second line says that the program should overwrite output files and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more tab characters to separate the word "overwrite" from the "yes" or "no".</li>
<li>The third line says where the report file should be created and what it should be called. Use one or more tab characters to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
<li>The fourth and subsequent lines list the MARC-8 files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
</ol>

<p class=helpnote>
The output files will be created in the directory from where you start the program (see below), and will be given the same name as the input file but with the extension "UTF". The output files for the example above will be named "brkrtest.UTF", "diacrit.UTF", and "harvard.UTF".
</p>

<p>
Command Mode is used as follows:
</p>

<ol>
<li>Open a Windows "Command Prompt" window. In Windows 2000 this is done by selecting Start/Programs/Accessories/Command Prompt. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>
<li>Navigate to the directory where you want the output files to be created.</li>
<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: 
<pre>
     "c:\program files\marconvert\marconvert" command.txt
</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command.txt" and exists in the "current directory". If the file "command.txt" were not in the directory from which you are entering the prompt, you may specify its path also, as in 
<pre>
     "c:\program files\marconvert\marconvert" "d:\mydata\command.txt"
</pre>
Or you may decide to use a different name for the Command File; if so just specify it as in this example:
<pre>
     "c:\program files\marconvert\marconvert" "d:\mydata\conversion_list.txt"
</pre>
</li>
<li>After entering these two items at the prompt, press the Enter key. When MARConvert begins to run, it will open a new window and show you its progress. When prompted, press the enter key again to close this window.</li>
</ol>

<p class=helpnote>
Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.
</p>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='charconv'></a>
<h2>Character Conversion</h2>

<p>
<%=product%> is able to convert any combination of MARC-8 characters, including complex cases where a letter is modified by more than one diacritic, or where a diacritic modifies a special character. A sample file from the Library of Congress, called Brkrtest.mrc, was installed with <%=product%>, and contains many unusual characters for you to test with. (Note that there are a few errors in this file, so do not worry if it appears to display the wrong symbol. For example, we noticed that what was described as the sound recording copyright symbol in one record was actually coded with the MARC-8 code for the lowercase script L.)
</p>

<p>
For the source of the translations between MARC-8 and UTF-8, we used this document from the Library of Congress MARC website: <a href="http://lcweb2.loc.gov/cocoon/codetables/45.html">lcweb2.loc.gov/cocoon/codetables/45.html</a>
</p>

<p>
For common combinations of diacritics and letters, <%=product%> outputs the so-called "normalized" Unicode, such as LATIN SMALL LETTER E WITH ACUTE. Rare combinations are output as the letter followed by the diacritic(s), so that i modified by a candrabindu will be output as LATIN SMALL LETTER I followed by COMBINING CANDRABINDU. Products that handle Unicode are required to display the unnormalized form identically to the normalized form. This approach saves having <%=product%> store tens of thousands of combinations, which would slow down the conversion process. The rare cases will occur mostly in romanized forms of South Asian languages. We will add the normalized form for any that are found to be problems.
</p>

<p>
Note that when you view the UTF-8 output in a product other than MARConvert, you will see the correct characters only if you display them using a font installed on your computer that contains the characters.
</p>

<p>
<%=product%> does not handle escapes to non-ANSEL character sets such as Greek. If this capability is required, please contact Systems Planning. If an escape code is detected, a warning will be displayed at the end of the run.
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows folder. This file is shared with our MARCView product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>

<hr>
<a name=overview></a>
<h1 align=center>Client CCC</h1>
<h2>Overview</h2>
<p>This customized version of MARConvert for CCC converts MARC21 records from UTF-8 to MARC-8 and from MARC-8 to UTF-8. The input files are left unchanged.</p>
<p>In addition to the output files, MARConvert creates a report file which can be viewed or printed. After converting, the report should be looked at to see if there were any problems. The report provides a count of records converted, as well as a listing and count of problem records.</p>
<p>There is no limit to the number of records in the input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes).</p>


	<div class=gotop><a href="#contents">Top</a></div>


<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>In Interactive Mode you may convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the <b>Convert</b> button or the <b>File/Convert</b> menu item (they do exactly the same thing). (If you had been viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</p>

<p>After you click the <b>Convert</b> button or menu item, a dialog box appears for you to specify the file to be converted, the new file, and the report file. When the dialog box appears, it shows the settings from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the <b>Browse</b> button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</p>

<p>Click the <b>OK</b> button when you are ready to start the conversion. When the run finishes, click the <b>OK</b> button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.</p>


	<div class=gotop><a href="#contents">Top</a></div>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>
<p>In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
 Each run can convert files in only one direction (either UTF-8 to MARC-8, or MARC-8 to UTF-8).
</p>
<h3>Creating the Command File</h3>
<p>To use the Command Mode, you must first create a <b>Command File</b>. This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File (included in the installation as a model):</p>
<pre>     MARConvert command file<br>     overwrite yes<br>     report "d:\marc\marconvert\CCC\MARConvert.rpt"<br>     "conversion utf-to-marc"<br>     "d:\marc\marconvert\CCC\test.oc"<br>     "d:\marc\marconvert\CCC\second.utf"</pre>
<p>Notes on the example:</p>
<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>
<li>The second line says that the program should overwrite the output file and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more tab characters or spaces to separate the word "overwrite" from the "yes" or "no".</li>
<li>Command Mode creates a single report file for all MARC files converted. The third line says where this report file should be created and what it should be called. Use one or more tab characters or spaces to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
<li>The fourth line indicates whether this batch run will convert from MARC-8 to UTF-8, or from UTF-8 to MARC-8. Only the first character of the parameter is read, but it aids comprehension to use words rather than a single letter. The parameter may be upper or lower case. Use one or more tab characters or spaces to separate the word "conversion" from the parameter.</li>
<li>The fifth and subsequent lines list the files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
</ol>
<li>The output files will be created in the directory the input files are in, and will be given the same name as the input file but with the extension "M8" or "UTF", depending on which type of conversion is specified. The output files for the example above will be named "test.M8" and "second.M8".</li>
<h3>Using Command Mode</h3>
<p>Command Mode is used as follows:</p>
<ol>
<li>Open a Windows "Command Prompt" window. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>
<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: </li>
<pre>     "c:\program files\marconvert\marconvert" command_file.txt</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command_file.txt" and exists in the current directory. If the file "command_file.txt" is not in the directory from which you are entering the prompt, you may specify its path also, as in
<pre>     "c:\program files\marconvert\marconvert" "d:\mydata\command_file.txt"</pre>
</li>
<li>After entering these two items at the prompt, press the Enter key. When MARConvert begins to run, it will open a new window and show you its progress. When prompted, press the enter key again to close this window.</li>
</ol>
<p class=helpnote>Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.</p>
<p class=helpnote>Tip: If you use the same Command File every time, you can create a shortcut on your Windows desktop that will start MARConvert and read the Command File.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='notes'></a>
<a name='charconv'></a><h2>Character Conversion</h2>
<p>MARConvert for CCC converts records in the UTF-8 encoding to MARC-8 encoding, and vice versa. The determination of which direction to perform the conversion is made by the user; MARConvert does not attempt to guess which encoding is used in theinput records.</p>
<p>MARConvert can convert the entire 16,000+ symbols in the MARC-8 repertoire. In addition, numerous precomposed symbols are handled.</p>

<p>Character conversion is performed using tables of equivalent encodings. The following sources were used for these tables:</p>

<ul>
<li><i>Unicode Character Database</i>, version 4.1.0 (www.unicode.org/ucd/).</li>
<li><i>MARC-8 to Unicode XML Mapping File</i>, September 2004 (www.loc.gov/marc/specifications/codetables.xml).</li>
<li>For conversion of precomposed characters, Systems Planning's proprietary symbol conversion table </li><li>For normalization of certain MARC-8 characters, Jack Cain's <i>Unicode to MARC8 Conversion: Exceptions to Decomposition of Precomposed Characters</i> (Dec. 13, 2005), in <i>MARC Proposal No. 2006-04.</i> (www.loc.gov/marc/marbi/2006/2006-04.html). MARConvert does not implement the last two recommendations in Basic Arabic and all of the Cyrillic items. These are actually single characters and the Unicode database does not offer any decomposition for them.</li>
</ul>

<p>MARConvert uses precomposed characters whenever available. This may prevent round-tripping if the source file used uncomposed characters.</p>
<p>Although it is technically possible to select what are normally G0 characters into the G1 set and vice versa, no one does this and no systems can handle this. It is generally considered that it was a mistake to ever offer these options. In accordance with this unstated policy, the Codetables.xml file from the Library of Congress provides no mappings for this option. Therefore we have not implemented it. We assume and use G0 for basic character sets and G1 for extended character sets. EACC characters(Chinese, Japanese, and Korean) are assumed to be in the G0 selection.</p>
<p>Lowercase alpha, beta, and gamma in UTF-8 are converted to MARC-8 Basic Greek rather than to Greek Symbols. Since there is no way to tell from the data which is wanted, this prevents repeated escaping between these two character sets in a field. Thisfeature may prevent perfect round-tripping if the original MARC-8 file contains Greek Symbols.</p>
<p>The ligature that spans two characters is constructed of two halves in MARC-8: EB (ligature, first half) and EC (ligature, second half). The tilde that spans two characters is constructed of two halves in MARC-8: FA (double tilde, first half) and FB (double tilde, second half). The preferred Unicode/UTF-8 mapping is to the single character ligature that spans two characters, U+0361 and the single character tilde U+0360; these are encoded between the two characters to be spanned. However, MARConvert for Backstage converts these to the two half ligatures or two half tildes, which was the preferred mapping before September 2004.</p>
<p>When converting from MARC-8 to UTF-8, if a Numeric Character Representation (NCR) is found of the form <b>&amp;#x1BC;</b>, it is converted to UTF-8.</p>


<h3>Conversion problems</h3>

<p class=helpnote>When displaying the results of a conversion, please note that MARConvert can display only Latin-1 characters. Thus it is not a true representation of the results of the conversion. Use MARConvert's <b>Hexview</b> (by clicking the <b>Hex</b> button) to see the actual contents of the records.<br><br>Also note that the display is affected by the <b>Character Set of Records</b> in <a href=#settings>Settings</a>.</p>
<p>When a character is encountered that cannot be found in the conversion tables, MARConvert inserts characters depending on the target encoding; for UTF-8, a Geta (Unicode 3013); for MARC-8, the Numeric Character Reference of the character (such as &amp;#xA1B;). In addition, a warning message is logged to the report file.</p>
<p>MARConvert cannot continue if it encounters a malformed MARC record, such as one where there is no Record Terminator. If this occurs frequently, ask us to have MARConvert skip such records and continue after them.</p>
<p>If a combining diacritic is not associated with a base character, MARConvert attempts to output the diacritic anyway. However, if the diacritic is at the end of a field or subfield, the diacritic is removed and a warning is logged.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name="errors"></a><h2>Error Messages</h2><p>Character-conversion messages are prefixed by "Problem" if the data could not be converted, or by "Warning" for issues that probably do not affect the data.</p>

<table class=embedded border>
<tr><th>Message</th><th>Discussion</th></tr><tr><td>Internal error, contact Systems Planning</td><td>The program cannot run. Contact Systems Planning</td></tr><tr><td>Problem: UTF-8 zzz not found in conversion tables</td><td>The specified UTF-8 character could not be found in the conversion tables</td></tr><tr><td>Problem: Illegal MARC-8 character yyy, codetable z</td><td>The specified MARC-8 character has not been defined in the specified code table</td></tr><tr><td>Problem: MARC-8yyy not found in conversion tables, codetable z</td><td>The specified MARC-8 character could not be found in the conversion tables for the specified codetable. The "z" is the code for the specified codetable: Basic Latin, Extended Latin, Basic Greek, etc.</td></tr><tr><td>Problem: Record longer than 99,999 characters</td><td>Addition of the specified field would have caused the output record to be longer than 99,999 bytes, which is the MARC maximum</td></tr><tr><td>Problem: Too many modifiers for zzz, contact Systems Planning</td><td>More than 10 modifiers were found for the specified base character. If this is encountered, contact Systems Planning to have this limit raised.</td></tr><tr><td>Problem: UTF-8 zzz not found in conversion tables</td><td>The specified UTF-8 character could not be found in the conversion tables</td></tr><tr><td>Status code z</td><td>A generic warning that a record could not be processed correctly. This message will probably never be seen. If it is, contact Systems Planning.</td></tr><tr><td>Warning: Converting xxx to yyy</td><td>In converting from UTF-8 to MARC-8, this message means that the symbol xxx could not be found in the LC code tables, and the Systems Planning symbol table was used instead. This can occur when LC changes the mapping of a symbol. For example, the mapping of MARC-8 ALIF (AE) was recently changed from Unicode 02BE to Unicode 02BC. Since older UTF-8 records may contain the older 02BE, they are converted to MARC-8 AE for convenience and this warning isissued.</td></tr><tr><td>Warning: Program removed diacritics modifying yyy</td><td>The MARC-8 character yyy, either a Field Terminator or a Subfield Delimiter, is not allowed to be modified by diacritics.</td></tr></table><br>
<div class=gotop><a href="#contents">Top</a></div>
<a name='inifile'></a>
<h2>MARCView.ini</h2>
<p>Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows directory. This file is shared with our MARCView&#8482; product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. </p>
<div class=gotop><a href="#contents">Top</a></div>

<br><p><br>
<hr>
<a name="convert"></a>
<h1 align=center>Client DDD</h1>

<a name='import'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for DDD converts Amico records to MARC, suitable for loading into the Innovative Interfaces OPAC. (In MARConvert terminology, this is an "import" into the MARC format.)
</P>

<p>
The input is a text file in which fields are separated by tabs; each record is a new line. The first line of the file contains the Amico fieldnames, separated by tabs. Here is an example of an input file, showing only the first four fields for space reasons:
</p>

<pre>
     AID             OTY    OTN                     CRN                   
     DDD_.1955.1781  Print  """Luke Blackburn"""    "STULL, HENRY, AFTER" 
     DDD_.1955.1782  Print  The Dells of Wisconsin  "DU PEYRON, F. A."    
     DDD_.1955.1783  Print  Afterglow               "DU PEYRON, F. A."    
</pre>

<P>
The conversion is controlled by a text file called a <B>Translation Table</B>. MARConvert uses the information in this file to convert each field from text format to MARC format. We have supplied a Translation Table for the initial data, but you may need to modify it to meet changing circumstances. Note that the Translation Table can control only some functions; complex changes may have to be made in the program code by Systems Planning.
</P>

<P>The conversion process results in three files:</P>

<UL>
<LI>A file of MARC records</LI>
<LI>A file of bad records, also in MARC format. Bad records are those in which a proper MARC record could not be built, because of missing mandatory data or similar cause.</LI>
<LI>A report file which can be viewed or printed. Look at the report after importing to see if there were any problems.</LI></UL>

<P>
Typically all three files are given the same name, with the file of good MARC records having the extension "MRC", the bad records "BAD", and the report "RPT". You may override these filenames and extensions when you perform an import.
</P>

<p>
There is no limit to the number of records in the input file, but the output file size is limited to 2 gigabytes (2,147,483,647 bytes). The input file is not affected in any way by converting it.
</P>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='excel'></a>
<h2>Converting from Excel</h2>
<p>
Converting records from Excel spreadsheets into the input-file format is a manual function. Here are the steps to do this:
</p>

<ol>
<img src="MC_DDD_Fig1.jpg" align="right" hspace=20 vspace=10>
<li>Open the spreadsheet in Excel</li>
<li>Choose File/Save As from the menu</li>
<li>Navigate to the directory in which you want to save the file</li>
<li>Select "Text (Tab delimited) (*.txt)" from the dropdown labeled "Save as type"</li>
</ol>

<p>
The spreadsheet will be saved as a text file with the same name as the original spreadsheet but with "txt" as the filename extension. (You can change the filename if you wish, but it is easier to keep track of your data if all formats of the same dataset have the same filename.)
</p>

<p>
Notice that Excel puts quotation marks around fields containing commas. For fields that are surrounded by quotation marks in the original spreadsheet, such as the title "Luke Blackburn", Excel puts in three sets of quotation marks. MARConvert knows how to handle both of these cases.
</p>

<p>
When you close Excel after this conversion process, it will display a dialog box saying that the new file is not in Microsoft Excel format. The rest of this dialog box makes little sense, but you should select the "No" button.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='dialog'></a>
<h2>Conversion Dialog</h2>
<P>To perform a conversion, run MARConvert, then click on the "Convert" button or the File/Convert menu item (they do exactly the same thing). (If you were viewing a file in MARConvert, it will close, since certain resources are needed by the import process. Simply reopen the file when the import is finished.)</P>

<P>Begin to fill in the Dialog Box by telling MARConvert what file to convert. When the Dialog Box appears, the filenames shown are those from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the Browse button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names you wish. All names and parameters are saved for you for the next time.</P>

<p>
When you install MARConvert, the default Translation Table "Amico.mvi" in installed with the program (normally in C:\Program Files\MARConvert). You may move or copy it to another location if that is more convenient.
</p>

<p class=helpnote>
For Amico conversion, you will probably always be using the same Translation Table, so this defaults to whatever Translation Table was used for the previous run. You may override this if you wish.
</p>

<P>You may also specify the number of records to convert. This can be helpful when you are testing changes to the Translation Table. Use 0 (zero) when you want to convert the entire file. There is no limit to the number of records you can convert; the "Maximum records" setting in the <a href="#settings">Settings dialog</a> does not apply to conversion. However, the input file is limited to 2 GB (2,147,483,647 bytes). </P>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='table'></a>
<h2>Translation Table</h2>

<P>
The Translation Table can be edited with Notepad. The Table consists of three columns separated with tabs. You may use as many tabs as you like to line up the columns for legibility. DO NOT USE SPACES BETWEEN COLUMNS.
</P>

<DIR>

<P><b>Translation Table as delivered</b></P>

<pre>
DDD Amico Translation Table
FieldSep	T
LDR		"LLLLLnkm  22BBBBBn  4500"
007		"ke ca "
007		"cr |||||||||||"
008&1		"DDDDDD"
008&2		"s19uu    xx nnn            sneng d"
035a		AID
040a		"removed"
040c		"removed"
090b		"Amico record"
100-		"1 "
100a		CRN
245-		"10"
245a		OTN		"+++$h[slide]"
260c		OCT
300a		OMD
300c		MET
500a		ALY		"Amico Library: +++"
511a		CRT
535a		OON
535b		OOP
541e		OOA
545a		CRC
655-		" 7"
655a		OTY		"+++$2aat"
774-		"18"
774t		RID
774n		RIR
774o		RIL
845u		ORL		"+++$zConnect to rights and reproduction policy"
000		CDT
000		OOC
000		RIP
//
</pre>

</DIR>

<h3>Notes</h3>

<ol>
<li>The first line of the Translation Table is ignored. You may record comments here.</li>

<li>The second line specifies the <B>Field Separator</B> used between fields in the input file. The capital T stands for the tab character.</li>
</li>
</ol>


<h3>Field mapping</h3>

<p>
After the second line, the Translation Table consists of one line for each part of the MARC records to be created. Each line may represent an entire MARC field, a byte-range of a fixed field, or one or more subfields of a variable field. In addition, a field or subfield may have more than one <b>Segment</b>.
</p>

<p>
Each line in the Translation Table requires two columns: the description of the part of the MARC record, and the source of data for that part. There is an optional third column for <b>Attachments</b> that are to be added.
</p>

<ul>
<LI>The first column specifies the MARC fields that are to be imported. You should specify subfield codes for MARC Data Fields (010 - 999). Use a hyphen (-) to mean "indicators," as shown above for the 100 and 245 fields. You may also import the MARC indicators as an Attachment to be appended to the first subfield.</li>

<li>
<b>Segments</b> are represented by following the field tag or subfield code with an ampersand and a digit. Segments must be in the Translation Table in ascending order. The Translation Table shown above uses two Segments for the 008 field, one for bytes 0-5 (Date Entered on File) and one for the remainder of the 008. (MARConvert will replace the DDDDDD with the Date Entered on File.)
</li>

<LI>
The second column shows the Amico field used to create that part of the MARC record. In the example above, the 100a (subfield a) is to be produced from the Amico CRN field.
</li>

<li>
Alternatively, column two may contain a <b>Constant</b>, such as those for the Leader and 007 fields. Constants must be within double quotation marks. (MARConvert automatically replaces the LLLLL and BBBBB with the Logical Record Length and Base Address of Data.)
</li>

<li>
Using Segments, any number of Amico fields and Constants can be used to generate a MARC field.
</li>

<li>
The third column specifies optional <b>Attachments</b> to be appended to the Amico data. For example, the line for 245a specifies that a subfield h containing "[slide]" is to be appended to the Amico OTN data. In other words, the 245 will have two subfields: $a from the OTN and $h from the Attachment in column three of the Translation Table. The "+++" before the Attachment means that the Attachment will follow the OTN data. In the 500a, the "+++" following the Attachment means the words "Amico Library: " will precede the ALY data. You may even put the "+++" in the middle of a Attachment to show that the variable data should be inserted into the Attachment.
</li>

<li>Amico fields found in the input file that are not represented in the Translation Table will generate a warning message in the report. Therefore the last three lines of the Translation Table shown above document the fact that the CDT, OOC, and RIP fields are being ignored intentionally.</li>

<LI>The Translation Table must end with a line whose first character is a slash.</LI>

<li>Fields in the MARC records are created in the order they are listed in the Translation Table. Normally MARC fields are in ascending numeric order.</li>
</UL>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='charconv'></a>
<h2>Character Conversion</h2>

<p>
MARConvert for DDD expects records in the Latin-1 character encoding, and converts them to the MARC-8 character encoding in the output file.
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows folder. This file is shared with our MARCView product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<br><p><br>
<hr>
<a name="convert"></a>
<h1 align=center>Client EEE</h1>

<a name='import'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for EEE converts text records exported from BASIS TechLib records to MARC. (In MARConvert terminology, this is an "import" into the MARC format.) Bibliographic records are converted to the MARC21 Bibliographic format, and holdings records are converted to the MARC21 Holdings format.
</P>

<P>
The conversion is controlled by a text file called a <B>Translation Table</B>. MARConvert uses the information in this file to convert each field from the text format to MARC format. We have supplied a Translation Table for the initial data, but you may need to modify it to meet changing circumstances. Note that the Translation Table can control only some functions; complex changes may have to be made in the program code by Systems Planning.
</P>

<P>The conversion process results in three files:</P>

<UL>
<LI>A file of MARC records</LI>
<LI>A file of bad records, also in MARC format. Bad records are those that contain field not in the Translation Table or have some other data problem.</LI>
<LI>A report file which can be viewed or printed. Look at the report after importing to see if there were any problems.</LI></UL>

<P>
Typically all three files are given the same name, with the file of good MARC records having the extension "MRC", the bad records "BAD", and the report "RPT". You may override these filenames and extensions when you perform an import.
</P>

<p>
There is no limit to the number of records in the input file. However, the output file is limited to 2 GB (2,147,483,647 bytes). The input file is not affected in any way by converting it.
</P>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='input'></a>
<h2>Input Files</h2>

<p>
The input files are in "tagged-text" format, in which each line typically contains a three-character field type, a field label, and the field data (but see exceptions below). <b>Example</b>:
</p>

<dir>
<pre>
R  CAT                                 <<< record # 1 >>>
E  ABSTRACT(1)
L70One briefcase with 10 specimens (Approx. 60 lbs.) and one manual. Can 
L62be checked out for 28 days. Must be returned via company mail.
V  ADD_DT(1)=19941202
V  AU(1)=Flaw Manufacturing Technology
...
R  CAT                                 <<< record # 2 >>>
</pre>
</dir>

<p>
Field types may be "R" to indicate the start of a record, "V" for a field whose label and data are on the same line, separated by "=", "E" to indicate a line which contains only the field label, and "L" for lines that contain the data following an "E" field. The two characters after an "L" are the number of data characters on the line.
</p>

<p>
Input files are named for the kind of data they contain. For example, Books_cat.txt contains bibliographic records for books, Books_copy.txt contains holdings records for books.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>
In Interactive Mode you will may convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the "Convert" button or the File/Convert menu item (they do exactly the same thing). (If you are viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</P>

<P>
After you click the Convert button or menu item, a "dialog box" appears for you to specify filenames. When the dialog box appears, it shows the filenames from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the Browse button -- when you select an input file, default names for the output files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</P>

<p>
When you install MARConvert, the default Translation Tables Techlib_cat.mci and Techlib_copy.mci are installed with the program (normally in C:\Program Files\MARConvert). You may move or copy them to another location if that is more convenient.
</p>

<p>
The dialog box also provides a way to specify whether the input file contains catalog data or copy data. This will tell MARConvert whether to generate bibliographic or holdings records.
</p>

<P>
You may also specify the number of records to convert. This can be helpful when you are testing changes to the Translation Table. Use 0 (zero) when you want to convert the entire file. There is no limit to the number of records you can convert; the "Maximum records" setting in the <a href="#settings">Settings dialog</a> does not apply to conversion. However, the output files are limited to 2 GB (2,147,483,647 bytes). 
</P>

<p>
Click the "OK" button when you are ready to start the conversion. When the run finishes, click the "OK" button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>
In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
</p>

<p>
To use the Command Mode, you must first create a "Command File". This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File:
</p>

<dir>
<pre>
MARConvert command file, 5/16/05
overwrite     yes
report        "d:\marc\marconvert\EEE\MARConvert.rpt"
catalogrun    yes
"d:\marc\marconvert\EEE\books_cat.txt"
"d:\marc\marconvert\EEE\realia_cat.txt"
</pre>
</dir>

<p>
Notes on the example:
</p>

<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>
<li>The second line says that the program should overwrite output files and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more tab characters to separate the word "overwrite" from the "yes" or "no".</li>
<li>The third line says where the report file should be created and what it should be called. Use one or more tab characters to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
<li>The fourth line is where you specify whether this run is for catalog records or copy records. The two cannot be mixed in the same run because they use different Translation Tables. Use one or more tab characters to separate the word "catalogrun" from the "yes" or "no".</li>
<li>The fifth and subsequent lines list the TechLib files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
</ol>

<p class=helpnote>
The output files will be created in the directory the input files are in, and will be given the same name as the input file but with the extension "MRC"; the files of bad records will have the same name as the input file but with the extension "BAD". The output files for the example above will be named "books_cat.MRC", "books_cat.BAD", "realia_cat.MRC", and "realia_cat.BAD".
</p>

<p>
Command Mode is used as follows:
</p>

<ol>
<li>Open a Windows "Command Prompt" window. In Windows 2000 this is done by selecting Start/Programs/Accessories/Command Prompt. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>
<li>Navigate to the directory where you want the output files to be created.</li>
<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: 
<pre>
     "c:\program files\marconvert\marconvert" command_file.txt
</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command_file.txt" and exists in the "current directory". If the file "command_file.txt" is not in the directory from which you are entering the prompt, you may specify its path also, as in 
<pre>
     "c:\program files\marconvert\marconvert" "d:\mydata\command_file.txt"
</pre>
Or you may decide to use a different name for the Command File; if so just specify it as in this example:
<pre>
     "c:\program files\marconvert\marconvert" "d:\mydata\conversion_list.txt"
</pre>
</li>
<li>After entering these two items at the prompt, press the Enter key. When MARConvert begins to run, it will open a new window and show you its progress. When prompted, press the enter key again to close this window.</li>
</ol>

<p class=helpnote>
Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='table'></a>
<h2>Translation Table</h2>

<P>
The Translation Table can be edited with Notepad. The Table consists of three columns separated with tabs. You may use as many tabs as you like to line up the columns for legibility.
</P>

<DIR>

<P><b>Translation Table for bibliographic records as delivered</b></P>

<pre>
TechLib Catalog Translation Table
LabelSep	"="
RecordSep	"R  C"
LDR&1		"LLLLLn"
LDR&2		MTYPE
LDR&3		"  22BBBBB"
LDR&4		LEVEL
LDR&5		"  4500"
001		CATNO
005		REV_DT
007		MC_007
008&1		"DDDDDD"
008&2		"||||||||||||||||||||||||||||||||||"
010a		M010
020a		ISBN
022a		ISSN
030a		CODEN
035a		OCLC		"(OCoLC)+++"
037a		M037
040a		M040
041-		"0 "
041a		M041
086a		M086
088a		M088
092a		CALL
100-		"1 "
100a		AU
110-		"2 "
110a		CORP
111-		"2 "
111a		M111
130-		"0 "
130a		M130
245a		TI
246-		"1 "
246a		ALTTI
247-		"00"
247a		M247
250a		EDTN
260a		PUBL
260c		YEAR
300a		MC_300a
300b		MC_300b
300c		MC_300c
310a		M310
321a		M321
440-		" 0"
440a		SERIES
500a		NOTES
504a		M504
505-		"8 "
505a		CONTENTS
520-		"3 "
520a		ABSTRACT
583a		CHECK_IN_NOTE
590a		M590
595a		M505
650-		" 4"
650a		SUBJ
653a		KW
700-		"12"
700a		M700
710-		"22"
710a		MC_710a
711-		"22"
711a		MC_711a
730-		"02"
730a		MC_730a
780-		"00"
780a		M780
785-		"00"
785a		M785
787-		"0 "
787a		M787
866-		" 0"
866a		HOLDINGS
000		ADD_DT
000		AUTH
000		AU_SX
000		CALL_LC_SORT
000		CLAIM_MODE
000		CLAIM_MODE_NM
000		CLAIM_PERIOD_DAYS
000		COLL
000		CPV
000		DAUTH
000		DAY_MONTH
000		DAY_WEEK
000		DISP_ISBN_ORDER
000		DISPLAY_TI
000		DOC
000		ENUM_TYPE3
000		EXCLUDE_DATES
000		INCLUDE_DATES
000		ISSUE_ENUM_TYPE
000		KEY_NUMBERS
000		M008
000		M049
000		M100_EXP
000		M110_EXP
000		M111_EXP
000		M130_EXP
000		M600_EXP
000		MEDIUM
000		NISS
000		PRED_CYCLE
000		PRED_CYCLE_NM
000		REV_UID
000		SEC_CD
000		SER_TYPE
000		TITLES
000		VOL_ENUM_TYPE
000		WEEK_MONTH
000		WEEK_YEAR
//</pre>

</DIR>

<h3>Notes</h3>

<ol>
<li>The first line of the Translation Table is ignored. You may record comments here.</li>

<li>The second line specifies the <B>Label Separator</B> that separates the name of the field in the input file from its data.</li>

<li>The third line specifies the <B>Record Separator</B> the indicates the start of the next record. This could be "R  CAT" but "R  C" works for "R  COPY" too.</li>
</ol>


<h3>Field mapping</h3>

<p>
After the third line, the Translation Table consists of one line for each part of the MARC records to be created. Each line may represent an entire MARC field, a byte-range of a fixed field, or one or more subfields of a variable field. In addition, a field or subfield may have more than one <b>Segment</b>.
</p>

<p>
Each line in the Translation Table requires two columns: the description of the part of the MARC record, and the source of data for that part. There is an optional third column for <b>Attachments</b> (see below) that are to be added.
</p>

<ul>
<LI>The first column specifies the MARC fields that are to be imported. You should specify subfield codes for MARC Data Fields (010 - 999). Use a hyphen (-) to mean "indicators," as shown above for the 100 field. You may also import the MARC indicators as an Attachment to be appended to the first subfield.</li>

<li>
<b>Segments</b> are represented by following the field tag or subfield code with an ampersand and a digit. Segments must be in the Translation Table in ascending order. The Translation Table shown above uses five Segments for the Leader and two for the 008 field. (In the 008 field, MARConvert will replace the DDDDDD with the Date Entered on File.)
</li>

<LI>
The second column shows the TechLib field used to create that part of the MARC record. In the example above, the second segment of the Leader (LDR&2) is to be produced from the TechLib MTYPE field.
</li>

<li>
Alternatively, column two may contain a <b>Constant</b>. Constants must be within double quotation marks. (In the Leader, MARConvert automatically replaces the LLLLL and BBBBB with the Logical Record Length and Base Address of Data.)
</li>

<li>
Using Segments, any number of TechLib fields and Constants can be used to generate a MARC field.
</li>

<li>
The third column specifies optional <b>Attachments</b> to be appended to the TechLib data. For example, the line for 035a specifies that the OCLC control number is to be preceded by the Library of Congress code for OCLC. The "+++" in the Attachment indicates where the variable data from the TechLib OCLC field is to be inserted -- the resulting MARC 035$a subfield will look like "(OCoLC)48393549".
You can put the "+++" anywhere in an Attachment to show that the variable data should be inserted into the Attachment.
</li>

<LI>The Translation Table must end with a line whose first character is a slash.</LI>

<li>Fields in the MARC records are created in the order they are listed in the Translation Table. Normally MARC fields are in ascending numeric order.</li>
</UL>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='iinds'></a>
<h3>More on Indicators</h3>

<P>Indicators can be added either as <b>Variables</b>, <b>Constants</b>, or <b>Attachments</b>, depending on your needs. If indicators are Variables, you supply the indicators for each record in the import file; as Constants they will be the same in every record; as Attachments, they can depend on the data.</P>


<UL>
<LI>To add indicators as a <b>Variable</b>, add them as the first subfield for the field.</LI></UL>

<DIR>
<DIR>

<pre>
245-&#9;Nonfiling_characters
245a&#9;TI
</pre>

</DIR>
</DIR>


<UL>
<LI>To add indicators as a <b>Constant</b>, use - (hyphen) as the first subfield code for the field.</LI></UL>

<DIR>
<DIR>

<pre>
650-&#9;"10"
650a&#9;SUBJ
</pre>

</DIR>
</DIR>


<UL>
<LI>To add indicators as an <b>Attachment</b>, add them to the first subfield code for the field, preceding them with a - (hyphen).</LI></UL>

<DIR>
<DIR>

<pre>
100a&#9;AU&#9;"-1 +++"
</pre>

</DIR>
</DIR>



<a name='iignore'></a>
<h3>Ignoring fields</h3>

<P>Field labels that are encountered in the import file that are not found in the Translation Table are reported in the output report. This is your way to verify whether you have considered all fields in building the Translation Table. </P>

<P>However, there may be times when you wish to purposely ignore certain fields. To show that this is intentional, add a line to the Translation Table assigning the field label to the MARC tag 000, like this:</P>

<DIR>
<DIR>

<pre>
000&#9;ADD_DT
</pre>

</DIR>
</DIR>


<a name='ival'></a>
<h3>Validation</h3>

<P>MARConvert does not perform any validation on the MARC records it creates. For example, it does not check to make sure that each record has the mandatory MARC fields, or even a Leader or 008. (We plan to add such checks in future.)</P>

<P>We would be happy to meet your requirements for custom validation under contract, such as converting codes from lookup tables or adding custom diacritical conversion.</P>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='charconv'></a>
<h2>Character Conversion</h2>

<p>
MARConvert for EEE expects records in the Latin-1 character encoding, and converts them to the MARC-8 character encoding in the output file.
</p>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='special'></a>
<h2>Special Processing</h2>

<p>
MARConvert processes the data as found, with the exceptions described below. Sometimes the TechLib files lacked sufficient information to generate the precise values in the MARC records, especially indicators, 007 fields, and 008 fields; in this case we used the most common settings. An example of this is that we had to assume that all personal authors were "single surnames", resulting in a first indicator value of 1 in the 100 fields. Another example is that we used the first author encountered in the record as the main entry; other personal and corporate authors were made into added entries. If there are problems resulting from this, we would be happy to discuss detailed parsing of certain fields in order to provide more accurate results.
</p>

<p>
MARConvert parses PUBL into 260 a and b subfields.
</p>

<p>
The TechLib files sometimes contain the character sequence "return return linefeed" in the middle of a field. MARConvert replaces this sequence with a space, otherwise the remainder of the field would be lost, as "return linefeed" is used to indicate the end of a field.
</p>

<p>
COLL (collation) fields are parsed into their proper subfields, where possible.
</p>

<p>
MARConvert sets the number of non-filing characters in the 130, 730, 222, 240, 242, 243, and 245 fields, but for English articles only (A, An, and The).
</p>

<p>
We found some sound recordings in the Videos_cat.txt file. The Leader and 007 were corrected for these records.
</p>

<p>
For journals copy data, the program merges the ENUM and the HOLDINGS fields into a single 866$a.
</p>

<p>
Data in the SHELF_LOC field overrides data in the LOC field.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows folder. This file is shared with our MARCView product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<br><p><br>
<hr>
<a name=overview></a>
<h1 align=center>Client FFF</h1>
<h2>Overview</h2>
<p>This customized version of MARConvert for FFF converts MARC21 records to UNIMARC records. The new records are UNIMARC both in format and in character encoding. The input file is left unchanged.</p>
<p>In addition to the output files, MARConvert creates a report file which can be viewed or printed. After converting, the report should be looked at to see if there were any problems. The report provides a count of records converted, as well as a listing and count of problem records.</p>
<p>There is no limit to the number of records in the input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes).</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>
<p>In Interactive Mode you may convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the <b>Convert</b> button or the <b>File/Convert</b> menu item (they do exactly the same thing). (If you had been viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</p>
<p>Click the <b>OK</b> button when you are ready to start the conversion. When the run finishes, click the <b>OK</b> button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.</p>
<p>After you click the <b>Convert</b> button or menu item, a dialog box appears for you to specify the file to be converted, the new file, and the report file. When the dialog box appears, it shows the settings from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the <b>Browse</b> button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='command'></a>
<h2>Conversion: Command Mode</h2>
<p>In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
</p>
<h3>Creating the Command File</h3>
<p>To use the Command Mode, you must first create a <b>Command File</b>. This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File (included in the installation as a model):</p>
<pre>     MARConvert command file<br>     overwrite yes<br>     report "d:\marc\marconvert\FFF\marconvert.rpt"<br>     "d:\marc\marconvert\FFF\marc_file.usm"<br>     "d:\marc\marconvert\FFF\big_test.mar"</pre>
<p>Notes on the example:</p>
<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>
<li>The second line says that the program should overwrite the output file and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more tab characters or spaces to separate the word "overwrite" from the "yes" or "no".</li>
<li>Command Mode creates a single report file for all MARC files converted. The third line says where this report file should be created and what it should be called. Use one or more tab characters or spaces to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
<li>The fourth and subsequent lines list the files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
</ol>
<li>The output files will be created in the directory the input files are in, and will be given the same name as the input file but with the extension "UNI". The output files for the example above will be named "marc_file.uni" and "big_test.uni".</li>
<h3>Using Command Mode</h3>
<p>Command Mode is used as follows:</p>
<ol>
<li>Open a Windows "Command Prompt" window. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>
<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: </li>
<pre>     "c:\program files\marconvert\marconvert" command_file.txt</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command_file.txt" and exists in the current directory. If the file "command_file.txt" is not in the directory from which you are entering the prompt, you may specify its path also, as in
<pre>     "c:\program files\marconvert\marconvert" "d:\mydata\command_file.txt"</pre>
</li>
<li>After entering these two items at the prompt, press the Enter key. When MARConvert begins to run, it will open a new window and show you its progress. This window will close automatically when all files are converted.</li>
</ol>
<p class=helpnote>Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.</p>
<p class=helpnote>Tip: If you use the same Command File every time, you can create a shortcut on your Windows desktop that will start MARConvert and read the Command File.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='notes'></a>
<h2>Conversion Notes</h2>
<p>Conversion into UNIMARC is quite complex. Besides the obvious differences in tags, subfield codes, and other content designators, the philosophies of the formats are quite different. For example, in MARC21 a personal-name subject entry may have a title associated with it in the 600$t subfield. In UNIMARC, personal name data and title data must be stored in separate fields; and a third field is created which "embeds" the other two fields.</p>
<p>In addition, UNIMARC has a finer-grained structure than MARC21; for example, an author's first and last names are in two separate subfields in UNIMARC. This means the program must parse the author data in order to assign the parts to the correct subfield.</p>
<p>Finally, codes for countries, national bibliograpies, relators, and sources are different between the two formats, and have different meanings. We have delivered code-conversion tables with MARConvert, but they cannot cover every case; for example, the Library of Congress in continually defining new codes. These code-conversion tables are plain text files that you can add new codes to as required. (The report file will provide a warning if a code is missing from the tables.)</p>
<p>In order to deliver MARConvert in a reasonable time, we have programmed conversions only for the fields we expect to be encountered in your data, based on the sample file. In particular, since the sample file contained records only for monographs, fields for other materials may not be converted. If problems are discovered in the report file, please let us know.</p>
<p>Bytes 00-07 of the UNIMARC 100 field specify the date the record was generated. In UNIMARC this is a 4-digit year, but in MARC21 it is only 2 digits. We are assuming your records are primarily for current materials, so the program uses the 21st century if the year is 10 or less, the 20th century otherwise. If you prefer no century, just ask. Or the program could use the date the record was converted by MARConvert.</p>
<p>As specified, conversion from the MARC-8 encoding using in MARC21 to the UNIMARC encoding is performed only for roman characters (referred to in MARC21 documentation as Basic Latin and Extended Latin). If non-roman characters are encountered, a warning will appear in the report. MARConvert can be extended to convert non-roman characters if required. Illegal MARC21 characters are dropped completely.</p>
<p class=helpnote>When displaying the results of a conversion, please note that MARConvert can display only Latin-1 characters. This does not affect the actual conversion, but only the display. Use MARConvert's <b>Hexview</b> (by clicking the <b>Hex</b>button) to see the actual contents of the records.</p>
<p>If any of the problems described above are encountered, the record is considered bad. The bad-record count in the report tallies these.</p>
<p>The following sources were used to map the MARC21 format and characters to UNIMARC:</p>
<ul>
<li><i>UNIMARC to MARC 21 Conversion Specifications</i>, version 3.0 (August 2001). Library of Congress. http://www.loc.gov/marc/unimarctomarc21.html</li>
<li><i>OCLC-MARC Records</i>. Appendix D: MARC Format Conversions. http://www.oclc.org/support/documentation/worldcat/records/subscription/appendixd.pdf</li>
<li><i>MARC 21 Concise Format for Bibliographic Data</i>. http://www.loc.gov/marc/bibliographic/ecbdhome.html</li>
<li><i>UNIMARC Manual: Bibliographic Format 1994</i>, revised March 1, 2000. http://www.ifla.org/VI/3/p1996-1/sec-uni.htm</li>
</ul>
<div class=gotop><a href="#contents">Top</a></div>
<a name='inifile'></a>
<h2>MARCView.ini</h2>
<p>Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows directory. This file is shared with our MARCView&#8482; product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. </p>
<div class=gotop><a href="#contents">Top</a></div>



<br><p><br>
<hr>
<a name=overview></a>
<h1 align=center>Client GGG</h1>


<a name='export'><h2>Overview</h2></a>

<P>
This customized version of MARConvert for GGG converts MARC records in the MARC-8 character encoding to MARC records in the UTF-8 encoding. The original file is not affected; the converted records are put into a new file. A report file is also produced that tells whether the conversion was successful and how many records were converted.
</P>

<p>
There is no limit to the number of records that may be in a file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes).
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='running'><h2>Running <%=product%></h2></a>

<p>
We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".
</p>

<P>
To run MARConvert, follow these steps:
</p>

<ol>
<li>Open a command prompt window in Windows.</li>
<li>Enter the name of the program followed by three parameters, separated by commas, as follows:
<ol>
<li>The input file</li>
<li>The output file</li>
<li>The report file</li>
</ol>

<li>The program will open a new window to display its messages. When the program is done, press the <Enter> key to close this windows. (The same information will be found in the report file, so there is no need to copy it.)</li>
</ol>

<p>
The name of the program, as well as any or all of the filename parameters, may contain path information if necessary. The path- and filenames can contain spaces if required. The directories for output file and report file must exist before running <%=product%>. No part of the command is case-sensitive.

</p>

<p>
Examples: 
</p>

<pre>
    MARConvert marc.in, marc.out, marc.rpt

    c:\program files\MARConvert\MARConvert marc.in, marc.out, marc.rpt

    MARConvert d:\data\marc\marc.in, d:\data\utf-8\marc.out, d:\data\reports\marc.rpt
</pre>

<p>
It is usually best to name the three files with the same name but different extensions, as shown here. That way it's easier to keep track of which files are related.
</p>

<p>
If the output or report file exist, you will be asked to confirm with a "y" if you wish to overwrite them.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='charconv'><h2>Character Conversion</h2></a>

<p>
<%=product%> is able to convert any combination of MARC-8 characters, including complex cases where a letter is modified by more than one diacritic, or where a diacritic modifies a special character. A sample file from the Library of Congress, called Brkrtest.mrc, was installed with <%=product%>, and contains many unusual characters for you to test with. (Note that there are a few errors in this file, so do not worry if it appears to display the wrong symbol. For example, we noticed that what was described as the sound recording copyright symbol in one record was actually coded with the MARC-8 code for the lowercase script L.)
</p>

<p>
For the source of the translations between MARC-8 and UTF-8, we used this document from the LC MARC website: lcweb2.loc.gov/cocoon/codetables/45.html
</p>

<p>
For common combinations of diacritics and letters, <%=product%> outputs the so-called "normalized" Unicode, such as LATIN SMALL LETTER E WITH ACUTE. Rare combinations are output as the letter followed by the diacritic(s), so that i modified by a candrabindu will be output as LATIN SMALL LETTER I followed by COMBINING CANDRABINDU. Products that handle Unicode are required to display the unnormalized form identically to the normalized form. This approach saves having <%=product%> store tens of thousands of combinations, which would slow down the conversion process. The rare cases will occur mostly in romanized forms of South Asian languages. We will add the normalized form for any that are found to be problems.
</p>

<p>
Note that when you view the UTF-8 output, you will see the correct characters only if you display them using a font installed on your computer that contains the characters.
</p>

<p>
<%=product%> does not handle escapes to non-ANSEL character sets, such as Greek. If this capability is required, please contact Systems Planning. If an escape code is detected, a warning will be displayed at the end of the run.
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='errors'><h2>Error Codes</h2></a>

<p>
It is very unlikely you will ever see these error messages, except when you specify a file or directory that does not exist. However, if something does go wrong, <%=product%> will provide a message describing the problem (it may also show a numeric error code).</p>

<p>
If you accidentally try to convert a file that is already in UTF-8, or is otherwise not in MARC-8, conversion errors will almost certainly result (unless the file contains no diactrics and special characters). The program will display the "Conversion problem" message. The original file remains unaffected.
</p>

<p>
Messages can be interpreted according to this table:
</p>

<dir>
<table class=embedded border>
<tr>
<th>Message</th>
<th>Meaning</th>
</tr>

<tr>
<td nowrap>ANSEL escape code detected</td>
<td><%=product%> has found an ANSEL escape code, which means the record contains Greek, Cyrillic, or other non-roman characters). 
<%=product%> does not handle escapes to non-ANSEL character sets. If this capability is required, please contact Systems Planning.</td>
</tr>


<tr>
<td nowrap>Cannot create output file [filename]
<br>Cannot create report file [filename]
</td>
<td>The specified file could not be created. Check the spelling of the path and filename. Note that the directory where these files are to go must exist before running <%=product%>.</td>
</tr>

<tr>
<td>Cannot find [filename]</td>
<td>The input file could not be found. Check the spelling of the path and filename.</td>
</tr>

<tr>
<td>Conversion problem</td>
<td><%=product%> has found an MARC-8 code it cannot convert, or there may be an error in the input file. Is the file already in UTF-8? If you believe the file is correct, send it to Systems Planning for analysis*.</td>
</tr>

<tr>
<td>File is a UNIMARC file</td>
<td>The input file is in UNIMARC, which uses a different character set. Only MARC-8 files may be converted.</td>
</tr>

<tr>
<td>File not open</td>
<td>The input file could not be opened. Is it in use by another program?</td>
</tr>

<tr>
<td nowrap>Illegal fieldlength or offset, record x</td>
<td>Record x in the input file has been corrupted. Check that the file you are using is really a MARC file. If it is, then the file has been severely damaged.</td>
</tr>

<tr>
<td>Record x is invalid (code y)</td>
<td>Record x has severe validation problems, such as invalid record length, missing record terminator, or garbled leader or directory.</td>
</tr>

<tr>
<td>Status code 2</td>
<td>Record has minor validation problems, such as a non-numeric tag.</td>
</tr>

<tr>
<td>Status code -1</td>
<td>Miscellaneous error. Send the file to Systems Planning for analysis*.</td>
</tr>

<tr>
<td>Status code -8</td>
<td>The computer is out of memory.</td>
</tr>

<tr>
<td>Status code -11</td>
<td>The input file may be blocked (that is, it is not a true MARC file).</td>
</tr>

<tr>
<td>Unexpected end of file</td>
<td>The last record is incomplete. Check for file corruption.</td>
</tr>

</table>
</dir>

<p>
* To send us a file, please zip it using PKZip or Winzip and attach it to an email. If the zipped file is over 3MB, please notify us first -- we may have to FTP it. Please tell us what error message was received, and what record caused the error.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>




<br><br>

<hr>


<a name=overview></a>
<h1 align=center>Client HHH</h1>
<h2>Overview</h2>

<p>This customized version of MARConvert for HHH converts MARC21 records from UTF-8 to MARC-8. The input files are left unchanged.</p>

<p>In addition to the output files, MARConvert creates a report file which can be viewed or printed. After converting, the report should be looked at to see if there were any problems. The report provides a count of records converted, as well as a listing and count of problem records.</p>

<p>There is no limit to the number of records in the input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes).</p>


	<div class=gotop><a href="#contents">Top</a></div>


<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>In Interactive Mode you may convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the <b>Convert</b> button or the <b>File/Convert</b> menu item (they do exactly the same thing). (If you had been viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</p>

<p>After you click the <b>Convert</b> button or menu item, a dialog box appears for you to specify the file to be converted, the new file, and the report file. When the dialog box appears, it shows the settings from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the <b>Browse</b> button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</p>

<p>Click the <b>OK</b> button when you are ready to start the conversion. When the run finishes, click the <b>OK</b> button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.</p>


	<div class=gotop><a href="#contents">Top</a></div>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
</p>


<h3>Creating the Command File</h3>

<p>To use the Command Mode, you must first create a <b>Command File</b>. This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File (included in the installation as a model):</p>

<pre>
     MARConvert command file
     overwrite       yes
     report          "d:\marc\marconvert\HHH\MARConvert.rpt"
     "d:\MARC\MARConvert\HHH\marc_record_in_utf8.mrc"
     "d:\MARC\MARConvert\HHH\lc_utf8.mrc"
     "d:\MARC\MARConvert\HHH\part022.utf"
</pre>

<p>Notes on the example:</p>

<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>

<li>The second line says that the program should overwrite the output files and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Of course, this may prevent the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more tab characters or spaces to separate the word "overwrite" from the "yes" or "no".</li>

<li>Command Mode creates a single report file for all MARC files converted. The third line says where this report file should be created and what it should be called. Use one or more tab characters or spaces to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>

<li>The fourth and subsequent lines list the files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
</ol>

<p>The output files will be created in the directory the input files are in, and will be given the same name as the input file but with the extension "M8". (You may change the default extension by editing MARCView.ini in your Windows directory.) The output files for the example above will be named "marc_record_in_utf8.M8", lc_utf8.M8", and part022.M8".</p>


<h3>Using Command Mode</h3>

<p>Command Mode is used as follows:</p>

<ol>
<li>Open a Windows "Command Prompt" window. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>

<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: </li>

<pre>
     "C:\Program Files\Systems Planning\MARConvert\MARConvert" command_file.txt
</pre>

Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br><br>
This example also assumes that the Command File was named "command_file.txt" and exists in the current directory. If the file "command_file.txt" is not in the directory from which you are entering the prompt, you may specify its path also, as in
<pre>
     "C:\Program Files\Systems Planning\MARConvert\MARConvert" "D:\mydata\command_file.txt"
</pre>

</li>

<li>After entering these two items at the prompt, press the Enter key. When MARConvert begins to run, it will open a new window and show you its progress. When prompted, press the enter key again to close this window.</li>
</ol>

<p class=helpnote>Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.</p>

<p class=helpnote>Tip: If you use the same Command File every time, you can create a shortcut on your Windows desktop that will start MARConvert and read the Command File.</p>


	<div class=gotop><a href="#contents">Top</a></div>


<a name='notes'></a>
<a name='charconv'></a><h2>Character Conversion</h2>

<p>MARConvert for HHH converts records in the UTF-8 encoding to MARC-8 encoding. MARConvert can convert the entire 16,000+ symbols in the MARC-8 repertoire. In addition, numerous precomposed symbols are handled.</p>

<p>Character conversion is performed using tables of equivalent encodings. The following sources were used for these tables:</p>

<ul>
<li><i>Unicode Character Database</i>, version 4.1.0 (www.unicode.org/ucd/).</li>

<li><i>MARC-8 to Unicode XML Mapping File</i>, last revised March 2005 (www.loc.gov/marc/specifications/codetables.xml).</li>

<li>For conversion of precomposed characters, Systems Planning's proprietary symbol conversion table.</li>

<li>For normalization of certain MARC-8 characters, Jack Cain's <i>Unicode to MARC8 Conversion: Exceptions to Decomposition of Precomposed Characters</i> (Dec. 13, 2005), in <i>MARC Proposal No. 2006-04.</i> 
(www.loc.gov/marc/marbi/2006/2006-04.html). MARConvert does not implement the last two recommendations in Basic Arabic and all of the Cyrillic items. These are actually single characters and the Unicode database does not offer any decomposition for them.</li>
</ul>

<p>MARConvert uses precomposed characters whenever available.</p>

<p>Lowercase alpha, beta, and gamma in UTF-8 are converted to MARC-8 Basic Greek rather than to Greek Symbols. Since there is no way to tell from the data which is wanted, this prevents repeated escaping between these two character sets in a field. We can change this default if required.</p>


<h3>Conversion problems</h3>

<p class=helpnote>When displaying the results of a conversion, please note that MARConvert can display only Latin-1 characters. Thus it is not a true representation of the results of the conversion. Use MARConvert's <b>Hexview</b> (by clicking the <b>Hex</b> button) to see the actual contents of the records.<br><br>Also note that the display is affected by the <b>Character Set of Records</b> in <a href=#settings>Settings</a>.</p>

<p>When a character is encountered that cannot be found in the conversion tables, MARConvert inserts the Numeric Character Reference of the character (such as &amp;#xA1B;). In addition, a warning message is logged to the report file.</p>

<p>MARConvert cannot continue if it encounters a malformed MARC record, such as one where there is no Record Terminator. If this occurs frequently, ask us to have MARConvert skip such records and continue after them.</p>

<p>If a combining diacritic is not associated with a base character, MARConvert attempts to output the diacritic anyway. However, if the diacritic is at the end of a field or subfield, the diacritic is removed and a warning is logged.</p>


	<div class=gotop><a href="#contents">Top</a></div>


<a name="errors"></a>
<h2>Error Messages</h2>

<p>Character-conversion messages are prefixed by "Problem" if the data could not be converted, or by "Warning" for issues that probably do not affect the data.</p>

<table class=embedded border>
<tr><th>Message</th><th>Discussion</th></tr>

<tr><td>Internal error, contact Systems Planning</td><td>The program cannot run. Contact Systems Planning</td></tr>

<tr><td>Problem: UTF-8 zzz not found in conversion tables</td><td>The specified UTF-8 character could not be found in the conversion tables</td></tr>

<tr><td>Problem: Record longer than 99,999 characters</td><td>Addition of the specified field would have caused the output record to be longer than 99,999 bytes, which is the MARC maximum</td></tr>

<tr><td>Problem: Too many modifiers for zzz, contact Systems Planning</td><td>More than 10 modifiers were found for the specified base character. If this is encountered, contact Systems Planning to have this limit raised.</td></tr>

<tr><td>Problem: UTF-8 zzz not found in conversion tables</td><td>The specified UTF-8 character could not be found in the conversion tables</td></tr>

<tr><td>Status code z</td><td>A generic warning that a record could not be processed correctly. This message will probably never be seen. If it is, contact Systems Planning.</td></tr>

<tr><td>Warning: Converting xxx to yyy</td><td>In converting from UTF-8 to MARC-8, this message means that the symbol xxx could not be found in the LC code tables, and the Systems Planning symbol table was used instead. This can occur when LC changes the mapping of a symbol. For example, the mapping of MARC-8 ALIF (AE) was recently changed from Unicode 02BE to Unicode 02BC. Since older UTF-8 records may contain the older 02BE, they are converted to MARC-8 AE for convenience and this warning isissued.</td></tr>

<tr><td>Warning: Program removed diacritics modifying yyy</td><td>The MARC-8 character yyy, either a Field Terminator or a Subfield Delimiter, is not allowed to be modified by diacritics.</td></tr>
</table>

<br>
<div class=gotop><a href="#contents">Top</a></div>
<a name='inifile'></a>
<h2>MARCView.ini</h2>
<p>Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows directory. This file is shared with our MARCView&#8482; product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. </p>
<div class=gotop><a href="#contents">Top</a></div>


<br><p><br>
<hr>
<a name="convert"></a>
<h1 align=center>Client JJJ</h1>

<a name='export'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for JJJ converts MARC records to Microtext, suitable for uploading to CHIN.
</P>


<p>
The output is a file in which each field is preceded by its field name and an equals sign. Lines are 80 characters long or less; longer fields are continued on the next line without a space. (The CHIN loader inserts the space.)

<DIR>
<DIR>

<P><b>Example of output</b></P>

<pre>
RST=c
RT=a
AVL=50.00F
COL=109 p. ill. (some col.) 24 cm.
...
</pre>

</DIR>
</DIR>


<P>
The export is controlled by a text file called a <B>Translation Table</B>. MARConvert uses the information in this file to convert each field from MARC format to the text format. We have supplied a Translation Table for the initial data, but you may need to modify it to meet changing circumstances. Of course, the Translation Table controls only some functions; complex changes must be made in the program code by Systems Planning.
</P>

<P>The export process results in three files:</P>


<UL>
<LI>A file of exported records in text format</LI>
<LI>A file of records which could not be exported. These represent data problems, such as malformed MARC records, missing mandatory fields, etc.</LI>
<LI>A report file which can be viewed or printed. Look at the report after exporting to see if there were any problems.</LI></UL>

<P>
The MARC records must be in the MARC-8 character set, and are converted to Latin-1 as required by CHIN. (During development we tried using Latin-1 records from Voyager in order to avoid conversion, but there were problems with some of the characters.)
</P>

<P>
There is no limit to the number of records in the input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes). Your original MARC file is not affected in any way by exporting it.
</P>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='edialog'></a>
<h2>Export dialog</h2>

<P>To export, click on the Export command in the Convert menu. (If you are viewing a file in MARConvert, it will close, since certain resources are needed by the Export process. Simply reopen the file when the Export is finished.)</P>

<P>Begin to fill in the dialog box by telling MARConvert what file to convert. The easiest way is to click the Browse button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names you wish. All names and parameters are saved for you for the next time you use Export.</P>

<P>The "Build" button creates the skeleton of a Translation Table for you, using the filename you specify in the box labelled "Translation file". This skeleton contains a line for every MARC field and subfield found in your file; that way you do not have to guess at what fields exist.</P>

<P>You may also specify the number of records to export. This can be helpful when you are testing changes to the Translation Table. Use 0 (zero) when you want to export the entire file. There is no limit to the number of records you can export from a MARC file; the "Maximum records" setting in the <a href="#settings">Settings dialog</a> does not apply to Export.</P>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='etable'></a>
<h2>Translation Table</h2>

<P>
The Translation Table can be edited with Notepad. The Table consists of two columns separated with spaces and/or tabs. You may use as many spaces and tabs as you like to line up the columns for legibility. If you have a field label that includes a space, put quotation marks around it. We recommend that all labels have quotation marks around them for clarity.
</P>

<DIR>

<P><b>Translation Table as delivered</b></P>

<pre>
JJJ Microtext Export Translation Table
LabelSep	"="
RecordSep	"/N"
SubfieldSep	" "
LDR/05		"RST"
LDR/06		"RT"
-		"AATALN"
520a		"PABS"
-		"ABSPR"
100u		"AFFL"
-		"AFFLA"
-		"AISS"
-		"AN"
-		"ANC"
-		"ATSRCE"
-		"AUAD"
-		"AUCR"
-		"AUCTY"
-		"AUPC"
-		"AUST"
245h		"AVFORM"
020c		"AVL"
265a		"AVL"
-		"AVOL"
-		"CHEKSUM"
-		"CLAS"
-		"COAAD"
-		"COACRY"
-		"COACTY"
-		"COAPC"
-		"COAST"
030a		"COD"
072ax		"CODESUB"
300abce		"COL"
-		"CR"
-		"DOBCR"
535a		"DOCLOC"
851a		"DOCLOC"
-		"DT"
260c		"DTP"
250a		"EDTN"
0359		"JJJACCN"
-		"JJJCN"
-		"JJJLN"
090a		"JJJSHLF"
020a		"ISBN"
-		"ISS"
-		"ISSL"
022a		"ISSN"
041b		"LAS"
041a		"LAT"
-		"LEV"
-		"LITTYPE"
-		"LOCALNO"
-		"MTGCRY"
-		"MTGCTY"
-		"MTGD"
500a		"NOTE"
710ab		"ORG"
-		"ORIG"
-		"ORIGDOB"
-		"ORIGDOC"
-		"ORIGNO"
-		"PAG"
-		"PAT"
-		"PATPL"
100a		"PAUT"
700a		"PAUT"
110a		"PAUTCR"
111acd		"PAUTCR"
710a		"PAUTCR"
-		"PAUTCRA"
-		"PAUTED"
-		"PAUTILL"
-		"PAUTOTH"
-		"PAUTRAN"
-		"PCAST"
-		"PCNOTE"
508a		"PCRED"
-		"PMTG"
260b		"PPUB"
-		"PPUBAD"
260a		"PPUBCTY"
-		"PPUBCRY"
-		"PPUBPC"
-		"PPUBST"
650a		"PSUBKY"
440av		"PTISER"
490av		"PTISER"
830av		"PTISER"
245ab		"PTISRC"
130ab		"PTISRC"
240ab		"PTISRC"
247ab		"PTISRC"
730ab		"PTISRC"
740ab		"PTISRC"
-		"PTISRTR"
242ab		"PTITRAN"
246a		"PTIVAR"
-		"PUBREM"
504a		"REFBIB"
088a		"REP"
-		"SER"
-		"SERFQ"
555a		"SPECBIB"
610a		"SUBCORP"
523a		"SUBDATE"
-		"SUBGEA"
651a		"SUBGEOE"
-		"SUBLBL"
600a		"SUBPER"
502a		"UNI"
502a		"UNIDG"
-		"UNILOC"
-		"VOL"
-		"VOLL"
-		"URL"
001		-
005		-
006		-
007		-
008		-
010		-
015		-
016		-
017		-
024		-
025		-
027		-
028		-
032		-
034		-
037		-
040		-
042		-
043		-
045		-
050		-
051		-
052		-
055		-
060		-
066		-
070		-
074		-
080		-
082		-
086		-
210		-
212		-
222		-
255		-
256		-
263		-
310		-
321		-
351		-
362		-
501		-
505		-
506		-
510		-
511		-
515		-
516		-
518		-
521		-
525		-
530		-
533		-
534		-
536		-
538		-
540		-
545		-
546		-
550		-
556		-
580		-
586		-
590		-
611		-
630		-
653		-
655		-
690		-
691		-
711		-
752		-
753		-
760		-
770		-
772		-
773		-
775		-
776		-
780		-
785		-
787		-
799		-
800		-
810		-
811		-
850		-
852		-
853		-
856		-
863		-
866		-
867		-
868		-
880		-
886		-
901		-
902		-
905		-
906		-
907		-
908		-
909		-
911		-
921		-
922		-
935		-
936		-
941		-
942		-
947		-
948		-
950		-
955		-
960		-
981		-
995		-
998		-
//
</pre>

</DIR>

<h3>Separators</h3>

<ol>
<li>The first line of the Translation Table is ignored. You may record comments here.</li>

<li>The second line specifies the <B>Label Separator</B> used between the field label and the field data in the export file (this is currently "=" for Microtext). Use a capital T to indicate a tab, and a capital N to indicate a new line (carriage return); other characters can be combined with these as the example shows.</li>

<li>The third line specifies the <B>Record Separator</B> used after each record (currently "/N" for Microtext); the only condition is that the Record Separator be on a line by itself in the export file. Use a capital T to indicate a tab, and a capital N to indicate a new line (carriage return); other characters can be combined with these as the example shows.</li>

<li>
The fourth line specifies the <B>Subfield Separator</B> used between subfields in the MARC record that are being combined into a single Microtext field.
</li>
</ol>

<p>
Put the desired values for the Separators into the second column. If your desired separator includes spaces, it will be a lot easier to read the Translation Table if you enclose the separator in double quotes -- but if your separator includes a backslash (also called a back solidus), the backslash cannot immediately precede a double quote.
</P>

<p>
A carriage return and line feed are added after each field in a tagged-text output file. A calculated LITTYPE and LEV are added to each record. The fields "ORIG=JJJ" and "ABSPR=No" are added to each record.
</P>

<P>The separators in the example Translation Table above will create output files like this:</P>

<DIR>

<P><b>Example of Tagged-Text Output</b></P>

<pre>
RST=c
RT=a
AVL=50.00F
COL=109 p. : ill. (some col.) ; 24 cm.
DTP=c1978.
JJJACCN=21211272-B
JJJSHLF=N2010
ISBN=2711800822
NOTE="Exposition ... réalisée par la Réunion des musées nationaux.
ORG=Musée du Louvre.; Réunion des musées nationaux (France)
PAUTCR=Musée du Louvre.; Réunion des musées nationaux (France)
PPUB=Ministère de la culture et de l'environnement, Éditions de la Réunion des
musées nationaux
PPUBCTY=[Paris]
PSUBKY=Art; Decedents' estates
PTISRC=Défense du patrimoine national : oeuvres acceptées par l'État en
paiement de droits de succession, 1972-1977 : Musée du Louvre, Galerie Mollien,
23 février-24 avril 1978.
REFBIB=Includes bibliographies.
LITTYPE=Monograph
LEV=Monographic
ORIG=JJJ
ABSPR=No
/
</pre>


</DIR>


<h3>Field mapping</h3>

<p>
After the fourth line, the Translation Table consists of one line for each part of the MARC record you wish to output. A "part" of a MARC record can be an entire field, a byte-range of a fixed field, or one or more subfields of a variable field. Each of these lines requires two columns, the MARC tag and the label for the tagged-text output.
</p>

<ul>
<li>You may indicate "no mapping" by using a hyphen in either column. This is a good way to document your decisions.</li>

<li>If several MARC fields are to be merged into the same Microtext field, they must be listed together in the Translation Table. Examples are "PTISER" and "PTISRC" in the Translation Table shown.</li>

<LI>The first column specifies the MARC fields that are to be exported. You may specify one or more subfields to be exported; if there are no subfield codes, the entire field is output. Use a hyphen (-) to mean "indicators," as shown above for the 050 and 650 fields. You may export the MARC indicators either as a separate field or combined with one or more subfields, or not at all.</li>

<li>For fixed-length elements like the Leader and 008, you may export the entire field, or you may specify the bytes to export.</LI>

<LI>The second column shows the "field label" to use in the output file. Leave this column blank to mean "do not export this field," as shown above for the 300 field; however, performance for large files will improve if you remove lines for tags you do not wish to export.</LI>
</UL>


<P>Notes on the Example:</P>

<UL>
<LI>Byte 5 of the Leader is exported as "Record Status", and byte 6 as "Record Type". Bytes specified in the Leader, 008, and other fixed fields follow the MARC convention of beginning with byte 0. You may export the entire Leader, 008, and similar fields by simply not specifying a byte-range.</LI>

<LI>The Translation Table must end with a line whose first character is a slash.</LI>

<li>Fields in the MARC records are exported in the order they are listed in the Translation Table.</li>
</UL>

<p class=helpnote>
If you change the label SUBKY or SUBGEE, the program will no longer be able to look for duplicate subject keywords.
<br>
<br>
If you change the labels AVFORM or COL, the program will no longer handle AV materials correctly.
<br>
<br>
The line to output TISRC must map to the 245 with no subfield codes specified; this is to handle parallel titles correctly.
<br>
<br>
If you change the labels AUT, AUTCR, CRED, DTP, LAT, TISRC or TISRTR, the program will no longer be able to validate records correctly.
<br>
<br>
If you change the mapping of 111 and 711, meetings will not longer be parsed correctly.
<br>
<br>
If you wish to change the labels ABSPR, DOCLOC, LITTYPE, LEV, ORIG, UNI, or UNIDG contact Systems Planning.
</p>




	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='langtable'></a>
<h2>Language Table</h2>

<P>
The Language Table can be edited with Notepad. The Table consists of two columns separated with spaces and/or tabs. You may use as many spaces and tabs as you like to line up the columns for legibility.
</P>

<p>
The Language Table must be called "language.txt" and be in the same folder as the Translation Table.
</p>

<p>The first line of the Language Table is ignored; you may record comments here. Succeeding lines consist of a MARC language code followed by the name of the language to convert the code to.
</p>

<p>
MARC language codes in the 041 may consist of multiple codes string together, such as "engfre". This is converted in the output to "English; French".
</p>

<DIR>

<P><b>Language Table as delivered</b></P>

<pre>
JJJ Microtext Export Language Codes
ara	Arabic
baq	Basque
cai	Central American Indian (Other)
cat	Catalan
chi	Chinese
cze	Czech
dan	Danish
dut	Dutch
eng	English
fin	Finnish
fre	French
gag	Galician
gal	Oromo
ger	German
glg	Galician
grc	Greek
gre	Greek
her	Herero
hin	Hindi
hun	Hungarian
ita	Italian
jpn	Japanese
lat	Latin
lav	Latvian
lit	Lithuanian
mac	Macedonian
mul	Multiple
nah	Nahuatl
nor	Norwegian
pol	Polish
por	Portuguese
rum	Romanian
rus	Russian
scc	Serbian
scr	Serbo-Croatian
slo	Slovak
slv	Slovenian
spa	Spanish
swe	Swedish
tha	Thai
tur	Turkish
und	Undetermined
wel	Welsh
zap	Zapotec
</pre>

</dir>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='charconv'></a>
<h2>Character Conversion</h2>

<p>
MARConvert for JJJ expects records in the MARC-8 character set as input, and produces output in the Latin-1 character set.
</p>

<p>
Because not all MARC-8 symbols have Latin-1 equivalents, some symbols are converted to the closest representation, or not converted at all. The following table shows the conversion performed. MARC-8 symbols with no Latin-1 symbol are removed from the record and the space closed up.
</p>


<table class=embedded border>
<tr>
<th>Name</th>
<th>MARC-8 symbol</th>
<th>Latin-1 symbol</th>
</tr>

<tr><td>POLISH L UPPERCASE</td><td>A1</td><td>L</td></tr>
<tr><td>SCANDINAVIAN O UPPERCASE</td><td>A2</td><td>Ø</td></tr>
<tr><td>D WITH CROSSBAR UPPERCASE</td><td>A3</td><td>Ð</td></tr>
<tr><td>ICELANDIC THORN UPPERCASE</td><td>A4</td><td>Þ</td></tr>
<tr><td>DIGRAPH AE UPPERCASE</td><td>A5</td><td>Æ</td></tr>
<tr><td>DIGRAPH OE UPPERCASE</td><td>A6</td><td>Œ</td></tr>
<tr><td>SOFT SIGN (PRIME), MIAGKIY ZNAK</td><td>A7</td><td>´</td></tr>
<tr><td>MIDDLE DOT</td><td>A8</td><td>·</td></tr>
<tr><td>MUSICAL FLAT</td><td>A9</td><td>flat</td></tr>
<tr><td>PATENT MARK</td><td>AA</td><td>®</td></tr>
<tr><td>PLUS OR MINUS</td><td>AB</td><td>±</td></tr>
<tr><td>O-HOOK UPPERCASE</td><td>AC</td><td>O</td></tr>
<tr><td>U-HOOK UPPERCASE</td><td>AD</td><td>U</td></tr>
<tr><td>ALIF</td><td>AE</td><td>’</td></tr>
<tr><td>ALPHA</td><td>AF</td><td>a</td></tr>
<tr><td>AYN</td><td>B0</td><td>‘</td></tr>
<tr><td>POLISH L LOWERCASE</td><td>B1</td><td>l</td></tr>
<tr><td>SCANDINAVIAN O LOWERCASE</td><td>B2</td><td>ø</td></tr>
<tr><td>D WITH CROSSBAR LOWERCASE</td><td>B3</td><td>d</td></tr>
<tr><td>ICELANDIC THORN LOWERCASE</td><td>B4</td><td>þ</td></tr>
<tr><td>DIGRAPH AE LOWERCASE</td><td>B5</td><td>æ</td></tr>
<tr><td>DIGRAPH OE LOWERCASE</td><td>B6</td><td>œ</td></tr>
<tr><td>HARD SIGN, TVERDYI ZNAK</td><td>B7</td><td>´</td></tr>
<tr><td>TURKISH I LOWERCASE</td><td>B8</td><td>i</td></tr>
<tr><td>BRITISH POUND</td><td>B9</td><td>£</td></tr>
<tr><td>ETH</td><td>BA</td><td>ð</td></tr>
<tr><td>DAGGER</td><td>BB</td><td>†</td></tr>
<tr><td>O-HOOK LOWERCASE</td><td>BC</td><td>o</td></tr>
<tr><td>U-HOOK LOWERCASE</td><td>BD</td><td>u</td></tr>
<tr><td>DEGREES</td><td>C0</td><td>°</td></tr>
<tr><td>SCRIPT L</td><td>C1</td><td>l</td></tr>
<tr><td>PHONO COPYRIGHT</td><td>C2</td><td>p</td></tr>
<tr><td>COPYRIGHT</td><td>C3</td><td>©</td></tr>
<tr><td>MUSICAL SHARP</td><td>C4</td><td>#</td></tr>
<tr><td>INVERTED QUESTION</td><td>C5</td><td>¿</td></tr>
<tr><td>INVERTED EXCLAMATION</td><td>C6</td><td>¡</td></tr>
<tr><td>PSEUDO QUESTION</td><td>E0</td><td>&nbsp;</td></tr>
<tr><td>GRAVE</td><td>E1</td><td>&nbsp;</td></tr>
<tr><td>A WITH GRAVE</td><td>E1 41</td><td>À</td></tr>
<tr><td>E WITH GRAVE</td><td>E1 45</td><td>È</td></tr>
<tr><td>I WITH GRAVE</td><td>E1 49</td><td>Ì</td></tr>
<tr><td>O WITH GRAVE</td><td>E1 4F</td><td>Ò</td></tr>
<tr><td>U WITH GRAVE</td><td>E1 55</td><td>Ù</td></tr>
<tr><td>a WITH GRAVE</td><td>E1 61</td><td>à</td></tr>
<tr><td>e WITH GRAVE</td><td>E1 65</td><td>è</td></tr>
<tr><td>i WITH GRAVE</td><td>E1 69</td><td>ì</td></tr>
<tr><td>o WITH GRAVE</td><td>E1 6F</td><td>ò</td></tr>
<tr><td>u WITH GRAVE</td><td>E1 75</td><td>ù</td></tr>
<tr><td>ACUTE</td><td>E2</td><td>&nbsp;</td></tr>
<tr><td>A WITH ACUTE</td><td>E2 41</td><td>Á</td></tr>
<tr><td>E WITH ACUTE</td><td>E2 45</td><td>É</td></tr>
<tr><td>I WITH ACUTE</td><td>E2 49</td><td>Í</td></tr>
<tr><td>O WITH ACUTE</td><td>E2 4F</td><td>Ó</td></tr>
<tr><td>U WITH ACUTE</td><td>E2 55</td><td>Ú</td></tr>
<tr><td>Y WITH ACUTE</td><td>E2 59</td><td>Ý</td></tr>
<tr><td>a WITH ACUTE</td><td>E2 61</td><td>á</td></tr>
<tr><td>e WITH ACUTE</td><td>E2 65</td><td>é</td></tr>
<tr><td>i WITH ACUTE</td><td>E2 69</td><td>í</td></tr>
<tr><td>o WITH ACUTE</td><td>E2 6F</td><td>ó</td></tr>
<tr><td>u WITH ACUTE</td><td>E2 75</td><td>ú</td></tr>
<tr><td>y WITH ACUTE</td><td>E2 79</td><td>ý</td></tr>
<tr><td>CIRCUMFLEX</td><td>E3</td><td>&nbsp;</td></tr>
<tr><td>A WITH CIRCUMFLEX</td><td>E3 41</td><td>Â</td></tr>
<tr><td>E WITH CIRCUMFLEX</td><td>E3 45</td><td>Ê</td></tr>
<tr><td>I WITH CIRCUMFLEX</td><td>E3 49</td><td>Î</td></tr>
<tr><td>O WITH CIRCUMFLEX</td><td>E3 4F</td><td>Ô</td></tr>
<tr><td>U WITH CIRCUMFLEX</td><td>E3 55</td><td>Û</td></tr>
<tr><td>a WITH CIRCUMFLEX</td><td>E3 61</td><td>â</td></tr>
<tr><td>e WITH CIRCUMFLEX</td><td>E3 65</td><td>ê</td></tr>
<tr><td>i WITH CIRCUMFLEX</td><td>E3 69</td><td>î</td></tr>
<tr><td>o WITH CIRCUMFLEX</td><td>E3 6F</td><td>ô</td></tr>
<tr><td>u WITH CIRCUMFLEX</td><td>E3 75</td><td>û</td></tr>
<tr><td>TILDE</td><td>E4</td><td>&nbsp;</td></tr>
<tr><td>A WITH TILDE</td><td>E4 41</td><td>Ã</td></tr>
<tr><td>N WITH TILDE</td><td>E4 4E</td><td>Ñ</td></tr>
<tr><td>O WITH TILDE</td><td>E4 4F</td><td>Õ</td></tr>
<tr><td>a WITH TILDE</td><td>E4 61</td><td>ã</td></tr>
<tr><td>n WITH TILDE</td><td>E4 6E</td><td>ñ</td></tr>
<tr><td>o WITH TILDE</td><td>E4 6F</td><td>õ</td></tr>
<tr><td>MACRON</td><td>E5</td><td>&nbsp;</td></tr>
<tr><td>BREVE</td><td>E6</td><td>&nbsp;</td></tr>
<tr><td>SUPERIOR DOT</td><td>E7</td><td>&nbsp;</td></tr>
<tr><td>UMLAUT (DIERESIS)</td><td>E8</td><td>&nbsp;</td></tr>
<tr><td>A WITH UMLAUT</td><td>E8 41</td><td>Ä</td></tr>
<tr><td>E WITH UMLAUT</td><td>E8 45</td><td>Ë</td></tr>
<tr><td>I WITH UMLAUT</td><td>E8 49</td><td>Ï</td></tr>
<tr><td>O WITH UMLAUT</td><td>E8 4F</td><td>Ö</td></tr>
<tr><td>U WITH UMLAUT</td><td>E8 55</td><td>Ü</td></tr>
<tr><td>a WITH UMLAUT</td><td>E8 61</td><td>ä</td></tr>
<tr><td>e WITH UMLAUT</td><td>E8 65</td><td>ë</td></tr>
<tr><td>i WITH UMLAUT</td><td>E8 69</td><td>ï</td></tr>
<tr><td>o WITH UMLAUT</td><td>E8 6F</td><td>ö</td></tr>
<tr><td>u WITH UMLAUT</td><td>E8 75</td><td>ü</td></tr>
<tr><td>y WITH UMLAUT</td><td>E8 79</td><td>ÿ</td></tr>
<tr><td>HACEK</td><td>E9</td><td>&nbsp;</td></tr>
<tr><td>CIRCLE ABOVE (ANGSTROM)</td><td>EA</td><td>&nbsp;</td></tr>
<tr><td>A WITH CIRCLE ABOVE</td><td>EA 41</td><td>Å</td></tr>
<tr><td>a WITH CIRCLE ABOVE</td><td>EA 61</td><td>å</td></tr>
<tr><td>LIGATURE, FIRST HALF</td><td>EB</td><td>&nbsp;</td></tr>
<tr><td>LIGATURE, SECOND HALF</td><td>EC</td><td>&nbsp;</td></tr>
<tr><td>HIGH COMMA OFF CENTER</td><td>ED</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE ACUTE</td><td>EE</td><td>&nbsp;</td></tr>
<tr><td>CANDRABINDU</td><td>EF</td><td>&nbsp;</td></tr>
<tr><td>CEDILLA</td><td>F0</td><td>&nbsp;</td></tr>
<tr><td>C WITH CEDILLA</td><td>F0 43</td><td>Ç</td></tr>
<tr><td>c WITH CEDILLA</td><td>F0 63</td><td>ç</td></tr>
<tr><td>RIGHT HOOK</td><td>F1</td><td>&nbsp;</td></tr>
<tr><td>DOT BELOW CHARACTER</td><td>F2</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE DOT BELOW CHARACTER</td><td>F3</td><td>&nbsp;</td></tr>
<tr><td>CIRCLE BELOW CHARACTER</td><td>F4</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE UNDERSCORE</td><td>F5</td><td>&nbsp;</td></tr>
<tr><td>UNDERSCORE</td><td>F6</td><td>&nbsp;</td></tr>
<tr><td>LEFT HOOK/TAIL</td><td>F7</td><td>&nbsp;</td></tr>
<tr><td>RIGHT CEDILLA</td><td>F8</td><td>&nbsp;</td></tr>
<tr><td>UPADHMANIYA</td><td>F9</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE TILDE FIRST HALF</td><td>FA</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE TILDE SECOND HALF</td><td>FB</td><td>&nbsp;</td></tr>
<tr><td>HIGH COMMA CENTERED</td><td>FE</td><td>&nbsp;</td></tr>
</table>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='validation'></a>
<h2>Validation</h2>

<p>
MARConvert performs certain checks to ensure that records meet the minimum standards for BCIN. If a record does not pass the validation, it is put into the bad-record file and a message is written to the report.
</p>

<p>
A record is invalid if any of the following conditions are met:
<ul>
<li>No DTP, LAT, or TISRC</li>
<li>If there is no AUT or AUTCR, copy the ORG into AUTCR. If there is no ORG, copy the PUB into AUTCR. If there is no PUB, the record is invalid.</li>
<li>If the first LAT is "English" and there is no TISRTR</li>
<li>An illegal language code</li>
<li>A field not in the Translation Table</li>
<li>DTP cannot be converted to BCIN form</li>
</ul>
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='errors'></a>
<h2>Error Codes</h2>

<p>
If a record cannot be read or has data problems, a message is put into the report file. Error codes are as follows:
</p>

<table class=embedded border>
<tr>
<th>Code</th>
<th>Meaning</th>
</tr>

<tr>
<td>3</td>
<td>Record has severe validation problems, such as invalid record length, missing record terminator, or garbled leader or directory.</td>
</tr>

<tr>
<td>2</td>
<td>Record has minor validation problems, such as a non-numeric tag.</td>
</tr>

<tr>
<td>-1</td>
<td>Unknown error.</td>
</tr>

<tr>
<td>-4</td>
<td>End-of-file before end of record. Check for file corruption.</td>
</tr>

<tr>
<td>-5</td>
<td>Not a MARC record, or severely damaged.</td>
</tr>

<tr>
<td>-8</td>
<td>Out of memory.</td>
</tr>

<tr>
<td>-9</td>
<td>Program problem. Notify Systems Planning.</td>
</tr>

<tr>
<td>-10</td>
<td>Problem converting characters.</td>
</tr>

<tr>
<td>-11</td>
<td>May be a blocked file.</td>
</tr>

</table>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows folder. This file is shared with our MARCView product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<br><p><br>
<hr>
<a name="convert"></a>
<h1 align=center>Client KKK</h1>

<a name='export'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for KKK converts MARC records to three text files, suitable for importing into The Museum System (TMS).
</P>


<p>
The output files are tab-delimited ASCII, each record on a separate line. Output files are named for the import file in this way: if the import files is called Testdata.mrc, the output files are called Testdata.objects.txt, Testdata.objtitles.txt, and Testdata.altnums.txt.
</p>

<P>In addition to the three output files, the export process creates a report file which can be viewed or printed. Look at the report after exporting to see if there were any problems. The report file for the example above would normally be called Testdata.rpt, but you can override this in Interactive Mode (see below).
</p>

<P>
The export is controlled by a text file called a <B>Translation Table</B>. MARConvert uses the information in this file to convert each field from MARC format to the text format. We have supplied a Translation Table for the initial data, but you may need to modify it to meet changing circumstances. However, the Translation Table can control only routine functions; complex changes must be made in the program code by Systems Planning.
</P>

<P>
The MARC records must be in the MARC-8 character set, and are converted to Latin-1.
</P>

<P>
There is no limit to the number of records in the input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes). Your original MARC file is not affected in any way by exporting it.
</P>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>
In Interactive Mode you will may convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the "Convert" button or the File/Convert menu item (they do exactly the same thing). (If you are viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</P>

<P>After you click the Convert button or menu item, a "dialog box" appears for you to specify filenames. When the dialog box appears, it shows the filenames from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the Browse button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</P>

<P>You may also specify the number of records to convert. This can be helpful when you are testing. Use 0 (zero) when you want to convert the entire file. (Note: the "Maximum records" setting in the <a href="#settings">Settings dialog</a> does not apply to conversion.)
</P>


<p>
Click the "OK" button when you are ready to start the conversion. When the run finishes, click the "OK" button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>
In Command Mode, you may convert a file by using a command from the Windows command prompt. The command can be added to a batch or script for automatic processing.
</p>

<p>
We assume you are familiar with using the command window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".
</p>

<P>
To run MARConvert in Command Mode, follow these steps:
</p>

<ol>
<li>Open a command prompt window in Windows.</li>
<li>Enter the name of the program followed by one or two parameters, separated by spaces, as follows:
<ol>
<li>The name of the MARC file to be converted</li>
<li>(Optional) "yes" if the command window should close automatically after the run; "no" is assumed if this parameter is missing. NOTE: the window will not close if a problem is detected, regardless of the parameter.</li>
</ol>

<li>The program will open a new window to display its messages. When the program is done, press the <Enter> key to close this window (unless you used the second parameter). (The same information will be found in the report file, so there is no need to copy it.)</li>
</ol>

<p>
In Command Mode, the output files and report file are named for the input file as described above. The name of the Translation Table and the number of records to run are taken from MARCView.ini in your Windows directory -- in other words, the values used in the previous run; if these need to be changed you can do this by performing an Interactive run, or you can edit MARCView.ini directly.
</p>

<p class=helpnote>
WARNING: In Command Mode, if the output files and report file exist, they will be overwritten.
</p>

<p>
The name of the program and the filename parameter may contain path information if necessary. The program name and input filename can contain spaces if required, but if so, enclose them in quotation marks. No part of the command is case-sensitive.
</p>

<p>
Examples: 
</p>

<pre>
    MARConvert c:\mydata\anydata.mrc
    MARConvert c:\mydata\anydata.mrc yes
    "c:\program files\marconvert\MARConvert" "c:\program files\marconvert\anydata.mrc" yes
</pre>

<p class=helpnote>
Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='etable'></a>
<h2>Translation Table</h2>

<p>
The Translation Table allows you to modify the mapping of the Export. You may add or remove lines, change the MARC tags and subfield codes, or change the fields to which the data is exported.
</p>

<p class=helpnote>
Program logic depends on the filenames remaining as they were during development. Please contact Systems Planning if filenames need to be changed.
</p>

<P>
The Translation Table can be edited with Notepad. The Table consists of three columns separated with spaces and/or tabs. You may use as many spaces and tabs as you like to line up the columns for legibility. If a value in a column includes a space, put quotation marks around it.
</P>

<p>
The Export Translation Table has three columns:
</p>

<ol>
<li>
Column 1 codes the MARC content designators describing the source of the data. The code consists of a three-digit MARC tag, optionally followed by one or more subfield codes. A subfield code of '-' means "indicators".
</li>

<li>
Column 2 codes the filename and column of the output table to which the data described in column 1 is written. The filename is followed by a slash and the column name.
</li>

<li>
Column 3 contains constants that may be substituted for or combined with the data in the MARC element. Or, if the constant is to be used independently of a MARC element, use a hyphen in Column 1. Constants must be within double quotation marks.
</li>
</ol>

<p>
The lines in the Translation Table should be in the order desired for the columns of the output files. All lines for the each of the three tables must be together.
</p>

<p>
The Translation Table must end with a line whose first character is a slash.
</p>

<p>
Fields in the MARC records are exported in the order they are listed in the Translation Table. Fields in the MARC record not found in the Translation Table are ignored.
</p>


<DIR>

<P><b>Translation Table as delivered</b></P>

<pre>
MARConvert Translation Table for KKK 11/5/2004
FieldSep        "T"
SubfieldSep     " "
LDR             -
001             objects/bibid
035z            objects/objectnumber
0359            objects/objectnumber
-               objects/sortnumber
-               objects/objectcount             "0"
-               objects/departmentid            "15"
-               objects/objectstatusid          "1"
-               objects/classificationid        "0"
-               objects/subclassid              "0"
-               objects/type                    "0"
-               objects/loanclassid             "0"
008/7-10        objects/datebegin
008/11-14       objects/dateend
-               objects/objectname
245f            objects/dated
260c            objects/dated
655a            objects/medium
-               objects/dimensions
-               objects/dimensionremarks
520             objects/description
-               objects/pubreferences
-               objects/publicaccess            "0"
-               objects/curatorapproved         "0"
-               objects/onview                  "0"
-               objects/textsearchid            "0"
-               objects/loginid                 "KKKREG"
-               objects/entereddate
-               objects/objectlevelid           "1"
-               objects/objecttypeid
-               objects/objectnameid
-               objects/objectnamealtid
260ab           objects/histattributions
001             objtitles/bibid
-               objtitles/titletypeid
245abkhc        objtitles/title
246ab           objtitles/title
240a            objtitles/title
-               objtitles/active                "1"
-               objtitles/loginid               "KKKREG"
-               objtitles/entereddate
001             altnums/bibid
-               altnums/tableid                 "108"
001             altnums/altnum
0359            altnums/altnum
-               altnums/description
-               altnums/loginid                 "KKKREG"
-               altnums/entereddate
//end of file
</pre>

</DIR>

<h3>Notes on the Example</h3>

<ol>
<li>The first line of the Translation Table is ignored. You may record comments here.</li>

<li>The second line specifies the <B>Field Separator</B> used between fields in the output files.  The capital T indicates the tab character.</li>

<li>The third line specifies the <B>Subfield Separator</B> used between subfields in the MARC record that are being combined into a single output field. This is overridden for certain fields in the program code.
</li>
</ol>

<p>
Put the desired values for the Separators into the second column. If your desired separator includes spaces, the Separator value must be enclosed in double quotes -- but if your separator includes a backslash (also called a back solidus), the backslash cannot immediately precede a double quote.
</P>


<h3>Field mapping</h3>

<p>
After the third line, the Translation Table consists of one line for the MARC element or Constant you wish to output. A MARC "element" can be an entire field, a byte-range of a fixed field, or one or more subfields of a variable field.
</p>

<ul>
<LI>Column 1 specifies the MARC elements that are to be exported. You may specify one or more subfields to be exported; if there are no subfield codes, the entire field is output. Use a hyphen (-) after a tag to export the indicators. You may export the indicators either as a separate field or combined with one or more subfields, or not at all. Do not use $, ‡, or spaces before the subfield codes.</li>

<li>For fixed-length elements like the Leader and 008, you may export the entire field, or you may specify the bytes to export, for example "008", "008/6", or "008/0-5".</LI>

<LI>
Column 2 shows the output file to use for this MARC element, and the field in that file. Use a slash between the file name and the field name. If column 2 contains a hyphen, that field is not exported; a line like this is purely for documentation purposes (such as LDR in the example; you may remove this line if you wish).
</LI>

<li>
Column 3 (optional) shows constants to be put into the export files. Constants must be within double quotation marks.
</li>
</UL>

<p>
Where MARConvert must choose between alternatives for different records, such as for the objtitles/title field, notice that the Translation Table includes all the possible sources from the MARC record. Once all the sources are read, the program determines which one to output.
</p>

<p>
If Column 1 contains a hyphen, and Column 3 is missing, MARConvert will output just the tab delimiter. See "objects/sortnumber" in the example.
</p>

<h3>More on Constants</h3>

<p>
Constants may be combined with variables from the MARC record in this way: if the Constant contains the characters "+++", then the MARC data is inserted at that point. If there is no "+++", then the Constant is used only if the MARC element is missing.
</p>

<p>
<b>Example with fictitious data:</b>
<pre>
         100a      objects/author      "Primary author: +++"
         100d      objects/authdates   "No dates"
</pre>
For a 100 consisting of "‡aSmith, John‡d1940-" the output will be
<pre>
         Primary author: Smith, John
         1940-
</pre>
For a 100 consisting of "‡aSmith, Mary" the output will be
<pre>
         Primary author: Smith, Mary
         No dates
</pre>

<p>
If the 100 field is missing, there is no output from these lines.
</p>

<P>
To add a MARC element to more than one output file, simply add it to the Translation Table more than once (see the 001 field in the example).
</p>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>

<a name='charconv'></a>
<h2>Character Conversion</h2>

<p>
MARConvert for KKK expects records in the MARC-8 character set as input, and produces output in the Latin-1 character set.
</p>

<p>
Because not all MARC-8 symbols have Latin-1 equivalents, some symbols are converted to the closest representation, or not converted at all. The following table shows the conversion performed. MARC-8 symbols with no Latin-1 symbol are removed from the record and the space closed up.
</p>

<table class=embedded border>
<tr>
<th>Name</th>
<th>MARC-8 symbol</th>
<th>Latin-1 symbol</th>
</tr>

<tr><td>POLISH L UPPERCASE</td><td>A1</td><td>L</td></tr>
<tr><td>SCANDINAVIAN O UPPERCASE</td><td>A2</td><td>Ø</td></tr>
<tr><td>D WITH CROSSBAR UPPERCASE</td><td>A3</td><td>Ð</td></tr>
<tr><td>ICELANDIC THORN UPPERCASE</td><td>A4</td><td>Þ</td></tr>
<tr><td>DIGRAPH AE UPPERCASE</td><td>A5</td><td>Æ</td></tr>
<tr><td>DIGRAPH OE UPPERCASE</td><td>A6</td><td>Œ</td></tr>
<tr><td>SOFT SIGN (PRIME), MIAGKIY ZNAK</td><td>A7</td><td>´</td></tr>
<tr><td>MIDDLE DOT</td><td>A8</td><td>·</td></tr>
<tr><td>MUSICAL FLAT</td><td>A9</td><td>flat</td></tr>
<tr><td>PATENT MARK</td><td>AA</td><td>®</td></tr>
<tr><td>PLUS OR MINUS</td><td>AB</td><td>±</td></tr>
<tr><td>O-HOOK UPPERCASE</td><td>AC</td><td>O</td></tr>
<tr><td>U-HOOK UPPERCASE</td><td>AD</td><td>U</td></tr>
<tr><td>ALIF</td><td>AE</td><td>’</td></tr>
<tr><td>ALPHA</td><td>AF</td><td>a</td></tr>
<tr><td>AYN</td><td>B0</td><td>‘</td></tr>
<tr><td>POLISH L LOWERCASE</td><td>B1</td><td>l</td></tr>
<tr><td>SCANDINAVIAN O LOWERCASE</td><td>B2</td><td>ø</td></tr>
<tr><td>D WITH CROSSBAR LOWERCASE</td><td>B3</td><td>d</td></tr>
<tr><td>ICELANDIC THORN LOWERCASE</td><td>B4</td><td>þ</td></tr>
<tr><td>DIGRAPH AE LOWERCASE</td><td>B5</td><td>æ</td></tr>
<tr><td>DIGRAPH OE LOWERCASE</td><td>B6</td><td>œ</td></tr>
<tr><td>HARD SIGN, TVERDYI ZNAK</td><td>B7</td><td>´</td></tr>
<tr><td>TURKISH I LOWERCASE</td><td>B8</td><td>i</td></tr>
<tr><td>BRITISH POUND</td><td>B9</td><td>£</td></tr>
<tr><td>ETH</td><td>BA</td><td>ð</td></tr>
<tr><td>DAGGER</td><td>BB</td><td>†</td></tr>
<tr><td>O-HOOK LOWERCASE</td><td>BC</td><td>o</td></tr>
<tr><td>U-HOOK LOWERCASE</td><td>BD</td><td>u</td></tr>
<tr><td>DEGREES</td><td>C0</td><td>°</td></tr>
<tr><td>SCRIPT L</td><td>C1</td><td>l</td></tr>
<tr><td>PHONO COPYRIGHT</td><td>C2</td><td>p</td></tr>
<tr><td>COPYRIGHT</td><td>C3</td><td>©</td></tr>
<tr><td>MUSICAL SHARP</td><td>C4</td><td>#</td></tr>
<tr><td>INVERTED QUESTION</td><td>C5</td><td>¿</td></tr>
<tr><td>INVERTED EXCLAMATION</td><td>C6</td><td>¡</td></tr>
<tr><td>PSEUDO QUESTION</td><td>E0</td><td>&nbsp;</td></tr>
<tr><td>GRAVE</td><td>E1</td><td>&nbsp;</td></tr>
<tr><td>A WITH GRAVE</td><td>E1 41</td><td>À</td></tr>
<tr><td>E WITH GRAVE</td><td>E1 45</td><td>È</td></tr>
<tr><td>I WITH GRAVE</td><td>E1 49</td><td>Ì</td></tr>
<tr><td>O WITH GRAVE</td><td>E1 4F</td><td>Ò</td></tr>
<tr><td>U WITH GRAVE</td><td>E1 55</td><td>Ù</td></tr>
<tr><td>a WITH GRAVE</td><td>E1 61</td><td>à</td></tr>
<tr><td>e WITH GRAVE</td><td>E1 65</td><td>è</td></tr>
<tr><td>i WITH GRAVE</td><td>E1 69</td><td>ì</td></tr>
<tr><td>o WITH GRAVE</td><td>E1 6F</td><td>ò</td></tr>
<tr><td>u WITH GRAVE</td><td>E1 75</td><td>ù</td></tr>
<tr><td>ACUTE</td><td>E2</td><td>&nbsp;</td></tr>
<tr><td>A WITH ACUTE</td><td>E2 41</td><td>Á</td></tr>
<tr><td>E WITH ACUTE</td><td>E2 45</td><td>É</td></tr>
<tr><td>I WITH ACUTE</td><td>E2 49</td><td>Í</td></tr>
<tr><td>O WITH ACUTE</td><td>E2 4F</td><td>Ó</td></tr>
<tr><td>U WITH ACUTE</td><td>E2 55</td><td>Ú</td></tr>
<tr><td>Y WITH ACUTE</td><td>E2 59</td><td>Ý</td></tr>
<tr><td>a WITH ACUTE</td><td>E2 61</td><td>á</td></tr>
<tr><td>e WITH ACUTE</td><td>E2 65</td><td>é</td></tr>
<tr><td>i WITH ACUTE</td><td>E2 69</td><td>í</td></tr>
<tr><td>o WITH ACUTE</td><td>E2 6F</td><td>ó</td></tr>
<tr><td>u WITH ACUTE</td><td>E2 75</td><td>ú</td></tr>
<tr><td>y WITH ACUTE</td><td>E2 79</td><td>ý</td></tr>
<tr><td>CIRCUMFLEX</td><td>E3</td><td>&nbsp;</td></tr>
<tr><td>A WITH CIRCUMFLEX</td><td>E3 41</td><td>Â</td></tr>
<tr><td>E WITH CIRCUMFLEX</td><td>E3 45</td><td>Ê</td></tr>
<tr><td>I WITH CIRCUMFLEX</td><td>E3 49</td><td>Î</td></tr>
<tr><td>O WITH CIRCUMFLEX</td><td>E3 4F</td><td>Ô</td></tr>
<tr><td>U WITH CIRCUMFLEX</td><td>E3 55</td><td>Û</td></tr>
<tr><td>a WITH CIRCUMFLEX</td><td>E3 61</td><td>â</td></tr>
<tr><td>e WITH CIRCUMFLEX</td><td>E3 65</td><td>ê</td></tr>
<tr><td>i WITH CIRCUMFLEX</td><td>E3 69</td><td>î</td></tr>
<tr><td>o WITH CIRCUMFLEX</td><td>E3 6F</td><td>ô</td></tr>
<tr><td>u WITH CIRCUMFLEX</td><td>E3 75</td><td>û</td></tr>
<tr><td>TILDE</td><td>E4</td><td>&nbsp;</td></tr>
<tr><td>A WITH TILDE</td><td>E4 41</td><td>Ã</td></tr>
<tr><td>N WITH TILDE</td><td>E4 4E</td><td>Ñ</td></tr>
<tr><td>O WITH TILDE</td><td>E4 4F</td><td>Õ</td></tr>
<tr><td>a WITH TILDE</td><td>E4 61</td><td>ã</td></tr>
<tr><td>n WITH TILDE</td><td>E4 6E</td><td>ñ</td></tr>
<tr><td>o WITH TILDE</td><td>E4 6F</td><td>õ</td></tr>
<tr><td>MACRON</td><td>E5</td><td>&nbsp;</td></tr>
<tr><td>BREVE</td><td>E6</td><td>&nbsp;</td></tr>
<tr><td>SUPERIOR DOT</td><td>E7</td><td>&nbsp;</td></tr>
<tr><td>UMLAUT (DIERESIS)</td><td>E8</td><td>&nbsp;</td></tr>
<tr><td>A WITH UMLAUT</td><td>E8 41</td><td>Ä</td></tr>
<tr><td>E WITH UMLAUT</td><td>E8 45</td><td>Ë</td></tr>
<tr><td>I WITH UMLAUT</td><td>E8 49</td><td>Ï</td></tr>
<tr><td>O WITH UMLAUT</td><td>E8 4F</td><td>Ö</td></tr>
<tr><td>U WITH UMLAUT</td><td>E8 55</td><td>Ü</td></tr>
<tr><td>a WITH UMLAUT</td><td>E8 61</td><td>ä</td></tr>
<tr><td>e WITH UMLAUT</td><td>E8 65</td><td>ë</td></tr>
<tr><td>i WITH UMLAUT</td><td>E8 69</td><td>ï</td></tr>
<tr><td>o WITH UMLAUT</td><td>E8 6F</td><td>ö</td></tr>
<tr><td>u WITH UMLAUT</td><td>E8 75</td><td>ü</td></tr>
<tr><td>y WITH UMLAUT</td><td>E8 79</td><td>ÿ</td></tr>
<tr><td>HACEK</td><td>E9</td><td>&nbsp;</td></tr>
<tr><td>CIRCLE ABOVE (ANGSTROM)</td><td>EA</td><td>&nbsp;</td></tr>
<tr><td>A WITH CIRCLE ABOVE</td><td>EA 41</td><td>Å</td></tr>
<tr><td>a WITH CIRCLE ABOVE</td><td>EA 61</td><td>å</td></tr>
<tr><td>LIGATURE, FIRST HALF</td><td>EB</td><td>&nbsp;</td></tr>
<tr><td>LIGATURE, SECOND HALF</td><td>EC</td><td>&nbsp;</td></tr>
<tr><td>HIGH COMMA OFF CENTER</td><td>ED</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE ACUTE</td><td>EE</td><td>&nbsp;</td></tr>
<tr><td>CANDRABINDU</td><td>EF</td><td>&nbsp;</td></tr>
<tr><td>CEDILLA</td><td>F0</td><td>&nbsp;</td></tr>
<tr><td>C WITH CEDILLA</td><td>F0 43</td><td>Ç</td></tr>
<tr><td>c WITH CEDILLA</td><td>F0 63</td><td>ç</td></tr>
<tr><td>RIGHT HOOK</td><td>F1</td><td>&nbsp;</td></tr>
<tr><td>DOT BELOW CHARACTER</td><td>F2</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE DOT BELOW CHARACTER</td><td>F3</td><td>&nbsp;</td></tr>
<tr><td>CIRCLE BELOW CHARACTER</td><td>F4</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE UNDERSCORE</td><td>F5</td><td>&nbsp;</td></tr>
<tr><td>UNDERSCORE</td><td>F6</td><td>&nbsp;</td></tr>
<tr><td>LEFT HOOK/TAIL</td><td>F7</td><td>&nbsp;</td></tr>
<tr><td>RIGHT CEDILLA</td><td>F8</td><td>&nbsp;</td></tr>
<tr><td>UPADHMANIYA</td><td>F9</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE TILDE FIRST HALF</td><td>FA</td><td>&nbsp;</td></tr>
<tr><td>DOUBLE TILDE SECOND HALF</td><td>FB</td><td>&nbsp;</td></tr>
<tr><td>HIGH COMMA CENTERED</td><td>FE</td><td>&nbsp;</td></tr>
</table>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='special'></a>
<h2>Special Processing</h2>

<p>
For objects/objectnumber, only the first 035‡z subfield is output.
</p>

<p>
For objects/objectnumber, the 035‡9 subfield is not output if the 035‡z subfield exists.
</p>

<p>
For objects/dated, the 260‡c is output for textual material (Leader/6 = 'p'); the 245‡f otherwise.
</p>

<p>
All asterisks are removed from 035‡z subfields.
</p>

<p>
The objtitles/titletypeid field is generated based on the title tag:
</p>

<table class=embedded border>
<tr><th>tag</th><th>titletypeid</th></tr>
<tr><td>245</td><td>6</td></tr>
<tr><td>246</td><td>10</td></tr>
<tr><td>240</td><td>22</td></tr>
</table>

<p>
The altnums/description field is generated based on the number tag:
</p>

<table class=embedded border>
<tr><th>tag</th><th>description</th></tr>
<tr><td>001</td><td>VOYAGERID</td></tr>
<tr><td>035</td><td>BIBID</td></tr>
</table>

<p>
If an 008 date (bytes 7-10 or 11-14) contains four blanks, '0' is output instead.
</p>

<p>
If an 008 date (bytes 7-10 or 11-14) contains any non-digits, '0's are substituted for the non-digits.
</p>

<p>
If 008/6 is 'e', a '0' is output instead of the 008/11-14 date.
</p>

<p>
If a 245 field (data plus constant, if any) is longer than 255 bytes, the field is truncated at 252 bytes and "..." is added.
</p>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='errors'></a>
<h2>Error Codes</h2>

<p>
If a record cannot be read or has data problems, a message is put into the report file. Error codes are as follows:
</p>

<table class=embedded border>
<tr>
<th>Code</th>
<th>Meaning</th>
</tr>

<tr>
<td>3</td>
<td>Record has severe validation problems, such as invalid record length, missing record terminator, or garbled leader or directory.</td>
</tr>

<tr>
<td>2</td>
<td>Record has minor validation problems, such as a non-numeric tag.</td>
</tr>

<tr>
<td>-1</td>
<td>Unknown error.</td>
</tr>

<tr>
<td>-4</td>
<td>End-of-file before end of record. Check for file corruption.</td>
</tr>

<tr>
<td>-5</td>
<td>Not a MARC record, or severely damaged.</td>
</tr>

<tr>
<td>-8</td>
<td>Out of memory.</td>
</tr>

<tr>
<td>-9</td>
<td>Program problem. Notify Systems Planning.</td>
</tr>

<tr>
<td>-10</td>
<td>Problem converting characters.</td>
</tr>

<tr>
<td>-11</td>
<td>May be a blocked file.</td>
</tr>

</table>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows folder. This file is shared with our MARCView product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>





<br><p><br>
<hr>
<a name="convert"></a>
<h1 align=center>Client LLL</h1>

<a name='import'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for LLL converts files of MARC 21 records to blocked files, following the record segmentation specifications in <i>MARC 21 Specifications for Record Structure, Character Sets, and Exchange Media</i> (<a href="http://www.loc.gov/marc/specifications/specexchtape1.html#seg">http://www.loc.gov/marc/specifications/specexchtape1.html#seg</a>). A report file is also produced that tells whether the conversion was successful, how many records were converted, and the number of "bad" records. (Bad records are those that contain illegal characters, as described below under <a href="#valid">Validation</a>.)
</P>

<p>
The conversion performs some validation: it checks to make sure that all characters are legal MARC-8 characters, reporting on any problems. In addition, MARConvert checks that the entire record is present; in case of a truncated file, the last good record and block are written. Other validation can be added if needed.
</p>

<p>
MARConvert works on both bibliographic and authority records. 
There is no limit to the number of records that may be in an input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes). The input file is not affected in any way by converting it; the converted records are put into a new file. 
</P>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>
In Interactive Mode you will may convert a single file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the "Convert" button or the File/Convert menu item (they do exactly the same thing). (If you are viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</P>

<P>After you click the Convert button or menu item, a "dialog box" appears for you to specify filenames. When the dialog box appears, it shows the filenames from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the Browse button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</P>

<p>
Click the "OK" button when you are ready to start the conversion. When the run finishes, click the "OK" button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>
In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
</p>

<p>
To use the Command Mode, you must first create a "Command File". This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File:
</p>

<pre>
     MARConvert parameter file, 3/13/04
     overwrite    yes
     report       "d:\marc\marconvert\LLL\MARConvert.rpt"
     "d:\marc\marconvert\LLL\brkrtest.mrc"
     "d:\marc\data\good\diacrit"
     "d:\marc\data\good\harvard.txt"
</pre>

<p>
Notes on the example:
</p>

<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>
<li>The second line says that the program should overwrite output files and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more tab characters to separate the word "overwrite" from the "yes" or "no".</li>
<li>The third line says where the report file should be created and what it should be called. Use one or more tab characters to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
<li>The fourth and subsequent lines list the MARC files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
</ol>

<p class=helpnote>
The output files will be created in the directory from where you start the program (see below), and will be given the same name as the input file but with the extension "BLK". The output files for the example above will be named "brkrtest.BLK", "diacrit.BLK", and "harvard.BLK".
</p>

<p>
Command Mode is used as follows:
</p>

<ol>
<li>Open a Windows "Command Prompt" window. In Windows 2000 this is done by selecting Start/Programs/Accessories/Command Prompt. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>
<li>Navigate to the directory where you want the output files to be created.</li>
<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: 
<pre>
     "c:\program files\marconvert\marconvert" command.txt
</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command.txt" and exists in the "current directory". If the file "command.txt" were not in the directory from which you are entering the prompt, you may specify its path also, as in 
<pre>
     "c:\program files\marconvert\marconvert" "d:\mydata\command.txt"
</pre>
Or you may decide to use a different name for the Command File; if so just specify it as in this example:
<pre>
     "c:\program files\marconvert\marconvert" "d:\mydata\conversion_list.txt"
</pre>
</li>
<li>After entering these two items at the prompt, press the Enter key. When MARConvert begins to run, it will open a new window and show you its progress. The window will close when the run is complete.</li>
</ol>

<p class=helpnote>
Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='valid'></a>
<h2>Validation</h2>

<p>
MARConvert performs basic validation to ensure that the input file contains valid MARC records. Problems are handled according to severity:
</p>

<ul>
<li>
<b>Unreadable file</b> -- The last good record and block will be output and then the conversion will stop. In command mode, the next file is attempted.
</li>
<li>
<b>Unreadable record</b> -- The last good record and block will be output and then the conversion will stop. In command mode, the next file is attempted.
</li>
<li>
<b>Illegal characters in record</b> -- The record is skipped and the next record is attempted. Illegal characters are defined as hexadecimal 00-1A, 1C, 7F-A0, AF, BB, BE, BF, C9-DF, FC, FD, FF.
</li>
</ul>

<p>
In all cases, the problem is logged to the report file.
</p>

<h3>Record Terminator problems</h3>

<p>
Beginning with release 1.4, if a record does not have a Record Terminator (RT) as its last byte (as calculated from the Logical Record Length), MARConvert goes back to the start of the record (point A) and scans forward until an RT is found (point B). The bytes between A and B are skipped, and conversion resumes with the first numeric digit after point B. 
</p>

<p>
Note that since more than one record may be skipped between A and B (depending on the problem), the record numbers and counts after that point may not be accurate.
</p>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows folder. This file is shared with our MARCView product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>





<br><p><br>

<hr>

<a name=overview></a>
<h1 align=center>Client MMM</h1>

<h2>Overview</h2>

<p>
This customized version of MARConvert for MMM converts MARC21 records into delimited-text files which it then loads into SQL Server. The input files are left unchanged.
</p>

<p>
MARConvert creates delimited-text files suitable for loading using Microsoft's BCP (bulk-convert program). MARConvert calls BCP when all input files have been processed. If BCP fails for any reason, the MARConvert output files can be loaded by invoking BCP from the Windows command prompt.
</p>

<p>
Before each run, MARConvert sends SQL Server the command ALTER INDEX ALL ON IIIDBA DISABLE to turn off the indexes; this greatly improves bulk-loading performance. After the run, the command ALTER INDEX ALL ON IIIDBA REBUILD is sent to SQL Server to rebuild the indexes. The use of ALTER INDEX instead of dropping and recreating indexes means that you can add, change, or drop indexes without our having to modify MARConvert.
</p>

<p>
In order to facilitate bulk-loading, the text output files are named for the tables they are to be added to, such as BIBLIO2BASE.txt and VAR_FIELDS2.txt.
</p>

<p>In addition to the output files, MARConvert creates a report file which can be viewed or printed. After converting, the report should be looked at to see if there were any problems. The report provides a count of records converted, as well as a listing and count of problem records.</p>

<p>There is no limit to the number of records in the input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes).</p>


<h3>ODBC setup</h3>

<p>
MARConvert empties the four SQL Server tables before each run. To do so, it needs an ODBC DSN (data-source name) set up for the SQL Server database. This is done by the following two steps:
</p>

<ol>
<li>
Create an ODBC data-source for the SQL Server database on your computer, using the Native Client ODBC driver (you select this in the ODBC Adminstrator).
</li>

<p class=note>
SQL Server comes with two ODBC drivers. We use the SQL Server Native Client ODBC driver; that it is the only one that offers all the features and data types of SS 2005; the older SQL Server driver is based on MDAC and will not be improved going forward. Using the Native Client driver appears to be Microsoft's recommendation for new applications.
</p>

<li>
The file MARCView.ini is created by the MARConvert installation program in your Windows directory. Add a new line to the [Export] section of MARCView.ini, as follows:
<br><br>
	DSN=xxx
<br><br>
where xxx is the data-source name you created in step 1.
</li>
</ol>


	<div class=gotop><a href="#contents">Top</a></div>


<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>In Interactive Mode you may convert a single file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the <b>Convert</b> button or the <b>File/Convert</b> menu item (they do exactly the same thing). (If you had been viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</p>

<p>
After you click the <b>Convert</b> button or menu item, a dialog box appears for you to specify the run parameters. When the dialog box appears, it shows the settings from the previous run -- this makes it easy to rerun the previous file.
</p>

<ol>
<li>
Select a <b>Type of input</b>, either bibliographic or order.
</li>
<li>
Click the <b>Browse</b> button to search for the input file (or type its full path and name).
</li>
<li>
Enter the full path and name of the report file you wish.
</li>
<li>
The <b>Records to export</b> box allows you to convert a partial file. This can be useful in testing. To convert the entire file, enter a 0 here.
</li>
<li>
Click the <b>OK</b> button when you are ready to start the conversion. When the run finishes, click the <b>OK</b> button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.
</li>
</ol>

<p>
The output files are created in the directory where the report is written. The directory where the report file and output files are written must exists before the run.
</p>


<div class=gotop><a href="#contents">Top</a></div>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
</p>

<p>
All input files write to the same output files, so that you have a single set of files to import into SQL Server regardless of how many input files there are. The output files are created in the directory where the report is written (you can control this with the Command File); this means the input files may be in different directories if you wish.
</p>

<p>
The directory where the report file and output files are written must exists before the run.
</p>


<h3>Creating the Command File</h3>

<p>
To use the Command Mode, you must first create a <b>Command File</b>. This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File (included in the installation as a model):
</p>

<pre>
     MARConvert command file
     overwrite     yes
     report        "d:\marc\marconvert\MMM\MARConvert.rpt"
     bib           "d:\MARC\MARConvert\MMM\testoutput.mrc"
     bib           "d:\MARC\MARConvert\MMM\MMM.bibs.d062007.mrc"
     order         "d:\MARC\MARConvert\MMM\orders_06_28_07a.mrc"
</pre>

<p>
Notes on the example:
</p>

<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>
<li>The second line says that the program should overwrite the output file and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more tab characters or spaces to separate the word "overwrite" from the "yes" or "no".</li>

<li>Command Mode creates a single report file for all MARC files converted. The third line says where this report file should be created and what it should be called. Use one or more tab characters or spaces to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>

<li>The fourth and subsequent lines list the files to be converted. Precede each file specification with "bib" or "order" to indicate its type. Follow "bib" or "order" with one or more spaces or tabs. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>

</ol>


<h3>Using Command Mode</h3>
<p>Command Mode is used as follows:</p>
<ol>
<li>Open a Windows "Command Prompt" window. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>

<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: 
<pre>     "c:\Program Files\Systems Planning\MARConvert\MARConvert.exe" command_file.txt</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command_file.txt" and exists in the current directory. If the file "command_file.txt" is not in the directory from which you are entering the prompt, you may specify its path also, as in
<pre>     "c:\Program Files\Systems Planning\MARConvert\MARConvert.exe" "d:\mydata\command_file.txt"</pre>
</li>

<li>
When MARConvert begins to run, it will open a new window and show you its progress. This window will close automatically when all files are converted, unless there was a serious error.

<p class=helpnote>
Some BCP messages can be shown only in this window and not in the report. This is outside our control.
</p>

</ol>
<p class=helpnote>Tip: If you use the same Command File every time, you can create a shortcut on your Windows desktop that will start MARConvert and read the Command File.</p>


<div class=gotop><a href="#contents">Top</a></div>


<a name='bcp'></a>
<h2>Using BCP</h2>

<p>
MARConvert creates delimited-text files suitable for loading using Microsoft's BCP (bulk-convert program) utility. BCP is installed to the server when SQL Server is installed. MARConvert calls BCP when all input files have been processed. If BCP fails for any reason, the MARConvert output files can be loaded by invoking BCP from the Windows command prompt.
</p>

<p>
BCP must be run separately for each table. In order to facilitate bulk-loading, the text output files are named for the tables they are to be added to, such as BIBLIO2BASE.txt and VAR_FIELDS2.txt.
</p>

<p>
The BCP command line to be used for BIBLIO2BASE is this. The other three are identical except for the names of the table, the input file, and the error file:
</p>

<pre>
     bcp.exe IIIDBA.dbo.biblio2base in "d:\data\biblio2base.txt" -c -T -F2 -C RAW -h "TABLOCK" -e "d:\data\bcp_biblio2base.err"
</pre>

<p>
The elements of the command have these meanings. The spacing and case of the parameters are critical.
</p>

<table class=embedded border>
<tr>
<td>bcp.exe</td>
<td>The name of the program. When SS is installed, the location of BCP is added to the Windows path. Or you can add the path if required. The ".exe" is optional.</td>
</tr>
<tr>
<td>IIIDBA.dbo.biblio2base</td>
<td>The fully qualified name of the table to load into. This may be different on your server from what is shown here.</td>
</tr>
<tr>
<td>in</td>
<td>Indicates that this is an import into SS. (BCP can also do other things.)</td>
</tr>
<tr>
<td>"d:\data\biblio2base.txt"</td>
<td>The full path to the MARConvert output file. Double quotes are required if the path contains spaces. MARConvert puts its output files in the directory you specify where the report should be written.</td>
</tr>
<tr>
<td>-c</td>
<td>Tells BCP that the data is character data rather than binary.</td>
</tr>
<tr>
<td>-T</td>
<td>Indicates a trusted source. You may prefer to use the -U and -P parameters with user ID and password.</td>
</tr>
<tr>
<td>-F2</td>
<td>Tells BCP to skip the first line, which contains the column names.</td>
</tr>
<tr>
<td>-C RAW</td>
<td>Tells BCP not to convert the characters to UTF-8. This preserves the MARC-8 encoding.</td>
</tr>
<tr>
<td>-h "TABLOCK"</td>
<td>Tells BCP to use table-level locking rather than row-level. This improves performance. You should remove this parameter if the tables will be used by other applications during loading.</td>
</tr>
<tr>
<td>-e&nbsp;"d:\data\bcp_biblio2base.err"</td>
<td>BCP will write errors to this file. Double quotes are required if the path contains spaces. The file is created even if there are no errors, but it will have 0 bytes. You should look at these files after the MARConvert run and after running BCP by hand. MARConvert puts its the error files in the directory you specify where the report should be written.</td>
</tr>
</table>


<h3>BCP performance</h3>

<p>
The speed at which BCP loads depends on certain database configurations that are beyond the control of MARConvert. Microsoft recommends the following actions before running BCP to maximize performance.
</p>

<ul>
<li>
Avoid transferring data across a network. In other words, install MARConvert on the same computer that the database is on.
</li>
<li>
If you are using the "Simple" recovery model, no change is required. If you are using the "Full" recovery model, switch to the "Bulk-logged" recovery model just before the load and reset after.
</li>
<li>
Drop indexes before the run and rebuild them later.
</li>
<li>
Disable constraints and triggers if possible.
</li>
<li>
Empty tables if possible.
</li>
</ul>


	<div class=gotop><a href="#contents">Top</a></div>

<a name=transtable></a>
<h2>Translation Table</h2>
<p>The Translation Tables MMM_bib.mce and MMM_order.mce are text files used by MARConvert to express the mapping between the MARC records and the target data model. Normally the Translation Table can be used to modify the mapping, but MMM requires so much special processing in the program code that this is not possible. Instead, please contact Systems Planning if changes are required.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='inifile'></a>
<h2>MARCView.ini</h2>
<p>Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows directory. This file is shared with our MARCView&#8482; product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<div class=gotop><a href="#contents">Top</a></div>





<br><p><br>
<hr>
<a name=overview></a>
<h1 align=center>Client NNN</h1>
<h2>Overview</h2>
<p>This customized version of MARConvert for NNN converts MARC21 records into a text format required for mainframe processing. The input files are left unchanged.</p>
<p>In addition to the output files, MARConvert creates a report file which can be viewed or printed. After converting, the report should be looked at to see if there were any problems. The report provides a count of records converted, as well as a listing and count of problem records.</p>
<p>There is no limit to the number of records in the input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes).</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>
<p>In Interactive Mode you may convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the <b>Convert</b> button or the <b>File/Convert</b> menu item (they do exactly the same thing). (If you had been viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</p>
<p>The "Records to export" box allows you to convert a partial file. This can be useful in testing. To convert the entire file, enter a 0 here.</p>
<p>Click the <b>OK</b> button when you are ready to start the conversion. When the run finishes, click the <b>OK</b> button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.</p>
<p>After you click the <b>Convert</b> button or menu item, a dialog box appears for you to specify the file to be converted, the new file, and the report file. When the dialog box appears, it shows the settings from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the <b>Browse</b> button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='command'></a>
<h2>Conversion: Command Mode</h2>
<p>In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
</p>
<h3>Creating the Command File</h3>
<p>To use the Command Mode, you must first create a <b>Command File</b>. This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File (included in the installation as a model):</p>
<pre>     MARConvert command file<br>     overwrite yes<br>     report "d:\marc\marconvert\NNN\MARConvert.rpt"<br>     "d:\marc\marconvert\NNN\cis2002.mrc"<br>     "d:\marc\marconvert\NNN\brkrtest.mrc"</pre>
<p>Notes on the example:</p>
<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>
<li>The second line says that the program should overwrite the output file and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more tab characters or spaces to separate the word "overwrite" from the "yes" or "no".</li>
<li>Command Mode creates a single report file for all MARC files converted. The third line says where this report file should be created and what it should be called. Use one or more tab characters or spaces to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
<li>The fourth and subsequent lines list the files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
</ol>

<p>The output files will be created in the directory the input files are in, and will be given the same name as the input file but with the extension "MCX" (MARConvert eXport). The output files for the example above will be named "cis2002.MCX", and "BRKRTEST.MCX".</p>

<h3>Using Command Mode</h3>
<p>Command Mode is used as follows:</p>
<ol>
<li>Open a Windows "Command Prompt" window. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>
<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: </li>
<pre>     "c:\program files\marconvert\marconvert" command_file.txt</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command_file.txt" and exists in the current directory. If the file "command_file.txt" is not in the directory from which you are entering the prompt, you may specify its path also, as in
<pre>     "c:\program files\marconvert\marconvert" "d:\mydata\command_file.txt"</pre>
</li>
<li>After entering these two items at the prompt, press the Enter key. When MARConvert begins to run, it will open a new window and show you its progress. When prompted, press the enter key again to close this window.</li>
</ol>
<p class=helpnote>Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.</p>
<p class=helpnote>Tip: If you use the same Command File every time, you can create a shortcut on your Windows desktop that will start MARConvert and read the Command File.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='notes'></a>
<h2>Conversion Notes</h2>
<p>The format of the output file is as follows:<ul><li>The file is plain ASCII text</li><li>The 001 field is output first, then the Leader, then all other fields in the order encountered in the record</li><li>The MARC tag is used as the output field-tag, except that NEW is used for the 001 and LDR is used for the Leader</li><li>Each field-tag is preceded by "#"</li><li>Indicators are preceded by " @" and followed by " "</li><li>Spaces in indicators, Leader, and fields 001 through 008 are replaced by "$"</li><li>Subfield delimiters are replaced by "~"</li><li>Field terminators are replaced by return/linefeed</li><li>Record terminators are removed; there is no delimiter between records</li><li>Lines are wrapped at spaces to make lines of 79 characters or fewer; trailing spaces are removed. If a line has no spaces in the first 79 characters it will be wrapped at the first space, and so will be longer than 79 characters. (This is because trailing spaces are removed, so forcing a break at 79 characters might cause the mainframe to insert a space where it should not be.)</li><li>No conversion of characters from the MARC-8 character set is performed, as the source data does not contain non-ASCII characters.</li></ul></p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='inifile'></a>
<h2>MARCView.ini</h2>
<p>Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows directory. This file is shared with our MARCView&#8482; product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. </p>
<div class=gotop><a href="#contents">Top</a></div>




<br><p><br>

<hr>

<a name=overview></a>
<h1 align=center>Client PPP</h1>

<h2>Overview</h2>

<p>
This customized version of MARConvert for PPP converts MARC21 records into delimited text files for importing into SQL Server. The input files are left unchanged.
</p>

<p>
MARConvert creates two forms of output: (1) delimited-text files suitable for loading using Microsoft's BCP (bulk-convert program); and (2) optionally, data is loaded into SQL Server using ODBC.
</p>

<p>
In order to facilitate bulk-loading, the text output files are named for the tables they are to be added to, such as Main.txt and Authors.txt.
</p>

<p>In addition to the output files, MARConvert creates a report file which can be viewed or printed. After converting, the report should be looked at to see if there were any problems. The report provides a count of records converted, as well as a listing and count of problem records.</p>

<p>
There is no limit to the number of records in an input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes).
</p>


<div class=gotop><a href="#contents">Top</a></div>


<a name='output'></a>
<h2>Output Options</h2>

<p>
MARConvert creates delimited-text output for every run. SQL statements processed by ODBC is an option that can be turned on or off.
</p>

<p>
To use MARConvert to load into SQL Server using ODBC, do the following two steps:
</p>

<ol>
<li>
Create an ODBC data-source for the SQL Server database on your computer, using the Native Client ODBC driver (you select this in the ODBC Adminstrator).
</li>

<p class=note>
SQL Server comes with two ODBC drivers. We use the SQL Server Native Client ODBC driver; that it is the only one that offers all the features and data types of SS 2005; the older SQL Server driver is based on MDAC and will not be improved going forward. Using the Native Client driver appears to be Microsoft's recommendation for new applications.
</p>

<li>
The file MARCView.ini is created by the MARConvert installation program in your Windows directory. Add a new line to the [Export] section of MARCView.ini, as follows:
<br><br>
	DSN=xxx
<br><br>
where xxx is the data-source name you created in step 1.
</li>
</ol>


<p>
To stop having MARConvert load into SQL Server, either remove the DSN line in MARCView.ini, or you can just comment the line by preceding it with a semicolon.
</p>


<div class=gotop><a href="#contents">Top</a></div>


<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>In Interactive Mode you may convert a single file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the <b>Convert</b> button or the <b>File/Convert</b> menu item (they do exactly the same thing). (If you had been viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</p>

<p>
After you click the <b>Convert</b> button or menu item, a dialog box appears for you to specify the run parameters. When the dialog box appears, it shows the settings from the previous run -- this makes it easy to rerun the previous file.
</p>

<ol>
<li>
Click the <b>Browse</b> button to search for the input file (or type its full path and name).
</li>
<li>
Enter the full path and name of the report file you wish.
</li>
<li>
The <b>Records to export</b> box allows you to convert a partial file. This can be useful in testing. To convert the entire file, enter a 0 here.
</li>
<li>
Click the <b>OK</b> button when you are ready to start the conversion. When the run finishes, click the <b>OK</b> button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.
</li>
</ol>

<p>
The output files are created in the directory where the report is written. The directory where the report file and output files are written must exists before the run.
</p>


<div class=gotop><a href="#contents">Top</a></div>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
</p>

<p>
All input files write to the same output files, so that you have a single set of files to import into SQL Server regardless of how many input files there are. The output files are created in the directory where the report is written (you can control this with the Command File); this means the input files may be in different directories if you wish.
</p>

<p>
The directory where the report file and output files are written must exists before the run.
</p>


<h3>Creating the Command File</h3>

<p>
To use the Command Mode, you must first create a <b>Command File</b>. This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File (included in the installation as a model):
</p>

<pre>
     MARConvert command file
     overwrite     yes
     report        "d:\marc\marconvert\PPP\MARConvert.rpt"
     "d:\MARC\MARConvert\PPP\USM_ara8_120606new.mrc"
</pre>

<p>
Notes on the example:
</p>

<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>
<li>The second line says that the program should overwrite the output file and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more tab characters or spaces to separate the word "overwrite" from the "yes" or "no".</li>

<li>Command Mode creates a single report file for all MARC files converted. The third line says where this report file should be created and what it should be called. Use one or more tab characters or spaces to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>

<li>The fourth and subsequent lines list the files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>

</ol>


<h3>Using Command Mode</h3>

<p>Command Mode is used as follows:</p>

<ol>
<li>Open a Windows "Command Prompt" window. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>

<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: 
<pre>     "c:\Program Files\Systems Planning\MARConvert\MARConvert.exe" command_file.txt</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command_file.txt" and exists in the current directory. If the file "command_file.txt" is not in the directory from which you are entering the prompt, you may specify its path also, as in
<pre>     "c:\Program Files\Systems Planning\MARConvert\MARConvert.exe" "d:\mydata\command_file.txt"</pre>
</li>

<li>
When MARConvert begins to run, it will open a new window and show you its progress. This window will close automatically when all files are converted.
</li>
</ol>

<p class=helpnote>
Tip: If you use the same Command File every time, you can create a shortcut on your Windows desktop that will start MARConvert and read the Command File.
</p>


	<div class=gotop><a href="#contents">Top</a></div>


<a name='notes'></a>
<a name='charconv'></a><h2>Character Conversion</h2>

<p>
MARConvert for PPP converts records in the MARC-8 encoding to UTF-8. As you requested, only symbols in Western European languages are handled; if you require encodings for additional languages, we can add them at any time.
</p>

<p>Character conversion is performed using tables of equivalent encodings. Conversions are based on documentation from the Library of Congress as well as Systems Planning's proprietary symbol conversion table.
</p>


<h3>Conversion problems</h3>

<p class=helpnote>When displaying the results of a conversion, please note that MARConvert can display only Latin-1 characters. Thus it is not a true representation of the results of the conversion. Use MARConvert's <b>Hexview</b> (by clicking the <b>Hex</b> button) to see the actual contents of the records.<br><br>Also note that the display is affected by the <b>Character Set of Records</b> in <a href=#settings>Settings</a>.</p>

<p>When a MARC-8 character is encountered that cannot be found in the conversion tables, MARConvert inserts a Geta (Unicode 3013, UTF-8 E38093);. In addition, a warning message is logged to the report file. As you requested, these records are written to the output files as if they had no problems.</p>


	<div class=gotop><a href="#contents">Top</a></div>


<a name='bcp'></a>
<h2>Using BCP</h2>

<p>
MARConvert creates delimited-text files suitable for loading using Microsoft's BCP (bulk-convert program) utility. BCP is installed to the server when SQL Server is installed. When MARConvert has finished converting the data, load the output files by invoking BCP from the Windows command prompt.
</p>

<p>
BCP must be run separately for each table. In order to facilitate bulk-loading, the text output files are named for the tables they are to be added to, such as Main.txt and Authors.txt.
</p>

<p>
The BCP command line to be used for MAIN is this. The other three are identical except for the names of the table, the input file, and the error file:
</p>

<pre>
     bcp.exe PPP.dbo.main in "d:\data\main.txt" -c -T -F2 -C RAW -h "TABLOCK" -e "d:\data\bcp_main.err"
</pre>

<p>
The elements of the command have these meanings. The spacing and case of the parameters are critical.
</p>

<table class=embedded border>
<tr>
<td>bcp.exe</td>
<td>The name of the program. When SS is installed, the location of BCP is added to the Windows path. Or you can add the path if required. The ".exe" is optional.</td>
</tr>
<tr>
<td>PPP.dbo.main</td>
<td>The fully qualified name of the table to load into. This may be different on your server from what is shown here.</td>
</tr>
<tr>
<td>in</td>
<td>Indicates that this is an import into SS. (BCP can also do other things.)</td>
</tr>
<tr>
<td>"d:\data\main.txt"</td>
<td>The full path to the MARConvert output file. Double quotes are required if the path contains spaces. MARConvert puts its output files in the directory you specify where the report should be written.</td>
</tr>
<tr>
<td>-c</td>
<td>Tells BCP that the data is character data rather than binary.</td>
</tr>
<tr>
<td>-T</td>
<td>Indicates a trusted source. You may prefer to use the -U and -P parameters with user ID and password.</td>
</tr>
<tr>
<td>-F2</td>
<td>Tells BCP to skip the first line, which contains the column names.</td>
</tr>
<tr>
<td>-C RAW</td>
<td>Tells BCP not to convert the characters to UTF-8 since MARConvert has already done this.</td>
</tr>
<tr>
<td>-h "TABLOCK"</td>
<td>Tells BCP to use table-level locking rather than row-level. This improves performance. You should remove this parameter if the tables will be used by other applications during loading.</td>
</tr>
<tr>
<td>-e&nbsp;"d:\data\bcp_main.err"</td>
<td>BCP will write errors to this file. Double quotes are required if the path contains spaces. The file is created even if there are no errors, but it will have 0 bytes. You should look at these files after running BCP by hand.</td>
</tr>
</table>


<h3>BCP performance</h3>

<p>
Microsoft recommends the following actions before running BCP to maximize performance. These are not necessary for loading small files.
</p>

<ul>
<li>
Avoid transferring data across a network. In other words, install MARConvert on the same computer that the database is on.
</li>
<li>
If you are using the "Simple" recovery model, no change is required. If you are using the "Full" recovery model, switch to the "Bulk-logged" recovery model just before the load and reset after.
</li>
<li>
Drop indexes before the run and rebuild them later.
</li>
<li>
Disable constraints and triggers if possible.
</li>
</ul>


	<div class=gotop><a href="#contents">Top</a></div>


<a name=transtable></a>
<h2>Translation Table</h2>

<p>
The Translation Table PPP.mce is a text file used by MARConvert to express the mapping between the MARC records and the target data model. Normally the Translation Table can be used to modify the mapping, but PPP requires so much special processing in the program code that this is not possible. Instead, please contact Systems Planning if changes are required.
</p>


	<div class=gotop><a href="#contents">Top</a></div>


<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows directory. This file is shared with our MARCView&#8482; product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. </p>
<div class=gotop><a href="#contents">Top</a></div>



<br><p><br>
<hr>
<a name="convert"></a>
<h1 align=center>Client QQQ</h1>

<a name='export'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for QQQ converts MARC records in the UTF-8 character set to MARC records in the MARC-8 character set.
</P>

<P>
In addition to the MARC output file, MARConvert creates a report file which can be viewed or printed. Look at the report after exporting to see if there were any problems.
</p>

<P>
There is no limit to the number of records in the input file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes). The original MARC file is not affected in any way by converting it.
</P>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>
In Interactive Mode you will may convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the "Convert" button or the File/Convert menu item (they do exactly the same thing). (If you are viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)
</P>

<P>After you click the Convert button or menu item, a "dialog box" appears for you to specify filenames. When the dialog box appears, it shows the filenames from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the Browse button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.
</P>

<p>
Click the "OK" button when you are ready to start the conversion. When the run finishes, click the "OK" button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>
In Command Mode, you may convert a file by using a command from the Windows command prompt. The command can be added to a batch or script for automatic processing.
</p>

<p>
We assume you are familiar with using the command window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".
</p>

<P>
To run MARConvert in Command Mode, follow these steps:
</p>

<ol>
<li>Open a command prompt window in Windows.</li>
<li>Enter the name of the program followed by one, two, or three parameters, separated by spaces, as follows:
<ol>
<li>The name of the MARC file to be converted</li>
<li>(Optional) "yes" if the command window should close automatically after the run; "no" is assumed if this parameter is missing. NOTE: the window will not close if a problem is detected, regardless of the parameter.</li>
<li>(Optional) "yes" if the output and report files should be overwritten. If this parameter is not "yes" or is missing, you will be asked to confirm the overwriting.</li>
</ol>

<li>The program will open a new window to display its messages. When the program is done, press the <Enter> key to close this window (unless you used the second parameter). (The same information will be found in the report file, so there is no need to copy it.)</li>
</ol>

<p>
In Command Mode, the output files and report file are named for the input file, but with the extensions ".M8" and ".RPT", respectively. 
</p>

<p>
The name of the program and the filename parameter may contain path information if necessary. The program name and input filename can contain spaces if required, but if so, enclose them in quotation marks. No part of the command is case-sensitive.
</p>

<p>
Examples: 
</p>

<pre>
    MARConvert c:\mydata\anydata.mrc
    MARConvert c:\mydata\anydata.mrc yes
    MARConvert c:\mydata\anydata.mrc yes yes
    "c:\program files\marconvert\MARConvert" "c:\program files\marconvert\anydata.mrc" yes
</pre>

<p class=helpnote>
Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='charconv'></a>
<h2>Character Conversion</h2>

<p>
MARConvert for QQQ expects records in the UTF-8 character set as input, and produces output in the MARC-8 character set.
</p>

<p>
Not all UTF-8 symbols have MARC-8 equivalents. If a UTF-8 symbol cannot be converted, a message is generated and the UTF-8 symbol is removed from the record and the space closed up.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows directory. This file is shared with our MARCView&#8482; product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>




<br><p><br>
<hr>
<a name="support"></a>
<h1 align=center>Client RRR</h1>

<a name='import'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for RRR converts text records to MARC. (In MARConvert terminology, this is an "import" into the MARC format.)
</P>

<p>
The input is a text file in which the data for each MARC subfield is in a separate record. The conversion also replaces Latin-1 diacritics and special characters with their MARC equivalents.
</P>

<P>The conversion process results in three files:</P>

<UL>
<LI>A file of MARC records</LI>
<LI>A file of bad records, also in MARC format. Bad records are those in which a proper MARC record could not be built, because of missing mandatory data or similar cause.</LI>
<LI>A report file which can be viewed or printed. Look at the report after importing to see if there were any problems.</LI></UL>

<P>
Typically all three files are given the same name, with the file of good MARC records having the extension "MRC", the bad records "BAD", and the report "RPT". You may override these filenames and extensions when you perform an import in interactive mode.
</P>

<p>
There is no limit to the number of records in the input file.  However, the output file is limited to 2 GB (2,147,483,647 bytes). The input file is not affected in any way by converting it.
</P>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='input'></a>
<h2>Input Files</h2>

<p>
Input files consist of four fields on each line: the MARC tag, the indicators, the subfield code, and the data. These fields are separated by tabs; each record is a new line. This example shows the first few lines of an example input file, where blank indicators are shown as dots:
</p>

<pre>
     001	..	 	PPS-1
     005	..	 	20040301        
     008	..	 	041011i||||||||xx|     |           eng|d
     020	..	a	0-910050-37-6
     035	..	a	1997.1.1
     040	..	a	Parr, Mary L.
     041	..	a	English
     050	11	a	AM139.R44
     050	22	b	78-15994
     245	..	a	Chemistry
     245	10	b	A workbook
</pre>

<p>
Indicator values are taken from the first non-blank indicators encountered for a field. In the example, the indicators "11" for the 050 field would be used, but the indicators on the next line would be ignored. For the 245, the indicators used would be "10".
</p>

<p>
The first field for each record must be the 001 field. The last line MUST have a return/linefeed. For all other fields, the fields in the MARC records are created in the same order as in the input file (normally MARC fields are in ascending numeric order). 
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>
In Interactive Mode you will may convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the "Convert" button or the File/Convert menu item (they do exactly the same thing). (If you are viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</P>

<P>After you click the Convert button or menu item, a "dialog box" appears for you to specify filenames. When the dialog box appears, it shows the filenames from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the Browse button -- when you select an input file, default names for all the other files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</P>

<P>You may also specify the number of records to convert. This can be helpful when you are testing. Use 0 (zero) when you want to convert the entire file. (Note: the "Maximum records" setting in the <a href="#settings">Settings dialog</a> does not apply to conversion.)
</P>


<p>
Click the "OK" button when you are ready to start the conversion. When the run finishes, click the "OK" button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>
In Command Mode, you may convert a file by using a command from the Windows command prompt.
</p>

<p>
We assume you are familiar with using the command window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".
</p>

<P>
To run MARConvert in command mode, follow these steps:
</p>

<ol>
<li>Open a command prompt window in Windows.</li>
<li>Enter the name of the program followed by one to three parameters, separated by spaces, as follows:
<ol>
<li>The name of the text file exported from your system</li>
<li>(Optional) "yes" if the command window should close automatically after the run; "no" is assumed if this parameter is missing</li>
<li>(Optional) "yes" if the output and report files can be overwritten without warning; "no" is assumed if this parameter is missing</li>
</ol>

<li>The program will open a new window to display its messages. When the program is done, press the <Enter> key to close this window. (The same information will be found in the report file, so there is no need to copy it.)</li>
</ol>

<p class=helpnote>
You must use the second parameter if you want to use the third parameter; use "no" for the second parameter if you do not want the command window to close after the run. See the third example below.
</p>

<p>
The name of the program, as well as any or all of the filename parameters, may contain path information if necessary. The program name and input filename can contain spaces if required, but if so, enclose them in quotation marks. No part of the command is case-sensitive.
</p>

<p>
Examples: 
</p>

<pre>
    MARConvert c:\mydata\anydata.txt
    MARConvert c:\mydata\anydata.txt yes
    MARConvert c:\mydata\anydata.txt no yes
    "c:\program files\marconvert\MARConvert" "c:\program files\marconvert\anydata.txt" yes yes
</pre>

<p>
All output files will have the same name as the input file with different extensions: "MRC" for the MARC file, "BAD" for the "bad record" file, and "RPT" for the report. The output files will be created in the same directory as the input file.
</p>

<p>
If the output or report file exist, you will be asked to confirm with a "y" if you wish to overwrite them, unless you used "yes" as the third parameter.
</p>

<p class=helpnote>
Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='charconv'></a>
<h2>Character Conversion</h2>

<p>
MARConvert for RRR expects records in the Latin-1 character encoding, and converts them to the MARC-8 encoding in the output file.
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows folder. This file is shared with our MARCView product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>





<br><p><br>
<hr>
<a name="convert"></a>
<h1 align=center>Client SSS</h1>

<a name='import'></a>
<h2>Overview</h2>

<P>
This customized version of MARConvert for SSS converts Ovid Medline records to MARC. (In MARConvert terminology, this is an "import" into the MARC format.)
</P>

<P>
The conversion is controlled by a text file called a <B>Translation Table</B>. MARConvert uses the information in this file to convert each field from the Ovid format to MARC format. We have supplied a Translation Table for the initial data; you may modify it to meet changing circumstances. Note that the Translation Table can control only some functions; complex changes may have to be made in the program code by Systems Planning.
</P>

<P>The conversion process results in three files:</P>

<UL>
<LI>A file of good MARC records</LI>
<LI>A file of bad records, also in MARC format. Bad records are those that contain fields not found in the Translation Table or have some other data problem.</LI>
<LI>A report file which can be viewed or printed. Look at the report after importing to see if there were any problems.</LI></UL>

<P>
Typically all three files are given the same name, with the file of good MARC records having the extension "MRC", the bad records "BAD", and the report "RPT". You may override these filenames and extensions when you perform an import.
</P>

<p>
There is no limit to the number of records in the input file. However, the output file is limited to 2 GB (2,147,483,647 bytes). The input file is not affected in any way by converting it.
</P>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='input'></a>
<h2>Input Files</h2>

<p>
The input files are in the following format: lines alternate between fieldnames and data, except that the "MeSH Subject Headings" and "General Note" fields may have multiple data lines representing multiple headings; all other fields must have all data on a single line (that is, with no returns or line-feeds in the data). Each record starts with a sequence number in angle brackets; this number is only the sequence number of the result set, and has no permanent meaning, so it is disregarded. <b>Example</b>:
</p>

<dir>
<pre>
<1>
Unique Identifier
  15888464
Authors
  Brook I.  Gober AE.
Institution
  Department of Pediatrics, Georgetown University School of Medicine, Washington DC 20016, USA. ib6@georgetown.edu
Title
  Long-term effects on the nasopharyngeal flora ... [truncated in this Help document]
Source
  Journal of Medical Microbiology.  54(Pt 6):553-6, 2005 Jun.
MeSH Subject Headings
    Acute Disease
    *Amoxicillin-Potassium Clavulanate Combination/tu [Therapeutic Use]
    *Anti-Bacterial Agents/tu [Therapeutic Use]
    *Cephalosporins/tu [Therapeutic Use]
    Child
    Colony Count, Microbial
    Comparative Study
    *Haemophilus influenzae/ip [Isolation & Purification]
    Humans
    *Moraxella (Branhamella) catarrhalis/ip [Isolation & Purification]
    *Nasopharynx/mi [Microbiology]
    *Otitis Media/dt [Drug Therapy]
    Otitis Media/mi [Microbiology]
    *Streptococcus pneumoniae/ip [Isolation & Purification]
    Time Factors
Abstract
  The effect on the nasopharyngeal bacterial flora ... [truncated in this Help document]


<2>
Unique Identifier
  15612839
[etc.]
</pre>
</dir>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>



<a name='interactive'></a>
<h2>Conversion: Interactive Mode</h2>

<p>
In Interactive Mode you convert a file using a typical Windows interface. To perform a conversion in Interactive Mode, run MARConvert, then click on the "Convert" button or the File/Convert menu item (they do exactly the same thing). (If you are viewing a file in MARConvert, it will close, since certain resources are needed by the conversion process. Simply reopen the file when the conversion is finished.)</P>

<P>
After you click the Convert button or menu item, a "dialog box" appears for you to specify filenames. When the dialog box appears, it shows the filenames from the previous run -- this makes it easy to rerun the previous file. To enter a new filename to convert, the easiest way is to click the Browse button -- when you select an input file, default names for the output files will be constructed for you. You can change any names or directories you wish. All names and directories are saved for the next run.</P>

<p>
When you install MARConvert, the default Translation Table Medline.mci is installed with the program (normally in C:\Program Files\MARConvert). You may move or copy it to another location if that is more convenient.
</p>

<P>
You may also specify the number of records to convert. This can be helpful when you are testing changes to the Translation Table. Use 0 (zero) when you want to convert the entire file. There is no limit to the number of records you can convert; the "Maximum records" setting in the <a href="#settings">Settings dialog</a> does not apply to conversion. However, the output files are limited to 2 GB (2,147,483,647 bytes). 
</P>

<p>
Click the "OK" button when you are ready to start the conversion. When the run finishes, click the "OK" button in the message box. You may then close MARConvert or leave it open for converting or viewing other files.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='command'></a>
<h2>Conversion: Command Mode</h2>

<p>
In Command Mode, you may convert one or many files in the same operation. This saves time when there are many files to convert.
</p>

<p>
To use the Command Mode, you must first create a <b>Command File</b>. This file tells MARConvert what MARC files to convert, as well as giving other information. Here is an example of a Command File (included in the installation as a model):
</p>

<dir>
<pre>
MARConvert command file
overwrite	yes
report		"d:\marc\marconvert\SSS\MARConvert.rpt"
"d:\marc\marconvert\SSS\medline records.txt"
"d:\marc\marconvert\SSS\citation samples.txt"
</pre>
</dir>

<p>
Notes on the example:
</p>

<ol>
<li>The first line is ignored by the program. You may use it to describe the purpose of the file or for other notes.</li>
<li>The second line says that the program should overwrite output files and the report file, if they already exist. If you change the word "yes" to "no", then the program will ask you before overwriting any files. (Obviously this prevents the program from processing multiple files automatically, which is the main purpose of the Command Mode.) Use one or more <b>tab characters</b> (not spaces) to separate the word "overwrite" from the "yes" or "no".</li>
<li>The third line says where the report file should be created and what it should be called. Use one or more <b>tab characters</b> (not spaces) to separate the word "report" from the path and filename you wish to use. In the example, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
<li>The fourth and subsequent lines list the Ovid files to be converted. As with the report file name, we put the path and filename in quotation marks; this is required if there are spaces in the path or filename, so it is good practice to use them always.</li>
<li>
The output files will be created in the directory the input files are in, and will be given the same name as the input file but with the extension "MRC"; the files of bad records will have the same name as the input file but with the extension "BAD". The output files for the example above will be named "medline records.MRC", "medline records.BAD", "citation samples.MRC", and "citation samples.BAD".
</li>
<li>
Command Mode uses the Translation Table specified in the MARCView.ini file -- that is, the one used in the last Interactive-Mode session.
</li>
</ol>


<p>
Command Mode is used as follows:
</p>

<ol>
<li>Open a Windows "Command Prompt" window. In Windows 2000 this is done by selecting Start/Programs/Accessories/Command Prompt. (We assume you are familiar with using the prompt window in Windows, and with the Windows path function. These vary slightly depending on your version of Windows, so it is not possible to include a comprehensive tutorial in this document. If you need more information, search the Web for "dos path reference".)</li>
<li>You will now enter at the command prompt two items, separated by a space. The first item specifies the MARConvert program. The second item identifies the Command File. Here is an example, assuming that MARConvert was installed in the default location: 
<pre>
     "c:\program files\marconvert\marconvert" command_file.txt
</pre>
Here again the path and filename are within double quotation marks, because of the space after the word "program".
<br>
<br>
This example also assumes that the Command File was named "command_file.txt" and exists in the "current directory". If the file "command_file.txt" is not in the directory from which you are entering the prompt, you may specify its path also, as in 
<pre>
     "c:\program files\marconvert\marconvert" "d:\mydata\command_file.txt"
</pre>
Or you may decide to use a different name for the Command File; if so just specify it as in this example:
<pre>
     "c:\program files\marconvert\marconvert" "d:\mydata\conversion_list.txt"
</pre>
</li>
<li>After entering these two items at the prompt, press the Enter key. When MARConvert begins to run, it will open a new window and show you its progress. When prompted, press the enter key again to close this window.</li>
</ol>

<p class=helpnote>
Tip: You can add the MARConvert directory to the Windows path and thus simplify the command.
</p>


	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='table'></a>
<h2>Translation Table</h2>

<P>
The Translation Table can be edited with Notepad. The Table consists of three columns separated with tabs. You may use as many tabs as you like to line up the columns for legibility.
</P>


<dir>
<P><b>Translation Table as delivered</b></P>
<pre>
MARConvert Translation Table
LDR		"LLLLLnam  22BBBBB   4500"
001		Unique Identifier
008&1		"DDDDDD"
008&2		Year of Publication	"s+++"
008&3		"    |||||||f|    |||| |||||  "
024-		"8 "
024a		Unique Identifier	
0242		"Medline"
100-		"1 "
100a		Authors
100u		Institution
245-		"10"
245a		Title
260a		Source
520a		Abstract
583-		"1 "
583a		Publication Type
650-		"12"
650a		MeSH Subject Headings
700-		"1 "
700a		MC_700a
780-		"01"
780a		Comments
785-		"01"
785a		MC_785a
856-		"41"
856u		Unique Identifier	"http://removed.search.serialssolutions.com
                                                    /?sid=SSS&id=pmid:+++"
000		Abbreviated Source
000		Authors Full Name
000		CAS Registry/EC Number/Name of Substance
000		Country of Publication
000		Entry Date
000		Gene Symbol
000		General Note
000		Grant Number
000		ISSN
000		Journal Subset
000		Keyword Heading
000		Language
000		Local Messages
000		NLM Journal Code
000		Publishing Model
000		Publishing Type
000		Record Owner
000		Revision Date
000		Update Date
//
</pre>
</dir>

<p>The first line of the Translation Table is ignored. You may record comments here.</p>


<h3>Field mapping</h3>

<p>
Beginning on the second line, the Translation Table consists of one line for each part of the MARC records to be created. Each line may represent an entire MARC field, a byte-range of a fixed field, or one or more subfields of a variable field. In addition, a field or subfield may have more than one <b>Segment</b>.
</p>

<p>
Each line in the Translation Table requires two columns: the description of the part of the MARC record, and the source of data for that part. There is an optional third column for <b>Attachments</b> (see below) that are to be added.
</p>

<ul>
<LI>The first column specifies the MARC fields that are to be imported. You should specify subfield codes for MARC Variable Fields (010 - 999). Use a hyphen (-) to mean "indicators," as shown above for the 024 field. You may also import the MARC indicators as an Attachment to be appended to the first subfield.</li>

<li>
<b>Segments</b> are represented by following the field tag or subfield code with an ampersand and a digit. Segments must be in the Translation Table in ascending order. The Translation Table shown above uses three Segments for the 008 field. (In first segment, MARConvert will replace the DDDDDD with the Date Entered on File.)
</li>

<LI>
The second column shows the Medline field used to create that part of the MARC record. In the example above, the 001 field will be produced from the Medline Unique Identifier field.
</li>

<li>
Alternatively, column two may contain a <b>Constant</b>, as in the LDR field in the example. Constants must be within double quotation marks. (In the Leader, MARConvert automatically replaces the LLLLL and BBBBB with the Logical Record Length and Base Address of Data.)
</li>

<li>
Using Segments, any number of Medline fields and Constants can be used to generate a MARC field.
</li>

<li>
The third column specifies optional <b>Attachments</b> to be appended to the Medline data. For example, the line for 856u specifies that the Unique Identifier is to be preceded by the URL shown. The "+++" in the Attachment indicates where the variable data from the Medline field is to be inserted.
You can put the "+++" anywhere in an Attachment to show that the variable data should be inserted into the Attachment.
</li>

<LI>The Translation Table must end with a line whose first character is a slash.</LI>

<li>Fields in the MARC records are created in the order they are listed in the Translation Table. Normally MARC fields are in ascending numeric order.</li>
</UL>


<a name='iinds'></a>
<h3>More on Indicators</h3>

<P>Indicators can be added either as <b>Variables</b>, <b>Constants</b>, or <b>Attachments</b>, depending on your needs. If indicators are Variables, you supply the indicators for each record in the import file; as Constants they will be the same in every record; as Attachments, they can depend on the data.</P>


<UL>
<LI>To add indicators as a <b>Variable</b>, add them as the first subfield for the field.</LI></UL>

<DIR>
<DIR>

<pre>
245-&#9;Nonfiling_characters
245a&#9;TI
</pre>

</DIR>
</DIR>


<UL>
<LI>To add indicators as a <b>Constant</b>, use - (hyphen) as the first subfield code for the field.</LI></UL>

<DIR>
<DIR>

<pre>
650-&#9;"10"
650a&#9;SUBJ
</pre>

</DIR>
</DIR>


<UL>
<LI>To add indicators as an <b>Attachment</b>, add them to the first subfield code for the field, preceding them with a - (hyphen).</LI></UL>

<DIR>
<DIR>

<pre>
100a&#9;AU&#9;"-1 +++"
</pre>

</DIR>
</DIR>



<a name='iignore'></a>
<h3>Ignoring fields</h3>

<P>Field labels that are encountered in the import file that are not found in the Translation Table are reported in the output report. This is your way to verify whether you have considered all fields in building the Translation Table. </P>

<P>However, there may be times when you wish to purposely ignore certain fields. To show that this is intentional, add a line to the Translation Table assigning the field label to the MARC tag 000, like this:</P>

<DIR>
<DIR>

<pre>
000&#9;ADD_DT
</pre>

</DIR>
</DIR>


<a name='ival'></a>
<h3>Validation</h3>

<P>MARConvert does not perform any validation on the MARC records it creates. For example, it does not check to make sure that each record has the mandatory MARC fields, or even a Leader or 008. (We plan to add such checks in future.)</P>

<P>We would be happy to implement your requirements for custom validation on a fee basis, such as converting codes from lookup tables or adding custom diacritical conversion.</P>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='charconv'></a>
<h2>Character Conversion</h2>

<p>
Ovid appears to strip diacritics from the input records. In case this policy changes, MARConvert for SSS converts Latin-1 characters to the MARC-8 character encoding in the output file.
</p>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='special'></a>
<h2>Special Processing</h2>

<p>
MARConvert processes the data as found, with the exceptions described here. Sometimes the Medline files lacked sufficient information to generate the precise values in the MARC records, especially the Leader and 008 fields; in this case we used the most common settings. An example of this is that we had to assume that all records are monographs, resulting in a first Leader byte 7 of "m". If there are problems resulting from these assumptions, we would be happy to discuss detailed parsing of certain fields in order to provide more accurate results.
</p>

<p>
The "Authors" field is split on periods, with the first author going into the 100 field, and the others into 700 fields.
</p>

<p>
For the "MeSH Subject Headings" field, only headings preceded by an asterisk are converted. Subdivision codes (preceded by a slash) are dropped. Subdivision terms (in square brackets) are converted to a subfield x. For example: <font face="courier"><b>*Skin/in [Injuries]</b></font> becomes <font face="courier"><b>$aSkin$xInjuries</b></font>.
</p>

<p>
The "Title" field may have one or more phrases added in square brackets. If the phrase contains "comment", it and its brackets are removed. If the phrase contains " refs", it and its brackets are removed. Otherwise the phrase is moved to a 245$k without the brackets.
</p>

<p>
If the "Comments" field contains the phrase "Comment on:" its contents are placed in a 780$a. Otherwise its contents are placed in a 785$a.
</p>

<p>
The "Year of Publication" field goes into the 008/7-10, preceded by "s". If there is no Year of Publication, the 008/6-10 is filled with "nuuuu".
</p>



	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>


<a name='inifile'></a>
<h2>MARCView.ini</h2>

<p>
Many of the settings for MARConvert are stored in a file called MARCView.ini in your Windows folder. This file is shared with our MARCView&#8482; product, should you have both. This is because you may use the two products alternately, and would expect the settings to be shared. 
</p>

	<center>
	<a href="#contents"><small><small>Return to Table of Contents</small></small></a>
	</center>





<hr>
<h1 align=center>Part II: File Viewing, Searching, and Printing</h1>
<a name='viewing'></a>
<h2>Viewing a File</h2>
<p>Begin by clicking the <a href='#open'>View</a> button and choosing a file of MARC records to view. Or click the File menu to see recently viewed files.</p>
<p>If a file is already open, it is closed and the new one you select is opened.</p>
<p>If the file contains MARC records, the first record is shown. The viewing area of MARCView is divided into two panes. On the left is a scrollable Navigation Grid. Click on a record to make it display in the right pane, or use the keyboard arrow keys to move up or down the Grid and display each record as it is selected.</p>
<p>The right pane shows the MARC record in an easy-to-read format. The record is color-coded to make it easier to read: subfield delimiters and subfield codes are blue and escape sequences (indicating a shift to another character set) are green. Characters that are not in the Latin-1 character set are displayed as green squares. (Although MARCView can read the entire MARC-8 and UTF-8 character sets, it can display only those characters with a Latin-1 equivalent. Use the <a href=#hex>Hexview</a> feature to see the record in its original character set.)</p>
<p>Besides the Navigation Grid, you can also navigate with the <a href='#first'>First</a>, <a href='#prev'>Prev</a>, <a href='#next'>Next</a>, and <a href='#last'>Last</a> buttons. Or use <a href='#goto'>Goto</a> to go to a specific record. Or use <a href='#find'>Find</a> to find a word or phrase in the records.</p>
<p>(Because of the delay and memory needed, the maximum number of records per file and the maximum number for which to build a Navigation Grid are user <a href='#settings'>Settings</a>.)</p>
<h3>Changing the display</h3>
<P>You may change the size of the two panes to suit your needs. Move the mouse pointer to the vertical bar between the two header lines at the top of the panes. When the cursor changes to two vertical bars with arrows pointing right and left, click and drag the cursor to the right or left. You may drag the cursor all the way to one side if you just want to see a single pane. To see both panes again, move the cursor to the edge of the header line and drag back to the center.</P>
<P>Likewise you can change the width of the columns in the Navigation Grid by dragging the vertical dividers in its header bar.</P>
<p class=helpnote>Note: It is possible to drag a column wider than the Navigation Grid, so that you can no longer see the right-hand divider in the header bar to make it narrower. If this happens, either widen the whole Navigation Grid until the wide column is fully displayed, or close and restart MARCView.</p>
<P>All these positions are stored when you exit, and used the next time you start MARCView.</P>
<P>If you move, resize, or maximize MARCView, the new size and position are saved and used the next time you start MARCView.</P>
<P>Other preferences are set using the <a href='#settings'>Settings</a> menu item.</P>
<h3>Opening multiple files</h3>
<p>Although MARCView cannot open more than one file at once, you can run MARCView several times at once and view multiple files that way.</p>
<h3>Closing a file</h3>
<p>There is no need to close a file. Files are automatically closed when you open a new one, or when you exit MARCView.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='copying'></a>
<h2>Copying Data</h2>
<p>MARCView allows you to copy part or all of a displayed MARC record into another application. What is copied is exactly what you see, not the underlying MARC record.</p>
<p>You cannot paste text <b>into</b> a MARCView display, because the displayed records are read-only. In other words, MARCView cannot be used to edit records.</p>
<ol>
<li>Depress the left mouse button and drag the mouse over the data to be copied. The selected text will be highlighted.</li>
<li>Use one of the following to copy the selected text:
<ul>
<li>Click the Edit/Copy menu item</li>
<li>Press Ctrl-Insert on the keyboard</li>
<li>Press Ctrl-C on the keyboard</li>
</ul></li>
<li>Move to the application where you want to paste the text, and put the cursor where the text is to go.</li>
<li>Use one of the following to paste the selected text into the other application:
<ul>
<li>Click the Edit/Paste menu item</li>
<li>Press Shift-Insert on the keyboard</li>
<li>Press Ctrl-V on the keyboard</li>
</ul></li>
</ol>
<div class=gotop><a href="#contents">Top</a></div>
<a name='sendto'></a>
<h2>Quick Access to MARCView Using Send-To</h2>
<p class=helpnote>Sorry, not available in MARConvert.</p>
<P>You may right-click on any MARC file and open it with MARCView. To use this function in <b>Windows 98</b>, open Windows Explorer to the folder \Windows\SendTo. Then right-click in a blank area of the right pane, and choose New/Shortcut. When the Create Shortcut dialog opens, click the Browse button, then navigate to \Program Files\MARCView. Double-click the filename MARCView.EXE, then click the Next button. You will probably want to change the name of the shortcut to &quot;MARCView&quot;.</P>
<p>With <b>Windows 2000</b>, you need to find the SendTo folder under your user name in C:\Documents and Settings. Since the SendTo folder is normally hidden, you first have to click on the Tools/Folder Options/ in Windows Explorer. Then click the View tab and click "Show hidden files and folders" about six lines down. Close this, then click on the SendTo folder for your user name. Then right-click in a blank area of the right pane, and choose New/Shortcut. When the Create Shortcut dialog opens, click the Browse button, then navigate to \Program Files\MARCView. Double-click the filename MARCView.EXE, then click the Next button. You will probably want to change the name of the shortcut to &quot;MARCView&quot;.</p>
<P>From now on, you can right-click on any file, move the cursor to SendTo, then choose MARCView. MARCView will attempt to open any file sent this way, whether a MARC file or not.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='charset'></a>
<h2>Display of Diacritics and Special Characters</h2>
<p>There are several reasons why characters may appear to be missing or display incorrectly.</p>
<ul>
<li>The records may be in a different character encoding from what MARCView is expecting. Use the <a href=#settings>Settings</a> dialog to change this.</li>
<li>MARCView uses the Latin-1 character set for display. If a character in the record has no Latin-1 equivalent, it will be displayed as a close approximation if any; otherwise as a green square.</li>
<li>For records with <b>MARC-8</b> encoding, characters not in the English alphabet may be stored in MARC records as one, two, or more bytes (more than two bytes if there are multiple diacritics for the letter). MARCView handles these as follows:</li>
<UL>
<li>Single-byte characters: If available in the Windows Latin-1 character set, the character is shown in its correct form (for example &AElig;). Otherwise the closest equivalent in the English alphabet is shown (for example, hook-o is displayed as "o").</li>
<li>Two-byte characters: If the two-byte character is in the Windows Latin-1 character set, it is displayed. Otherwise the unaccented letter represented by the second byte is shown. However, if second byte represents a letter not in the English alphabet, both diacritics are ignored. (For example, if a diacritic modifies a hook-o, both the diacritic and the hook are ignored and the character is displayed as "o".)</li>
<li>More-than-two-byte characters: Only the last diacritic before the character is used; this diacritic and the character are then treated like two-byte characters.</li>
</UL>
<li>For records with <b>UTF-8</b> encoding, MARCView can display UTF-8 accented characters only when the accent and the character are in the same UTF-8 symbol. For example, the single character representing an "e" with acute accent can be displayed correctly, but if the "e" and the acute were in separate characters, only the "e" will be displayed. (The acute will be displayed as a green square because it is not a separate Latin-1 character.)</li>
</ul>
<p>If characters are not displaying, try using the <a href='#hex'>Hexview</a> feature to see the record's raw data.</p>
<p class=helpnote>This discussion refers to display only; the input file is not changed in any way.</p>
<p>(For searching records with diacritics and non-latin characters, see <a href='#find'>Find</a>.)</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='support'></a>
<h2>User Support</h2>
<p>Please see the <a href='http://www.systemsplanning.com/marc/support.asp'>MARCView Support Page</a> for information.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='technical'></a>
<h2>Technical Details</h2>
<h3>File format</h3>
<p>MARCView can read some files that do not adhere strictly to the MARC standard. Because we assume you want to see the data rather than messages, MARCView does not inform you of such discrepancies.</p>
<h3>Format integration</h3>
<P>MARCView has been tested with the Library of Congress' format integration test file (FORMAT_I.TES) and had no difficulties with this file. Of course, the older file formats can be read also.</P>
<h3>Highlighting terms found in searches</h3>
<P>MARCView's default is to show terms found in searches in red. However, this is possible only if the term as displayed is identical to the term as searched for. Because a search is done without diacritics, the word Ren&eacute; cannot be highlighted after a search for Rene. Likewise, search terms that include tags cannot be highlighted because the tag is not displayed adjacent to the term. If this discrepancy bothers you, turn off highlighting using the <a href='#settings'>Settings</a> menu item.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='uninstall'></a>
<h2>Uninstalling MARCView</h2>
<p>To uninstall MARCView use the Add/Remove Programs icon in Windows Control Panel. Click on "MARCView" or "Systems Planning MARCView", then click the Add/Remove button.</P></p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='open'></a>
<h2>View</h2>
<P>To open a file for viewing, use either the View button or the File/View menu item. If a file is already open, it is closed and the new one you select is opened. In other words, you may open a new file without restarting MARCView. There is no need to close a file; files are closed when you open another file, or when you exit MARCView.</p>
<p>Using either the button or the menu, the last-used directory is shown. In addition, the File menu displays the nine most recently used files; select a recently used file to open it.</p>
<P>When you open a file, the filename is centered below the buttons. On the right below the buttons is a record counter that shows what record you are viewing and how many there are in the file.</P>
<P>MARCView can handle any number of records in a file. The default is 100,000. Use the <a href='#settings'>Settings</a> menu item to change this setting. MARCView has been tested with files of 1 million records. MARCView may require a minute or more tovalidate and analyze a large file. However, this means that navigating to any record in the file is instantaneous.</P>
<p>By default, when the Open dialog appears, you see all files in the directory. To see a subset, enter a mask in the "File name" box (NOT the "Files of type" box). For example, to see files with an extension of MRC, enter *.MRC in the "File name" box.</p>
<P>After the file is open, you will see the first record on the right. On the left is a scrollable Navigation Grid. Click on a record to make it display in the right pane. Note that because of the delay and memory needed, the maximum number of records for which to build a Navigation Grid is a user <a href='#settings'>Setting</a>.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='first'></a>
<h2>First</h2>
<P>Displays the first record in the file.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='prev'></a>
<h2>Previous</h2>
<P>Displays the previous record in the file.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='next'></a>
<h2>Next</h2>
<P>Displays the next record in the file.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='last'></a>
<h2>Last</h2>
<P>Displays the last record in the file.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='goto'></a>
<h2>Goto</h2>
<P>Displays the record identified by its sequence number in the file. The dialog box will allow you to type numbers only.</P>
<P>If you type a number higher than the number of records in the file, the Goto command is ignored.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='find'></a>
<h2>Find</h2>
<P>Allows you to search for a word or phrase in the MARC records.</P>
<p>Enter a term in the first box and click OK to perform a search. Do not use quotation marks.</p>
<ul>
<li>Click the "Case-sensitive search term" checkbox if terms should be found using the same case as your search term.</li>
<li>To restrict a search to a specific MARC three-character tag (including control fields like 008), enter the tag in the second box. The tag may include the "x" wildcard character in any position or positions. You may search the Leader by using "Ldr" as the tag. You may search Leader bytes by using as the tag the three-letter code that MARCView uses to display the bytes, such as "Len" for record length, "Sta" for record status, etc. Tags are NOT case-sensitive.</li>
<li>To restrict a search a specific subfield, enter the subfield code in the third box (do not include a $ or any other character, just the one-character code). Subfield codes ARE case-sensitive. You need not specify a tag when you specify a subfield, so you can search for "new york" in all b subfields in the file.</li>
<li>To restrict a search to fields with certain indicator values, enter them in the appropriate boxes. Use the fourth box for indicator 1, the fifth box for indicator 2. Use a single character in each box. <b>If you do not specify a tag also, the indicator values will be ignored.</b></li>
<li>Click the "Start search with first record" checkbox to begin the search at the beginning of the file instead of at the current record (searching is always forward).</li>
</ul>
<p>The tag and subfield-code boxes can be used without a search term; that is, you can search for the presence of fields or subfields regardless of their contents. You may also specify indicator values for these searches if you specify a tag.</p>
<p>Data in the Leader is not searched unless you specify "Ldr" as the tag. To search for data in the entire Leader, enter "Ldr" into the box for the tag. You may also search for data in any of the Leader values that MARCView displays by using its three-letter display code. For example, to search in Record Status, use "Sta" as the tag.</p>
<p>The MARC Record Directory is not searchable. MARC Escape Sequences are not searchable.</p>
<h3>Interrupting a search</h3>
<P>To interrupt a search, hold down the Ctrl key until the search stops. The Ctrl key is checked each 100 records.</P>
<h3>Normalization</h3>
<p>MARCView does NOT normalize your search term or the MARC records in order to match, except for capitalization (at your request) and diacritics (see next section). In other words, you must use the spacing and punctuation that you expect to find in the MARC records. "Tokyo, Japan" does NOT match "Tokyo Japan".</p>
<h3>Diacritics and non-latin characters</h3>
<p>MARCView can find words containing diacritical marks. To search for these, enter the letters of the word without the diacritics. For example, enter "R&egrave;n&eacute;" as "Rene". Do not enter any symbols that are not accented letters, such as musical flat and sharp, &copy;, &reg;, &trade;, &#xb1;, etc.</p>
<P>Single-byte characters are translated for searching as shown below.</P>
<table class=embedded border cellspacing=0 cellpadding=6>
<tr>
<th>name</th>
<th>character</th>
<th>enter as</th>
</tr>
<tr>
<td>Polish L</td>
<td>&#321;, &#322;</td>
<td>L, l</td>
</tr>
<tr>
<td>slash O</td>
<td>&Oslash;, &oslash;</td>
<td>O, o</td>
</tr>
<tr>
<td>crossbar D</td>
<td>&#272;, &#273;</td>
<td>D, d</td>
</tr>
<tr>
<td>thorn</td>
<td>&THORN;, &thorn;</td>
<td>TH, th</td>
</tr>
<tr>
<td>ligature AE</td>
<td>&AElig;, &aelig;</td>
<td>AE, ae</td>
</tr>
<tr>
<td>ligature OE</td>
<td>Œ, œ</td>
<td>OE, oe</td>
</tr>
<tr>
<td>hook O</td>
<td>&#416;, &#417;</td>
<td>O, o</td>
</tr>
<tr>
<td>hook U</td>
<td>&#431;, &#432;</td>
<td>U, u</td>
</tr>
<tr>
<td>Turkish i</td>
<td>&#305;</td>
<td>i</td>
</tr>
<tr>
<td>eth</td>
<td>&eth;</td>
<td>d</td>
</tr>
</table>
<p>Although MARCView can read the entire MARC-8 and UTF-8 character sets, it can search and display only those characters with a Latin-1 equivalent. Other characters are displayed as green squares, <i><b>but these are ignored in searching</b></i>. In other words, if you see a string of character such as</p>
<pre>     use the <font color=green>&#x25af;</font>-square function</pre>
<p>you would search for this as</p>
<pre>     use the -square function</pre>
<p>Subscript and superscript characters should be entered as if they were normal characters. That is, to search for "H&#8322;O" search for "H2O".</p>
<p>For display of diacritics and non-latin characters, see <a href='#charset'>Display of Diacritics and Special Characters</a>.</p>
<h3>Search results</h3>
<p>Usually when MARCView finds a record matching your search, it will display the record with the search term highlighted in red; it is highlighted wherever it occurs in the record, even if you restricted your search to a particular field or subfield. In some cases highlighting is not possible, and MARCView will display the message "Found but cannot highlight". The search term is highlighted in the record wherever it is found, even in fields you may not have searched on; if this bothers you, you can turn off highlighting using the <a href="#settings">Settings</a> menu item.</p>
<h3>Including the subfield delimiter in a search</h3>
<p>You may include the subfield delimiter as part of the search term if your computer supports the keyboard command required. For example, you may search for "Donizetti, Gaetano,&#x2021;d1797-1848" by entering that exact string. To enter the subfield delimiter, make sure your NumLock key is on, then hold down the Alt key and press 031 on the <u>numeric keypad</u>, then release the Alt key. The subfield delimiter will display as a rectangle. Do not use the dollar sign or the double dagger to represent the subfield delimiter.</p>
<p>You may search multiple subfields with this method, such as by using the string "&#x2021;aDonizetti, Gaetano,&#x2021;d1797-1848". You may restrict such searches to a specific tag in the usual manner.</p>
<P>If the subfield code is included in this way, do not also add it to the third entry box or MARCView will search only a single subfield.</P>
<P>Some computers will not insert a character for Alt-031. We have no solution for this problem.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='again'><h2>Again</h2></a>
<P>Once you have done a search with the Find button, you can search forward for the same search specifications by clicking on the Again button.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='hex'></a>
<h2>Hexadecimal and ASCII view</h2>
<p>MARCView can show you the unmodified MARC record from the input file. This "Hexview" shows the record currently in the right-hand pane of the main window -- so you must have a file of MARC records open to use Hexview.</p>
<p>To see the Hexview, simply click the "Hex" button. A new window opens with the current record shown in both hexadecimal and ASCII formats. By changing the width of the Hexview window, you can cause the record to wrap according to your needs.</p>
<P>The record is color-coded to help you read it: subfield delimiters and subfield codes are blue, field terminators are red, record terminators are white on red, and escape sequences (indicating a shift to another character set) are green. (For MARCXMLrecords the color scheme is different.)</P>
<P>If you click the mouse in a directory entry or in a data field (either hex or ASCII), the corresponding byte in both the ASCII and hexadecimal views will be highlighted in yellow. If you double-click in a directory entry, it will be highlighted, and so will the corresponding data field for that directory entry. Likewise, if you double-click in a data field, both it and its directory entry are highlighted. (Double-clicking has no effect in MARCXML records.)</P>
<p>In the Hexview, both displayed and printed, certain characters are replaced in the right column to make it easier to read. <ul><li>For <b>MARC21 and UNIMARC</b> records, subfield delimiters are replaced by <span style="color:blue">&Dagger;</span> and field terminators by <span style="color:red">&para;</span>.</li><li> For <b>MARCXML</b> records, tabs are replaced by <span style="background-color:lightgrey">T</span>, carriage returns by <span style="background-color:lightgrey">R</span>, and line-feeds by <span style="background-color:lightgrey">L</span>; all three of these are on a gray background so they are distinct from data.</li></ul></p>
<P>Each time you click the Hex button, a new window opens. This way you can compare several records in their native MARC formats.</P>
<P>When you exit MARCView, all Hexview windows are closed for you.</P>
<h3>Print button</h3>
<p>Click the <b>Print</b> button to print the current Hexview window. The same colors as described above are used in the print, including any yellow highlighting. The layout of the print is not affected by the width of the Hexview window.</p>
<h3>Close button</h3>
<p>Click the <b>Close</b> button to close the Hexview window.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='print'></a>
<h2>Printing</h2>
<h3>Print button</h3>
<p>The Print button prints the current record formatted as it is in the right-hand pane of MARCView.</p>
<h3>File/Print menu item</h3>
<p>The File/Print menu item offers three choices for printing:</p>
<ul>
<li>You may print the currently visible record, formatted as it is in the right-hand pane of MARCView. This is the same as clicking the Print button.</li>
<li>You may select a range of records to print. Click the "Range" radio button and fill in the record numbers to print (inclusive). Each record starts on a new page.</li>
<li>You may print the entire file. Each record starts on a new page.</li>
</ul>
<p>To interrupt a long print job you didn't intend, right-click the printer icon in your Windows task-tray (under the time-of-day) and select the printer. Then click the menu item Printer/Cancel All Documents. </p>
<p class=helpnote>The "Selection" radio button in the printer dialog does nothing.</p>
<p class=helpnote>We do not recommend you "Print To File" for large ranges or files. Because each MARC record is a separate document, you will be asked for a filename for each record.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='help'></a>
<h2>Help</h2>
<p>Opens your browser to the MARCView Help file on the Internet. You may save this file for off-line use if you choose. Note where the file is saved so you can bookmark the downloaded file in your browser.</p>
<p>Every time you click a Help button a new browser window is opened. We apologize for this, but there seems to be no way to detect that MARCView Help is already open in the browser.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='exit'></a>
<h2>Exit</h2>
<p>Closes the MARC file you are viewing and exits MARCView.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='fonts'></a>
<h2>Fonts</h2>
<p>When installed, MARCView uses the Windows Courier New font. You may change the size or use another font by selecting the menu item Tools/Fonts. You should choose a font that will display the Latin-1 character set. A standard font-selection dialog is shown.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='settings'></a>
<h2>Settings</h2>
<P>By clicking on the Settings menu item in the Tools menu, you can change how MARCView handes your files when opened for Viewing. These settings are saved when you exit MARCView and used the next time you start the program.</P>
<P>Some settings cannot take effect until you reopen the file you are viewing. Others require you to exit and restart MARCView. You will be shown a message to indicate which action is required.</P>
<h3>General Settings</h3>
<P><u>Non-numeric tags</U>. If checked, tags like u01 are allowed (these are used in RLIN acquisitions records). If not checked, tags must be numerics in the range 001 - 999.</P>
<P><u>Highlight terms found in search</U>. If checked, terms found in searching are shown in red. Since terms cannot always be highlighted (see <a href='#technical'>Technical details</a> for the reasons), highlighting can be turned off if you wish.</P>
<P><u>New line for each subfield</U>. If checked, each subfield begins on a new line of the display.</P>
<h3>Size Limits</h3>
<p class=helpnote>Do not use commas when entering these numbers.</p>
<P>You can set the maximum number of records that can be viewed (default is 100,000). If a file has more records than your setting, the first part of the file will still be viewable. (The reason for this setting is to accomodate systems with little memory.) There is no limit except your computer's resources to the number of records in a file, but the file size is limited to 2 gigabytes (2,147,483,647 bytes).</p>
<P>You can also set the number of records that will be examined to build the Navigation Grid in the left pane of the display. The default is 5,000, as building the Grid becomes slower as the number gets larger. The maximum number is 10,000 for two reasons: more than that is impossibly slow to build, and also a larger grid is practically unusable. Enter 0 if you do not wish the Navigation Grid to be built.</p>
<p>If you enter non-numeric characters into these fields, the defaults will be used.</p>
<h3>Character Set of Records</h3>
<p>(Does not apply to UNIMARC files. MARCView will automatically use the UNIMARC character set for UNIMARC files.)</p>
<p>(Does not apply to MARCXML files, which are assumed to be in UTF-8. Please contact us if this is a problem.)</p>
<p>Use these choices to tell MARCView what character set is used in the MARC records. If your accented records look wrong, your records may be using a different character set; change this setting and reopen the file.</p>
<ul>
<li>Read Leader (the default setting) means that MARCView should look at Leader byte 09 of each record, which says whether the record uses MARC-8 or UTF-8.</li>
<li>MARC-8 refers to the standard MARC character set, in which diacritics are stored as separate characters preceding the letters they modify.</li>
<li>Latin-1 refers to the ISO-standard character set in which a diacritic plus letter is a single character.</li>
<li>UTF-8 is a common representation of the Unicode character set that can represent all currently used languages and symbols in the world.</li>
</ul>
<P>We can provide support for other character sets on a contractual basis.</P>
<p>(For details on how characters are displayed, see <a href=#charset>Display of Diacritics and Special Characters</a>).</p>
<h3>Navigation Grid</h3>
<P>Use this section to describe what fields (and optional subfields) you want displayed in the Navigation Grid, and what the column headings should be. There are four sets of specifications so that you may have different selections for bibliographic andauthority records, and for MARC21 and UNIMARC formats.</p>
<p>You may enter a tag only or a tag and subfield code. MARCView shows a $ between the tag and the subfield code.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='statistics'></a>
<h2>File statistics</h2>
<P>MARCView can analyze the currently open file and display statistics about the records in the file.</P>
<UL>
<LI>For the file as a whole, the number of records, the file size, and the minimum, maximum, and mean record lengths are shown.</LI>
<LI>For the record Leaders, a count is produced of each distinct value in the file. (The # symbol represents the blank space.) Only digits 0 - 9, lower-case letters a - z, and the space are counted; other characters are ignored as they are not legal.</LI>
<LI>For the fields, each tag represented in the file is listed, along with its count and its minimum, maximum, and mean lengths.</LI>
</UL>
<P>To see the statistics, select the menu item Tools/Statistics from the MARCView menu. In a few seconds a window will open with the information (longer for large files). This window may be left open while you perform other functions in MARCView. You may move it if it is in your way. The Statistics window must be closed before you request to see statistics on a different file.</p>
<p>Statistics will be slower for MARCXML files, as there is no Record Directory to analyze. Instead, each field in each record must be read.</p>
<p class=helpnote>If the "Maximum records" setting (in the <a href="#settings">Settings</a> dialog) is less that the size of the file, the entire file is not read. This means that the Statistics shown apply only to the number of records read.</p>
<div class=gotop><a href="#contents">Top</a></div>
<a name='about'></a>
<h2>About MARCView</h2>
<P>Provides information about the program and about Systems Planning.</P>
<div class=gotop><a href="#contents">Top</a></div>
<a name='sql'></a>
<h2>SQL Search</h2>
<h3>Basics</h3>
<p>MARCView allows you to enter SQL commands to search the currently open file. This gives MARCView an advanced search capability with boolean and exact-match searching. Queries are derived from SQL syntax such as:</p>
<pre>     select * from brkrtest.mrc where 100='jones'<br>     select * from brkrtest.mrc where x00$a='jones, george'<br>     select * from brkrtest.mrc where 6xx='history'</pre>
<p>However, since entire records are retrieved, and the search is limited to the currently open file, the "select" and "from" clauses are irrelevant; only the "where" clause is necessary (without the word "where"). (Other SQL clauses such as "order by", "having", etc. are not currently implemented.)</p>
<p>Therefore the three searches above are entered simply as</p>
<pre>     100=jones<br>     x00$a='jones, george'<br>     6xx=history</pre>
<p>Notice that quotes around single words are not necessary unless you want to specify case-sensitivity (see below):</p>
<p>As shown in the second example, you may optionally enter a single subfield code preceded by a dollar sign. This restricts the search to the specified subfield. To search in multiple subfields, use this syntax:</p>
<pre>     650$a=history and 650$z=france</pre>
<p>However, this will not necessarily find these terms in the SAME 650 field, just in the same record.</p>
<p>The tag may include the "x" wildcard character in any position or positions. You may search the Leader by using "Ldr" as the tag. You may search Leader bytes by using as the tag the three-letter code that MARCView uses to display the bytes, such as "Len" for record length, "Sta" for record status, etc. Tags are NOT case-sensitive.</p>
<p>A SQL search stops when 10,000 hits have been found.</p>
<h3>Interrupting a search</h3>
<P>To interrupt a search, hold down the Ctrl key until the search stops. The Ctrl key is checked each 100 records.</P>
<h3>Normalization</h3>
<p>MARCView does NOT normalize your search term or the MARC records in order to match, except for capitalization (at your request) and diacritics (see next section). In other words, you must use the spacing and punctuation that you expect to find in the MARC records. "Tokyo, Japan" does NOT match "Tokyo Japan".</p>
<h3>Diacritics</h3>
<p>MARCView can find words containing diacritical marks. To search for these, enter the letters of the word without the diacritics. For example, enter "R&egrave;n&eacute;" as "Rene". Do not enter any symbols that are not accented letters, such as musical flat and sharp, &copy;, &reg;, &trade;, &#xb1;, etc.</p>
<P>Single-byte characters are translated for searching as shown below.</P>
<table class=embedded border cellspacing=0 cellpadding=6>
<tr>
<th>name</th>
<th>character</th>
<th>enter as</th>
</tr>
<tr>
<td>Polish L</td>
<td>&#321;, &#322;</td>
<td>L, l</td>
</tr>
<tr>
<td>slash O</td>
<td>&Oslash;, &oslash;</td>
<td>O, o</td>
</tr>
<tr>
<td>crossbar D</td>
<td>&#272;, &#273;</td>
<td>D, d</td>
</tr>
<tr>
<td>thorn</td>
<td>&THORN;, &thorn;</td>
<td>TH, th</td>
</tr>
<tr>
<td>ligature AE</td>
<td>&AElig;, &aelig;</td>
<td>AE, ae</td>
</tr>
<tr>
<td>ligature OE</td>
<td>Œ, œ</td>
<td>OE, oe</td>
</tr>
<tr>
<td>hook O</td>
<td>&#416;, &#417;</td>
<td>O, o</td>
</tr>
<tr>
<td>hook U</td>
<td>&#431;, &#432;</td>
<td>U, u</td>
</tr>
<tr>
<td>Turkish i</td>
<td>&#305;</td>
<td>i</td>
</tr>
<tr>
<td>eth</td>
<td>&eth;</td>
<td>d</td>
</tr>
</table>
<P>For display of diacritics, see <a href='#charset'>Display of Diacritics and Special Characters</a>.</P>
<h3>Search results</h3>
<p>After the search is performed, the set of records retrieved (the "result set") is shown in the Navigation Grid in the order the records appear in the file; the first record retrieved is displayed. You may use the Navigation Grid, the buttons, or the menu to navigate the result set.</p>
<p class=helpnote>If the Navigation Grid is not showing, you may have at some previous time dragged it to zero-width. To restore it, move the mouse to the vertical bar on the extreme left of the box that says "MARC Record". When the cursor changes to arrows pointing left and right, click and drag the vertical bar to the right.</p>
<p>While you are viewing the results of a SQL search, the Goto and Find functions are disabled.</p>
<p>To restore navigation of the full file, use the menu command Navigate/SQL Reset (or reopen the file, which takes longer).</p>
<p>The nine most recent queries are saved so you can re-issue them or edit them. Selecting one puts it into the textbox to be edited or reused. </p>
<h3>"Like" and exact-match searches</h3>
<p>By default, searches are "like" searches -- the words or phrases to be found may be anywhere in the field/subfield specified; in other words the first example is performed as if it were:</p>
<pre>     100 like '%jones%'</pre>
<p>To override this behavior, use == instead of =, but you will have to enter the entire field or subfield exactly as spaced and punctuated:</p>
<pre>     245$a=='Field experiments with sugar cane :'</pre>
<p>Entering an exact-match search for multiple subfields is not recommended. </p>
<h3>Case-sensitivity</h3>
<p>Searches default to <b>case-insensitivity</b>. Use double-quotes instead of single-quotes if case-sensitivity is wanted:</p>
<pre>     100$a="Jones"</pre>
<h3>Multiple search terms</h3>
<p>You may enter up to three search terms, separating the terms with "and" or "or":</p>
<pre>     1xx=jones and 245$a=chemistry and 260$a='new york'<br>     1xx=jones and 245$a=chemistry or 650=chemistry<br>     1xx=jones or 1xx=jonas or 1xx=johns</pre>
<p>Note the difference between</p>
<pre>     245$a='chemistry and physics'</pre>
<p>and</p>
<pre>     245$a=chemistry and 245$a=physics</pre>
<h3>Nesting</h3>
<p>You may use parentheses to make your desired nesting clear. If you enter three terms without parentheses, such as</p>
<pre>     1xx=jones and 245$a=chemistry or 650=chemistry</pre>
<p>they are interpreted like this:</p>
<pre>     (1xx=jones and 245$a=chemistry) or 650=chemistry</pre>
<p>You may override this default nesting by using parentheses like this:</p>
<pre>     1xx=jones and (245$a=chemistry or 650=chemistry)</pre>
<p>(By the laws of set theory, parentheses have no effect if both operators are "and" or both are "or". Parentheses also have no effect on one- or two-term searches.)</p>
<h3>"Not" searches</h3>
<p>You may search for words and phrases NOT in the record like this (where "<>" means "does not contain"):</p>
<pre>     260$a<>'new york'</pre>
<p>Terms containing "not" may be combined like any others:</p>
<pre>     1xx="Jones" and 260$a<>'new york'</pre>
<p>"Not" searches are always "not like" searches. There is no way to specify an exact-match "not" search.</p>
<h3>Field/subfield presence or absence</h3>
<p>You may also use terms that specify a field and/or subfield be present, but with no particular value:</p>
<pre>     035=''<br>     6xx$x=''</pre>
<p>Or use the <> operator to specify the absence of a field or subfield:</p>
<pre>     035<>''<br>     6xx$x<>''</pre>
<div class=gotop><a href="#contents">Top</a></div>
<a name='reset'></a>
<h2>SQL Reset</h2>
<p>After a SQL search, you may resume working with the entire file by using the SQL Reset menu item in the Navigate menu. Or you may immediately perform another SQL search on the entire file; resetting is not required before another SQL query. Or you may open a new file if you wish.</p>
<div class=gotop><a href="#contents">Top</a></div>

<!--#include virtual="/footer.htm"-->
</body>
</html>
