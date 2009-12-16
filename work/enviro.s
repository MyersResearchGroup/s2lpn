 include <example.inst>
 univ_pred	~shutdown 
mark
e_start	set_rate  temp<=2200 temp 2 3 5
	set_rate  temp>=9800 temp -2  3 5
	link	  e_start
