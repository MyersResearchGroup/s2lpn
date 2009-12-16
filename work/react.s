;@ include <example.inst>
;@ univ_pred	~shutdown 
start	set_val  true A ADC1 35 45 
	set_val  true B ADC2  5 5
	iff	 ((A-B)<=3)&((A-B)>=-3) start  5 5 
	set_sig	 NO_TRANS shutdown true 0 0
