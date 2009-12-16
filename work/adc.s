include <example.inst>
univ_pred	~shutdown 
mark
a_start	set_val  true ADC1 temp*256/10000 10 10
	set_val  true ADC2 temp*256/10000 10   10
	link	  a_start
