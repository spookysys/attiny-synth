/*****************************************************************************
*
* Atmel Corporation
*
* File              : update_yl.asm
* Compiler          : IAR EWAAVR 3.10C
* Revision          : $Revision: 1.4 $
* Date              : $Date: Monday, November 15, 2004 12:53:58 UTC $
* Updated by        : $Author: raapeland $
*
* Support mail      : avr@atmel.com
*
* Supported devices : This example is written for the ATmega128. The firmware
*                     should compile on any AVR with at least 4kB of Flash
*                     memory (plus enough room for sound data).
*
* AppNote           : AVR336: ADPCM Decoder.
*
* Description       : Assembly routine to update decoder parameter yl
*
****************************************************************************/

.global update_yl
.text

update_yl:
  
MOVW  R0, R16     ; save yl
MOVW  R2, R18
  
COM   R19         ; negate yl
COM   R18
COM   R17
NEG   R16
SBCI  R17, 0xFF
SBCI  R18, 0xFF
SBCI  R19, 0xFF
  
ASR   R19         ; shift right by 6
ROR   R18
ROR   R17
ROR   R16

ASR   R19
ROR   R18
ROR   R17
ROR   R16

ASR   R19
ROR   R18
ROR   R17
ROR   R16

ASR   R19
ROR   R18
ROR   R17
ROR   R16

ASR   R19
ROR   R18
ROR   R17
ROR   R16

ASR   R19
ROR   R18
ROR   R17
ROR   R16

ADD   R16, R20    ; add yu
ADC  R17, R21
LDI  R20, 0
ADC  R18, R20
ADC  R19, R20
  
ADD  R16, R0
ADC  R17, R1
ADC  R18, R2
ADC  R19, R3
  
RET

