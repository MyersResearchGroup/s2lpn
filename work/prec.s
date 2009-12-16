include <example.inst>
main	set_sig ~irq_req irq_forb true 0 0 
	set_val true regB 1 3 3
	set_sig NO_TRANS irq_forb false 0 0
	set_sig ~irq_req irq_forb true 0 0 
	set_sig true irq_forb false 3 3
	link main

	mark
isr	pause irq 0 0
	set_sig true irq_req true 0 0
	pause ~irq_forb 12 12
	set_sig true irq_req false 12 12
	link isr