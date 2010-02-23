;@ include <6811.inst>

main	ldab	#48
	stab 	ADCTL
test	ldab	ADCTL
	bpl	 test
loop	ldab 	 ADR1
	ldaa 	 ADR2
	sba
	adda	 #6
	cmpa 	 #12
	bls  	 loop
;@	fail_set
	ldab	 #7
	stab	 PORTB
term	bra 	 term

