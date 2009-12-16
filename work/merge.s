;@ include <example.inst>
;@ univ_pred ~fail
start	     set_val true x 45 10 12
	     set_rate NO_TRANS x 2 0 0
	     set_sig x>100 d true 10 20
	     set_sig NO_TRANS g false 10 20
;	     link start
	     jump_back
