#include "load.h"

void process_line(string line, language *in_language, fstream* progfile,
		  language *out_language, pr_list *pragmas, int *mark, 
		  int *fail_trans){
  instruction *curr_mnem = 0;
  string prag, prag_args;
  string word, word2;
  string assignment;
  int found=0;

  if (line == ""){
    //cout << "processed blank line\n";
    return;
  }

  // check for unlabelled instruction
  word = "";
  word2 = getword(line);
  line = stripword(line);
  //cout << "looking up \""<<word2<<"\" with args \"" <<line <<"\"\n";
  if (word2 == "mark"){
    *mark = 1;
    found++;
    //cout << "marking next instruction.\n";
  }
  if (word2 == "unmark"){
    *mark = 0;
    found++;
    //cout << "unmarking next instruction.\n";
  }
  if (word2 == "fail_set"){
    *fail_trans = 1;
    found++;
    //cout << "next instruction in fail set.\n";
  }
  if ((word2 == "univ_pred")||(word2 == "init_val")||(word2 == "init_sig")||
      (word2 == "init_rate")){
    //out << "pragma: " << word2<<" "<<line<< "\n";
        pragmas->tail_insert(word2, line);
    found++;
  }
  else {
    curr_mnem = in_language->find_inst(upconvert(word2), line);
    if (curr_mnem)
      found++;
  }
  if (!found){
    // not found, must be a labelled instruction
    // strip colon
    if (word2.at(word2.length()-1)==':'){
      //cout << word2 << " ends in a colon\n";
      word = word2.substr(0,word2.length()-1);
      //cout << word << " does not\n";
    }
    else
      word = word2;
    //cout << word << " must be a label\n";
    word2 = getword(line);
      line = stripword(line);
      //cout << "looking up \""<<word2<<"\" with args \"" <<line <<"\"\n";
      if ((word2 == "univ_pred")||(word2 == "init_val")||
	(word2 == "init_sig")||(word2 == "init_rate")){
	cout << "pragma: " << word2<<" "<<line<< "\n";
	pragmas->tail_insert(word2, line);
	found++;
      }
      else {
	curr_mnem=in_language->find_inst(upconvert(word2), line);
	if (curr_mnem)
	  found++;
      }
  }
  if (curr_mnem){
    //cout << "found the instruction\n";
    curr_mnem->label = word;
    if (*mark){
      //cout << "marking this instruction\n";
      curr_mnem->marked = 1;
      *mark = 0;
    }
    if (*fail_trans){
      curr_mnem->failed = 1;
      *fail_trans = 0;
    }
    out_language->tail_insert(curr_mnem);
  }
  else if (!found){
    cout << "*** instruction "<< word << " " << word2 
	   << " " << line << " not found ***\n";
  }
}

language *get_language(library *languages, string fname){
  char *env_str = getenv("S2LPNLIB");
  string env_var;
  if (env_str == NULL) {
    env_var = "../";
  } else {
    env_var = getenv("S2LPNLIB");
  }
  int local = 0;
  if (fname.at(0) == '"'){
    local = 1;
  }
  fname = fname.substr(1,fname.length()-2);
  language *temp = languages->lookup(fname);
  if (!temp){
    //cout << "loading language "<< fname <<"\n";
    if (local)
      temp = load_language(fname);
    else{
      temp = load_language(env_var+fname);      
    }
    temp->filename = fname;
    if (temp){
      cout << "loaded language "<< fname <<"\n";
      //temp->print();
      languages->insert(temp);
    }
  }
  if (!temp){
    cout << "could not load language from file "<<fname<<".\n";
  }
  return temp;
}

int main(int argc, char *argv[]){
  // this is what we're building
  lhpn *graph = new lhpn;
  // input lines and parts thereof
  string line, prag, prag_args;
  // set of pragmas
  pr_list *pragmas;
  // file names
  string langFname, progFname, outFname;
  // reformatted instructions for output
  language *in_language,*out_language;
  string word1, word2;
  library *langs = new library;
  int i;
  if (argc >1){
    line = argv[1];
    if (line == "-o"){
      outFname = argv[2];
      size_t suffix = outFname.find_last_of(".");
      if (suffix != outFname.npos){
	outFname = outFname.substr(0,suffix) + ".lpn";
      }
      else{
	outFname += ".lpn";
      }
      i = 3;
    }
    else {
      outFname = argv[1];
      outFname = outFname.substr(0,outFname.length()-2)+".lpn";
      i = 1;
    }
  }
  while(i<argc){
    // status of first instruction
    int mark = 1;
    int fail_trans = 0;
    pragmas = new pr_list;
    out_language = new language;
    progFname = argv[i++];
    cout << "opening file " << progFname << "\n";
    fstream progfile(progFname.c_str(),fstream::in);
    if (!progfile.is_open()){
      cout << "failed to open file " << progFname <<endl;
      return 2;
    } 
    out_language->filename = progFname;
    // get Language name
    line = ass_pop_line(&progfile);
    langFname = stripword(line);
    in_language = get_language(langs,langFname);
    if (!in_language){
      return 1;
    }
    //return 0;
    out_language->merger = in_language->merger;
    // get first input instruction
    line = ass_pop_line(&progfile);
    // repeat until you've exhausted the file
    while ((!progfile.eof())||(line.length()!=0)){
      //cout << "mark = "<<mark<<"\n";
      //cout << "fail_trans = "<<fail_trans<<"\n";
      process_line(line,in_language,&progfile,out_language,pragmas,
		   &mark,&fail_trans);
      line = ass_pop_line(&progfile);
    }
    //cout << "finished processing\n";
    //out_language->print();
    // process "merged" instructions here.
    out_language->merge();
    //out_language->print();
    str_pr *tmp_pr = pragmas->head;
    while (tmp_pr){
      //cout << "pragma: "<< tmp_pr->left<< ":" <<tmp_pr->right<<"\n";
      if (tmp_pr->left == "univ_pred")
	out_language->predicate(tmp_pr->right);
      else if (tmp_pr->left == "init_val"){
	//cout << "found init_val :"<< tmp_pr->right<<endl;
	if (!out_language->set_val(tmp_pr->right)){
	  word1 = getword(tmp_pr->right);
	  word2 = stripword(tmp_pr->right);
	  out_language->head->vars->insert(word1,word2);
	}
      }
      else if (tmp_pr->left == "init_sig"){
	//	cout << "found init_sig :"<< tmp_pr->right<<endl;
	if (!out_language->set_val(tmp_pr->right)){
	  word1 = getword(tmp_pr->right);
	  word2 = stripword(tmp_pr->right);
	  cout << word1 <<":"<<word2<<":"<<endl;
	  out_language->head->sigs->print();
	  out_language->head->sigs->insert(word1,word2);
	  out_language->head->sigs->print();
	}
      }
      else if (tmp_pr->left == "init_rate"){
	//cout << "found init_rate :"<< tmp_pr->right<<endl;
	if (!out_language->set_rate(tmp_pr->right)){
	  word1 = getword(tmp_pr->right);
	  word2 = stripword(tmp_pr->right);
	  out_language->head->conts->insert(word1,word2);
	}
     }
      tmp_pr = tmp_pr->next;
    }
    //out_language->print();
    graph->update(out_language);
    delete pragmas;
    delete out_language;
    progfile.close();
  } 
  graph->print(outFname);
}
