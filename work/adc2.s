include <example.inst>
init_val	VRl 0
init_val	VRh 10000
init_sig	adc_start false
init_sig	adc_ccf false
init_val	AN2 undef
init_val	AN3 undef

; initiate round robin reading from an0-an3
a_start	set_sig	 adc_start&adc_mult&~adc_cc adc_start false 0 0
ins0	set_val	 ~adc_start ADR1 (temp-VRl)*255/(VRh-VRl) 32 32
ins1	set_val  ~adc_start ADR2 (temp-VRl)*255/(VRh-VRl) 32 32
ins2	set_val  ~adc_start ADR3 AN2 32 32
ins3	set_val  ~adc_start ADR4 AN3 32 32
	set_sig  NO_TRANS adc_ccf true 0 0
	iff	 adc_scan ins0 0 0 0 0
	link	 a_start 
ins0	pause	 adc_start 0 0 
	link	 a_start 
ins1	pause	 adc_start 0 0 
	link	 a_start 
ins2	pause	 adc_start 0 0 
	link	 a_start 
ins3	pause	 adc_start 0 0 
	link	 a_start

; initiate round robin reading from an7-an7
a_start	set_sig	 adc_start&adc_mult&adc_cc adc_start false 0 0
ins4	set_val	 ~adc_start ADR1 (temp-VRl)*255/(VRh-VRl) 32 32
ins5	set_val  ~adc_start ADR2 (temp-VRl)*255/(VRh-VRl) 32 32
ins6	set_val  ~adc_start ADR3 AN2 32 32
ins7	set_val  ~adc_start ADR4 AN3 32 32
	set_sig  NO_TRANS adc_ccf true 0 0
	iff	 adc_scan ins4 0 0 0 0
	link	 a_start
ins4	pause	 adc_start 0 0 
	link	 a_start 
ins5	pause	 adc_start 0 0 
	link	 a_start 
ins6	pause	 adc_start 0 0 
	link	 a_start 
ins7	pause	 adc_start 0 0 
	link	 a_start 
