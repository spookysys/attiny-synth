#!/usr/bin/php
<?
/*
Player for compressed Monotone songs.
(C) 2012 Jeroen Domburg (jeroen AT spritesmods.com)
Monotone, by Trixter: http://www.oldskool.org/pc/MONOTONE

This program is free software: you can redistribute it and/or modify
t under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


//Split a song into seperate arrays containing all data and compress it by leaving
//out the bytes containing a '00'. See player.c for an explanation on the
//compressed format.


//$infile="./monotone/PRELUDE.MON";
$infile="./monotone/RICKROLL.MON";
//$infile="./monotone/YESSO.MON";


function getbyte($f) {
    return ord(fread($f,1));
}

function getshort($f) {
    $i=getbyte($f);
    $i+=getbyte($f)*256;
    return $i;
}

function getPascalString($f) {
    $l=getbyte($f);
    return fread($f,$l);
}

/*
WARNING! Strings may be pascal-type strings, e.g. first the length, then the data.
Contents of the .mon songfile:
0-9 (9) : "MONOTONE" <-- Pascal string
A-32 (0x28) : Title <-- Pascal string, stored in 41 bytes
2A-5A (0x28): Comment <- Pascal string, stored in 41 bytes
5B: version
5C: totalPatterns
5D: totalTracks
5E: Cellsize
5F-15E: Pattern-order (ff = end of song)
*/

$f_c=fopen("songdata.c","w");
$f_h=fopen("songdata.h","w");
$f=fopen($infile,"r");
fputs($f_c,"//Song data auto-generated from $infile\n");
fputs($f_h,"//Song data auto-generated from $infile\n");

$t=getPascalString($f);
if ($t!="MONOTONE") die("Not a monotone file! (sig wrong)\n");
$tlen=getbyte($f);
$title=trim(substr(fread($f,0x28),0,$tlen));
$clen=getbyte($f);
$comment=trim(substr(fread($f,0x28),0,$clen));
$version=getbyte($f);
$nopats=getbyte($f);
$notracks=getbyte($f);
$cellsize=getbyte($f);
fwrite($f_h,"#define songTitle \"$title\"\n");
fwrite($f_h,"#define songComment \"$comment\"\n");
fwrite($f_h,"#define songVersion ".$version."\n");
fwrite($f_h,"#define songTotalPatterns ".$nopats."\n");
fwrite($f_h,"#define songTotalTracks ".$notracks."\n");
fwrite($f_h,"#define songCellSize ".$cellsize."\n"); //afaik, always 2

fwrite($f_h,"extern const unsigned short songPatterns[];\n");


//for later
for ($x=0; $x<256; $x++) {
    $patorder[$x]=getbyte($f);
}

fwrite($f_h,"extern const unsigned char songData[];\n\n");
$d="#ifdef AVR\n";
$d.="#include <avr/pgmspace.h>\n";
$d.="const unsigned char songData[] PROGMEM = {\n";
$d.="#else\n";
$d.="const unsigned char songData[] = {\n";
$d.="#endif\n";
$dlen=0;
for ($pno=0; $pno<$nopats; $pno++) {
    $patpos[$pno]=$dlen;
    $d.="// pattern $pno\n";
    for ($pos=0; $pos<64; $pos++) {
	$d.="        ";
	for ($tno=0; $tno<$notracks; $tno++) {
	    $fd=getshort($f);
	    $pd[$tno*2]=$fd>>8;
	    $pd[$tno*2+1]=$fd&0xff;
	}
	$ab=0;
	$dd="";
	for ($b=0; $b<8; $b++) {
	    $ab=$ab>>1;
	    if ($pd[$b]!=0) {
		$ab+=0x80;
		$dd.=sprintf("0x%02x, ",$pd[$b]);
		$dlen++;
	    }
	}
	$dlen++;
	$d.=sprintf("0x%02x,",$ab).$dd."\n";
    }
}
$d.="};\n\n";
fwrite($f_c,$d);

$d="#ifdef AVR\n";
$d.="const unsigned short songPatterns[] PROGMEM = {\n";
$d.="#else\n";
$d.="const unsigned short songPatterns[] = {\n";
$d.="#endif\n        ";
$insong=true;
$plen=0;
$cd="//Order of patterns: ";
for ($x=0; $x<256; $x++) {
    if ($insong) {
	$plen+=2;
        if ($patorder[$x]==255) {
	    $d.="0xffff\n";
	    $insong=false;
	} else {
	    $cd.=$patorder[$x].",";
	    $d.=sprintf("0x%04x, ",$patpos[$patorder[$x]]);
	}
    }
}
$d.="};\n\n";
fwrite($f_c,$cd." end\n");
fwrite($f_c,$d);


echo "Pattern length: $plen\nData length: $dlen\nTotal: ".($plen+$dlen)."\n";
?>