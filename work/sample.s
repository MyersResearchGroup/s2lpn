include <example.inst>
p0	set_sig x>9 g true 0 3
	set_rate NO_TRANS x -2 0 0
p1	set_val x<3 x y+2 0 0
	link p0

mark
p2	set_val x<3 y y*2 0 0 
	link p2
