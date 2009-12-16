;@ include <6811.inst>
; guard all transitions with the boolean variable "main_thr"
;@ univ_pred main_thr
main	ldAB	foo
	addb	#1
;@ set_pred fail
	stab	foo
;@ pred fail
;@ clear_pred fail
	bra	main
;@ univ_pred  ~fail
;@ init_val joe 1
;@ init_val fail false
;@ init_val foo 78
