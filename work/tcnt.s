;@ include <example.inst>
;@ univ_pred	~shutdown
;@ init_val	MAIN_THR true
;@ init_val	TOC1_HANDLER false
;@ init_val	OC1F false
;@ init_val	OC1I false
;@ mark
tcnt_start	set_sig  (TCNT=TOC1)&OC1I MAIN_THR false 0 0
		set_sig	 NO_TRANS OC1F true 0 0
		set_sig  true TOC1_HANDLER TRUE 12 12
		set_sig	 TOC1_HANDLER=false MAIN_THR TRUE 0 0
		link	 tcnt_start

;@ init_val	TCNT 0
;@ mark
loopy		set_val TCNT<=65534 TCNT TCNT+1 1 1
		link loopy
loopy		set_val TCNT>=65535 TCNT 90 1 1
		link loopy
