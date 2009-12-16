;@ include <6811.inst>
; @ univ_pred	 ~shutdown
;@ init_sig 	shutdown false
;@ mark
main	ldab	#48
	stab 	ADCTL
test	ldab	ADCTL
	cmpb	#128 
	blo	 test
loop	ldab 	 ADR1
	ldaa 	 ADR2
	sba
	adda	 #1
	cmpa 	 #2
	bls  	 loop
	ldab	 #7
	stab	 PORTB
;@	set_pred shutdown 
term	bra 	 term

