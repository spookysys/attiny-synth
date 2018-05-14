;**** A P P L I C A T I O N   N O T E   A V R 2 0 0 ************************
;*
;* Title:		Multiply and Divide Routines
;* Version:		1.1
;* Last updated:	97.07.04
;* Target:		AT90Sxxxx (All AVR Devices)
;*
;* Support E-mail:	avr@atmel.com
;* 
;* DESCRIPTION
;* This Application Note lists subroutines for the following
;* Muliply/Divide applications. Routines are straight-line implementations
;* optimized for speed:
;*
;*  8 x  8 = 16 bit unsigned
;* 16 x 16 = 32 bit unsigned
;*  8 /  8 =  8 +  8 bit unsigned
;* 16 / 16 = 16 + 16 bit unsigned
;*
;***************************************************************************

.include "1200def.inc"

	rjmp	RESET	;reset handle

;***************************************************************************
;*
;* "mpy8u" - 8x8 Bit Unsigned Multiplication
;*
;* This subroutine multiplies the two register variables mp8u and mc8u.
;* The result is placed in registers m8uH, m8uL
;*  
;* Number of words	:34 + return
;* Number of cycles	:34 + return
;* Low registers used	:None
;* High registers used  :3 (mc8u,mp8u/m8uL,m8uH)	
;*
;* Note: Result Low byte and the multiplier share the same register.
;* This causes the multiplier to be overwritten by the result.
;*
;***************************************************************************

;***** Subroutine Register Variables

.def	mc8u	=r16		;multiplicand
.def	mp8u	=r17		;multiplier
.def	m8uL	=r17		;result Low byte
.def	m8uH	=r18		;result High byte

;***** Code

mpy8u:	clr	m8uH		;clear result High byte
	lsr	mp8u		;shift multiplier
	
	brcc	noad80		;if carry set
	add	m8uH,mc8u	;    add multiplicand to result High byte
noad80:	ror	m8uH		;shift right result High byte 
	ror	m8uL		;rotate right result L byte and multiplier

	brcc	noad81		;if carry set
	add	m8uH,mc8u	;    add multiplicand to result High byte
noad81:	ror	m8uH		;shift right result High byte 
	ror	m8uL		;rotate right result L byte and multiplier

	brcc	noad82		;if carry set
	add	m8uH,mc8u	;    add multiplicand to result High byte
noad82:	ror	m8uH		;shift right result High byte 
	ror	m8uL		;rotate right result L byte and multiplier

	brcc	noad83		;if carry set
	add	m8uH,mc8u	;    add multiplicand to result High byte
noad83:	ror	m8uH		;shift right result High byte 
	ror	m8uL		;rotate right result L byte and multiplier

	brcc	noad84		;if carry set
	add	m8uH,mc8u	;    add multiplicand to result High byte
noad84:	ror	m8uH		;shift right result High byte 
	ror	m8uL		;rotate right result L byte and multiplier

	brcc	noad85		;if carry set
	add	m8uH,mc8u	;    add multiplicand to result High byte
noad85:	ror	m8uH		;shift right result High byte 
	ror	m8uL		;rotate right result L byte and multiplier

	brcc	noad86		;if carry set
	add	m8uH,mc8u	;    add multiplicand to result High byte
noad86:	ror	m8uH		;shift right result High byte 
	ror	m8uL		;rotate right result L byte and multiplier

	brcc	noad87		;if carry set
	add	m8uH,mc8u	;    add multiplicand to result High byte
noad87:	ror	m8uH		;shift right result High byte 
	ror	m8uL		;rotate right result L byte and multiplier
	
	ret


;***************************************************************************
;*
;* "mpy16u" - 16x16 Bit Unsigned Multiplication
;*
;* This subroutine multiplies the two 16-bit register variables 
;* mp16uH:mp16uL and mc16uH:mc16uL.
;* The result is placed in m16u3:m16u2:m16u1:m16u0.
;*  
;* Number of words	:105 + return
;* Number of cycles	:105 + return
;* Low registers used	:None
;* High registers used  :6 (mp16uL,mp16uH,mc16uL/m16u0,mc16uH/m16u1,m16u2,
;*			    m16u3)	
;*
;***************************************************************************

;***** Subroutine Register Variables



.def	mc16uL	=r16		;multiplicand low byte
.def	mc16uH	=r17		;multiplicand high byte
.def	mp16uL	=r18		;multiplier low byte
.def	mp16uH	=r19		;multiplier high byte
.def	m16u0	=r18		;result byte 0 (LSB)
.def	m16u1	=r19		;result byte 1
.def	m16u2	=r20		;result byte 2
.def	m16u3	=r21		;result byte 3 (MSB)

;***** Code

mpy16u:	clr	m16u3		;clear 2 highest bytes of result
	clr	m16u2	
	lsr	mp16uH		;rotate multiplier Low
	ror	mp16uL		;rotate multiplier High

	brcc	noadd0		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd0:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noadd1		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd1:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noadd2		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd2:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noadd3		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd3:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noadd4		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd4:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noadd5		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd5:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noadd6		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd6:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noadd7		;if carry sett
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd7:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noadd8		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd8:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noadd9		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noadd9:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noad10		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noad10:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noad11		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noad11:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noad12		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noad12:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noad13		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noad13:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noad14		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noad14:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	brcc	noad15		;if carry set
	add	m16u2,mc16uL	;    add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;    add multiplicand high to byte 3 of res
noad15:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low

	ret



;***************************************************************************
;*
;* "div8u" - 8/8 Bit Unsigned Division
;*
;* This subroutine divides the two register variables "dd8u" (dividend) and 
;* "dv8u" (divisor). The result is placed in "dres8u" and the remainder in
;* "drem8u".
;*  
;* Number of words	:66 + return
;* Number of cycles	:50/58/66 (Min/Avg/Max) + return
;* Low registers used	:1 (drem8u)
;* High registers used  :2 (dres8u/dd8u,dv8u)
;*
;***************************************************************************

;***** Subroutine Register Variables

.def	drem8u	=r15		;remainder
.def	dres8u	=r16		;result
.def	dd8u	=r16		;dividend
.def	dv8u	=r17		;divisor

;***** Code

div8u:	sub	drem8u,drem8u	;clear remainder and carry
	
	rol	dd8u		;shift left dividend
	rol	drem8u		;shift dividend into remainder
	sub	drem8u,dv8u	;remainder = remainder - divisor
	brcc	d8u_1		;if result negative
	add	drem8u,dv8u	;    restore remainder
	clc			;    clear carry to be shifted into result
	rjmp	d8u_2		;else
d8u_1:	sec			;    set carry to be shifted into result

d8u_2:	rol	dd8u		;shift left dividend
	rol	drem8u		;shift dividend into remainder
	sub	drem8u,dv8u	;remainder = remainder - divisor
	brcc	d8u_3		;if result negative
	add	drem8u,dv8u	;    restore remainder
	clc			;    clear carry to be shifted into result
	rjmp	d8u_4		;else
d8u_3:	sec			;    set carry to be shifted into result

d8u_4:	rol	dd8u		;shift left dividend
	rol	drem8u		;shift dividend into remainder
	sub	drem8u,dv8u	;remainder = remainder - divisor
	brcc	d8u_5		;if result negative
	add	drem8u,dv8u	;    restore remainder
	clc			;    clear carry to be shifted into result
	rjmp	d8u_6		;else
d8u_5:	sec			;    set carry to be shifted into result

d8u_6:	rol	dd8u		;shift left dividend
	rol	drem8u		;shift dividend into remainder
	sub	drem8u,dv8u	;remainder = remainder - divisor
	brcc	d8u_7		;if result negative
	add	drem8u,dv8u	;    restore remainder
	clc			;    clear carry to be shifted into result
	rjmp	d8u_8		;else
d8u_7:	sec			;    set carry to be shifted into result

d8u_8:	rol	dd8u		;shift left dividend
	rol	drem8u		;shift dividend into remainder
	sub	drem8u,dv8u	;remainder = remainder - divisor
	brcc	d8u_9		;if result negative
	add	drem8u,dv8u	;    restore remainder
	clc			;    clear carry to be shifted into result
	rjmp	d8u_10		;else
d8u_9:	sec			;    set carry to be shifted into result

d8u_10:	rol	dd8u		;shift left dividend
	rol	drem8u		;shift dividend into remainder
	sub	drem8u,dv8u	;remainder = remainder - divisor
	brcc	d8u_11		;if result negative
	add	drem8u,dv8u	;    restore remainder
	clc			;    clear carry to be shifted into result
	rjmp	d8u_12		;else
d8u_11:	sec			;    set carry to be shifted into result

d8u_12:	rol	dd8u		;shift left dividend
	rol	drem8u		;shift dividend into remainder
	sub	drem8u,dv8u	;remainder = remainder - divisor
	brcc	d8u_13		;if result negative
	add	drem8u,dv8u	;    restore remainder
	clc			;    clear carry to be shifted into result
	rjmp	d8u_14		;else
d8u_13:	sec			;    set carry to be shifted into result

d8u_14:	rol	dd8u		;shift left dividend
	rol	drem8u		;shift dividend into remainder
	sub	drem8u,dv8u	;remainder = remainder - divisor
	brcc	d8u_15		;if result negative
	add	drem8u,dv8u	;    restore remainder
	clc			;    clear carry to be shifted into result
	rjmp	d8u_16		;else
d8u_15:	sec			;    set carry to be shifted into result

d8u_16:	rol	dd8u		;shift left dividend
	ret


;***************************************************************************
;*
;* "div16u" - 16/16 Bit Unsigned Division
;*
;* This subroutine divides the two 16-bit numbers 
;* "dd8uH:dd8uL" (dividend) and "dv16uH:dv16uL" (divisor). 
;* The result is placed in "dres16uH:dres16uL" and the remainder in
;* "drem16uH:drem16uL".
;*  
;* Number of words	:196 + return
;* Number of cycles	:148/173/196 (Min/Avg/Max)
;* Low registers used	:2 (drem16uL,drem16uH)
;* High registers used  :4 (dres16uL/dd16uL,dres16uH/dd16uH,dv16uL,dv16uH)
;*
;***************************************************************************

;***** Subroutine Register Variables

.def	drem16uL=r14
.def	drem16uH=r15
.def	dres16uL=r16
.def	dres16uH=r17
.def	dd16uL	=r16
.def	dd16uH	=r17
.def	dv16uL	=r18
.def	dv16uH	=r19

;***** Code

div16u:	clr	drem16uL	;clear remainder Low byte
	sub	drem16uH,drem16uH;clear remainder High byte and carry

	rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_1		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_2		;else
d16u_1:	sec			;    set carry to be shifted into result

d16u_2:	rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_3		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_4		;else
d16u_3:	sec			;    set carry to be shifted into result

d16u_4:	rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_5		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_6		;else
d16u_5:	sec			;    set carry to be shifted into result

d16u_6:	rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_7		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_8		;else
d16u_7:	sec			;    set carry to be shifted into result

d16u_8:	rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_9		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_10		;else
d16u_9:	sec			;    set carry to be shifted into result

d16u_10:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_11		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_12		;else
d16u_11:sec			;    set carry to be shifted into result

d16u_12:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_13		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_14		;else
d16u_13:sec			;    set carry to be shifted into result

d16u_14:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_15		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_16		;else
d16u_15:sec			;    set carry to be shifted into result

d16u_16:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_17		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_18		;else
d16u_17:	sec			;    set carry to be shifted into result

d16u_18:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_19		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_20		;else
d16u_19:sec			;    set carry to be shifted into result

d16u_20:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_21		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_22		;else
d16u_21:sec			;    set carry to be shifted into result

d16u_22:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_23		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_24		;else
d16u_23:sec			;    set carry to be shifted into result

d16u_24:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_25		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_26		;else
d16u_25:sec			;    set carry to be shifted into result

d16u_26:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_27		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_28		;else
d16u_27:sec			;    set carry to be shifted into result

d16u_28:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_29		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_30		;else
d16u_29:sec			;    set carry to be shifted into result

d16u_30:rol	dd16uL		;shift left dividend
	rol	dd16uH
	rol	drem16uL	;shift dividend into remainder
	rol	drem16uH
	sub	drem16uL,dv16uL	;remainder = remainder - divisor
	sbc	drem16uH,dv16uH	;
	brcc	d16u_31		;if result negative
	add	drem16uL,dv16uL	;    restore remainder
	adc	drem16uH,dv16uH
	clc			;    clear carry to be shifted into result
	rjmp	d16u_32		;else
d16u_31:sec			;    set carry to be shifted into result

d16u_32:rol	dd16uL		;shift left dividend
	rol	dd16uH
	ret



;****************************************************************************
;*
;* Test Program
;*
;* This program calls all the subroutines as an example of usage and to 
;* verify correct verification.
;*
;****************************************************************************

;***** Main Program Register variables

.def	temp	=r16		;temporary storage variable

;***** Code
RESET:
;---------------------------------------------------------------
;Include these lines for devices with SRAM
;	ldi	temp,low(RAMEND)
;	out	SPL,temp	
;	ldi	temp,high(RAMEND)
;	out	SPH,temp	;init Stack Pointer
;---------------------------------------------------------------

;***** Multiply Two Unsigned 8-Bit Numbers (250 * 4)

	ldi	mc8u,250
	ldi	mp8u,4
	rcall	mpy8u		;result: m8uH:m8uL = $03e8 (1000)

;***** Multiply Two Unsigned 16-Bit Numbers (5050 * 10,000)
	ldi	mc16uL,low(5050)
	ldi	mc16uH,high(5050)
	ldi	mp16uL,low(10000)
	ldi	mp16uH,high(10000)
	rcall	mpy16u		;result: m16u3:m16u2:m16u1:m16u0
				;=$030291a0 (50,500,000)

;***** Divide Two Unsigned 8-Bit Numbers (100/3)
	ldi	dd8u,100
	ldi	dv8u,3
	rcall	div8u		;result: 	$21 (33)
				;remainder:	$01 (1)

;***** Divide Two Unsigned 16-Bit Numbers (50,000/24,995)
	ldi	dd16uL,low(50000)
	ldi	dd16uH,high(50000)
	ldi	dv16uL,low(24995)
	ldi	dv16uH,high(24995)
	rcall	div16u		;result:	$0002 (2)
				;remainder:	$000a (10)

forever:rjmp	forever
