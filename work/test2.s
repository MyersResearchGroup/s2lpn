;@ include <6811.inst>
main1	ldAB	#0
loop	stab	foo
	ldab	foo
	addb	#1
	cmpb	#10
	beq	main1
	cmpb	#20
	bge	loop
;@ set_pred fail
self	bra	self
;@ univ_pred  ~fail
