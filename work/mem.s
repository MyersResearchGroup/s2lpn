; implements a 16-byte memory element
include <example.inst>

init_val MEM_ADDR 7
init_sig MEM_READ false
init_sig MEM_WRITE false

unmark
mem_write   set_val MEM_WRITE&(MEM_ADDR=0) MEM_0 MEM_DATA 0 0
	    set_sig NO_TRANS  MEM_WRITE false 0 0 mem_write_done
mem_write   set_val MEM_WRITE&(MEM_ADDR=1) MEM_1 MEM_DATA 0 0
	    set_sig NO_TRANS  MEM_WRITE false 0 0 mem_write_done
mem_write   set_val MEM_WRITE&(MEM_ADDR=2) MEM_2 MEM_DATA 0 0
	    set_sig NO_TRANS  MEM_WRITE false 0 0 mem_write_done
mem_write   set_val MEM_WRITE&(MEM_ADDR=3) MEM_3 MEM_DATA 0 0
	    set_sig NO_TRANS  MEM_WRITE false 0 0 mem_write_done
mem_write   set_val MEM_WRITE&(MEM_ADDR=4) MEM_4 MEM_DATA 0 0
	    set_sig NO_TRANS  MEM_WRITE false 0 0 mem_write_done
mem_write   set_val MEM_WRITE&(MEM_ADDR=5) MEM_5 MEM_DATA 0 0
	    set_sig NO_TRANS  MEM_WRITE false 0 0 mem_write_done
mem_write   set_val MEM_WRITE&(MEM_ADDR=6) MEM_6 MEM_DATA 0 0
	    set_sig NO_TRANS  MEM_WRITE false 0 0 mem_write_done
mem_write   set_val MEM_WRITE&(MEM_ADDR=7) MEM_7 MEM_DATA 0 0
	    set_sig NO_TRANS  MEM_WRITE false 0 0 mem_write_done
mem_write_done goto false mem_write

mem_read    set_val MEM_READ&(MEM_ADDR=0)  MEM_DATA MEM_0 0 0
	    set_sig NO_TRANS  MEM_READ 	false 0 0 mem_read_done
mem_read    set_val MEM_READ&(MEM_ADDR=1)  MEM_DATA MEM_1 0 0
	    set_sig NO_TRANS  MEM_READ 	false 0 0 mem_read_done
mem_read    set_val MEM_READ&(MEM_ADDR=2)  MEM_DATA MEM_2 0 0
	    set_sig NO_TRANS  MEM_READ 	false 0 0 mem_read_done
mem_read    set_val MEM_READ&(MEM_ADDR=3)  MEM_DATA MEM_3 0 0
	    set_sig NO_TRANS  MEM_READ 	false 0 0 mem_read_done
mem_read    set_val MEM_READ&(MEM_ADDR=4)  MEM_DATA MEM_4 0 0
	    set_sig NO_TRANS  MEM_READ 	false 0 0 mem_read_done
mem_read    set_val MEM_READ&(MEM_ADDR=5)  MEM_DATA MEM_5 0 0
	    set_sig NO_TRANS  MEM_READ 	false 0 0 mem_read_done
mem_read    set_val MEM_READ&(MEM_ADDR=6)  MEM_DATA MEM_6 0 0
	    set_sig NO_TRANS  MEM_READ  false 0 0 mem_read_done
mem_read    set_val MEM_READ&(MEM_ADDR=7)  MEM_DATA MEM_7 0 0
	    set_sig NO_TRANS  MEM_READ 	false 0 0 mem_read_done
mem_read_done goto false mem_read

