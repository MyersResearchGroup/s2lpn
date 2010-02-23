;@ include <example.inst>
init_rate	temp -2
init_val	temp 2200

e_start	set_rate  temp<=2200 temp 2 5 5
dr_rod	set_rate  temp>=9800 temp -2 5 5
	link	  e_start
