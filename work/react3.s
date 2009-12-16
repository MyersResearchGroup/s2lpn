;@ include <6811.inst>
;@ univ_pred	 ~shutdown
;@ mark
main	ldab	 #80
	stab 	 c1
	stab 	 c2
	ldab 	 #128 
	stab	 OPTION

	orab 	 TMSK1
	stab 	 TMSK1
	ldab 	 #128 
	ldab 	 #128 
	stab 	 TFLG1
	ldd  	 TCNT
	addd 	 #30
	STD  	 TOC1
loop	ldab 	 ADR1
	ldaa 	 ADR2
	sba
	adda	 #1
	cmpa 	 #2
	bls  	 skip
	ldab	 #7
	stab	 PORTB
;@	set_pred shutdown 
term	bra 	 term
skip	stab	 c1	;start the rid checks here, inst is bogus!!!
	bra	 loop
