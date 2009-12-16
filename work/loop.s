include	<example.inst>
	set_val true x 0 1 1
loop	set_val x<10 x x+1 1 1
	link	loop
loop	pause	x>=10 1 1
