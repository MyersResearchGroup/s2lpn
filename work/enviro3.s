include <example.inst>
univ_pred	~shutdown
e_start		set_rate	rod1&rod2 temp 2 3 5
		link		e_start

e_start		set_rate  	rod1&~rod2 temp -2 3 5
		link		e_start
e_start		set_rate  	~rod1&rod2 temp -1 3 5
	link	  e_start
e_start		set_rate  ~rod1&~rod2 temp 0 3 5
	link	  e_start

