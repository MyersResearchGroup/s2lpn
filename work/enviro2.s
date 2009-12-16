;@ include <example.inst>
; @ univ_pred	~shutdown 
e_start	set_rate  temp<=2200 temp 2 3 5
dr_rod	set_rate  temp>=9800 temp -2 3 5
	link	  e_start
dr_rod	set_rate  temp>=9800 temp -1 3 5
	link	  e_start
