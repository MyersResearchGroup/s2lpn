;@ include <6811.inst>
;@ univ_pred second_thr
fran	ldAB	fox
	addb	#1
	stab	fox
;@ pred fail
	bra	fran
;@ univ_pred  ~fail
