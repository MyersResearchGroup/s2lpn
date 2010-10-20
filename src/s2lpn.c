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

void process_prags(env *curr_env){
  string word1,word2;
  str_pr *tmp_pr = curr_env->pragmas->head;
  while (tmp_pr){
    //cout << "pragma: "<< tmp_pr->left<< ":" <<tmp_pr->right<<"\n";
    if (tmp_pr->left == "univ_pred")
      curr_env->out_language->predicate(tmp_pr->right);
    else if (tmp_pr->left == "init_val"){
      //cout << "found init_val :"<< tmp_pr->right<<endl;
      if (!curr_env->out_language->set_val(tmp_pr->right)){
	word1 = getword(tmp_pr->right);
	word2 = stripword(tmp_pr->right);
	curr_env->out_language->head->vars->insert(word1,word2);
      }
    }
    else if (tmp_pr->left == "init_sig"){
      //	cout << "found init_sig :"<< tmp_pr->right<<endl;
      if (!curr_env->out_language->set_val(tmp_pr->right)){
	word1 = getword(tmp_pr->right);
	word2 = stripword(tmp_pr->right);
	cout << word1 <<":"<<word2<<":"<<endl;
	curr_env->out_language->head->sigs->print();
	curr_env->out_language->head->sigs->insert(word1,word2);
	curr_env->out_language->head->sigs->print();
      }
    }
    else if (tmp_pr->left == "init_rate"){
      //cout << "found init_rate :"<< tmp_pr->right<<endl;
      if (!curr_env->out_language->set_rate(tmp_pr->right)){
	word1 = getword(tmp_pr->right);
	word2 = stripword(tmp_pr->right);
	curr_env->out_language->head->conts->insert(word1,word2);
      }
    }
    tmp_pr = tmp_pr->next;
  }
}



int main(int argc, char *argv[]){
  // this is what we're building
  lhpn *graph = new lhpn;
  // input lines and parts thereof
  string line, prag, prag_args;
  // file names
  string langFname, progFname, outFname;
  string word1, word2;
  library *langs = new library;
  env *curr_env;
  env_stack *stack = new env_stack;
  int i;
  //determine name for output file
  if (argc >1){
    line = argv[1];
    if (line == "-o"){
    // if argv[1] is "-o", use argv[2] as output name
      outFname = argv[2];
      // make sure output has a ".lpn" terminator
      size_t suffix = outFname.find_last_of(".");
      if (suffix != outFname.npos){
	outFname = outFname.substr(0,suffix) + ".lpn";
      }
      else{
	outFname += ".lpn";
      }
      // advance past "-o outputfilename"
      i = 3;
    }
    else {
      // no outputfile was specified, use name of the first file instead.
      outFname = argv[1];
      outFname = outFname.substr(0,outFname.length()-2)+".lpn";
      i = 1;
    }
  }
  while(i<argc){
    //start to process a main file, create seed environment
    curr_env = new env;
    curr_env->pragmas = new pr_list;
    curr_env->out_language = new language;
    // extract file name from argv and open file
    progFname = argv[i++];
    cout << "opening file " << progFname << "\n";
    curr_env->progfile.open(progFname.c_str(),fstream::in);
    if (!curr_env->progfile.is_open()){
      cout << "failed to open file " << progFname <<endl;
      return 2;
    } 
    curr_env->out_language->filename = progFname;
    // file is open.  begin to process.
    // get first input instruction
    line = ass_pop_line(&(curr_env->progfile));
    // repeat until you've exhausted the file
    while ((curr_env!=NULL)&&(!curr_env->progfile.eof())||(line.length()!=0)){
      word1 = getword(line) ;
      // start using a new language
      if (word1 == "include"){
	// merge existing inputs (we may be changing merge codes)
	if (!(curr_env->out_language->empty())){
	  // process "merged" instructions here.
	  curr_env->out_language->merge();
	}
	// get new input language
	langFname = stripword(line);
	curr_env->in_language = get_language(langs,langFname);
	if (!(curr_env->in_language)){
	  return 1;
	}      
	curr_env->out_language->merger = curr_env->in_language->merger;
      }
      // start new input file
      else if (word1 == "input"){
	stack->push(curr_env);
	curr_env = new env(curr_env);
	curr_env->out_language = new language;
	//open input
	word2 = getword(stripword(line));
	cout << "opening file " << word2 << "\n";
	curr_env->progfile.open(word2.c_str(),fstream::in);
	if (!curr_env->progfile.is_open()){
	  cout << "failed to open file " << word2 <<endl;
	  return 2;
	} 	
      }
      else{
	// just another line.  Process it...
	process_line(line,curr_env->in_language,&curr_env->progfile,
		     curr_env->out_language,curr_env->pragmas,
		     &curr_env->mark,&curr_env->fail_trans);
      }
      // get the next line
      line = ass_pop_line(&curr_env->progfile);
      // if you finished a file
      if ((curr_env->progfile.eof())&&(line.length()==0)){
	//cout << "flushing...\n";
	// process pragmas
	process_prags(curr_env);
	// update lhpn
	graph->update(curr_env->out_language);
	// delete finished environment
	delete curr_env;
	// pop stack
	curr_env = stack->pop();
	if (curr_env){
	  // get new line from current envirionment
	  line = ass_pop_line(&curr_env->progfile);
	}
      }
    }
    cout << "finished processing\n";
  } 
  graph->print(outFname);
}
