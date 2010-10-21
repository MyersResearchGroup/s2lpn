#include "load.h"
using namespace std;

const int INFIN = 2147483647;

int lhpn::inst_num = 0, lhpn::pl_num = 0, lhpn::tr_num = 0;

int library::unique = 0;

string ass_pop_line(fstream *in_file){
  //cout << "ass_pop_line\n";
  string temp;
  unsigned int i, done = 0;
  // get first line
  getline(*in_file,temp);  
  while (!done){
    if (temp.length() == 0)
      return temp;
    // discard comment lines
    i = 0;
    //skip leading whitespace  
    while ((temp.at(i) == ' ') || (temp.at(i) == '\t')){
      i++;
      if (i == temp.length()){
	if (in_file->eof())
	  return "";
	getline(*in_file,temp);
	break;
      }
    }
    // test for comment characters, test for pragma characters
    if (temp.at(i) == ';'){ 
      if (temp.at(i+1) == '@'){
	return stripword(temp);
      }
      else{
	// get new line
	getline(*in_file,temp);
	if (in_file->eof())
	  return "";
	//	else 
	//cout << temp.length() << "|" << temp << "|\n";
      }
    }
    else{
      //cout << "found a non-comment line("<<i<<"):" << temp<<endl;
      done++;
    }
  }
  return temp;
}

string cpp_pop_line(fstream *in_file){
  string temp;
  int i, done = 0;
  // get first line
  getline(*in_file,temp);
  if (temp.length() == 0)
    return temp;
  // discard comment lines
  while (!done){
    i = 0;
    //skip leading whitespace  
    while ((temp.at(i) == ' ') || (temp.at(i) == '\t')){
      i++;
    }
    // test for comment characters
    if ((temp.at(i) == '/')&&(temp.at(i+1) == '/')){
      //get new line
      getline(*in_file,temp);
       if (temp.length() == 0)
	return temp;
    }
    else{
      //cout << "found a non-comment line("<<i<<"):" << temp<<endl;
      done++;
    }
  }
  return temp;
}

string upconvert(string in_string){
  int i,str_len = in_string.length();
  char t_str[256];
  const char *i_str =  in_string.c_str();
  for (i = 0;i<str_len;i++){
    t_str[i] = toupper(i_str[i]);
  }
  t_str[i] = 0;
  return t_str;
}

string getword(string in_string){ 
  //remove leading whitespace
  size_t i = in_string.find('\t');
  size_t j = in_string.find(' ');
  while ((i == 0)||(j==0)){
    in_string.erase(0,1);
    i = in_string.find('\t');
    j = in_string.find(' ');
  }
  if (((i<j)||(j == in_string.npos))&&(i != in_string.npos)){
    //cout << "found a tab\n";
    return in_string.substr(0,i);
  }
  else if (((i>j)||(i == in_string.npos))&&(j != in_string.npos)){
    //cout << "found a space\n";
    return in_string.substr(0,j);
  }
  return in_string;
}

string stripword(string in_string){
  string out_string;
  //remove leading whitespace
  size_t i = in_string.find('\t');
  size_t j = in_string.find(' ');
  while ((i == 0)||(j==0)){
    in_string.erase(0,1);
    i = in_string.find('\t');
    j = in_string.find(' ');
  }
  if (((i<j)||(j == in_string.npos))&&(i != in_string.npos))
    out_string = in_string.substr(i+1);
  else if (((i>j)||(i == in_string.npos))&&(j != in_string.npos))
    out_string = in_string.substr(j+1);
  else
    return "";
  //remove intermediate white space
  i = out_string.find('\t');
  j = out_string.find(' ');
  while ((i == 0)||(j==0)){
    out_string.erase(0,1);
    i = out_string.find('\t');
    j = out_string.find(' ');
  }
  return out_string;
}

int next_element(string in_string, int *pos, int *count, 
		 string delimiters, int nlimits){
  int length = in_string.length(), term = length - *pos;
  if (term == 0){
    //cout << "empty string\n";
    return 0;
  }
  while ((in_string.at(*pos) == ' ') || (in_string.at(*pos) == '\t')){
    //cout << "whitespace...\n";
    (*pos)++;
    if (*pos >= length)
      return 0;
  }
  term = length - *pos;
  // find first delimiting character
  for (*count = 0;*count < term;(*count)++){
     // found a comment:  ends line
    if (in_string.at((*pos)+(*count)) == ';'){
      if (in_string.at((*pos)+(*count)+1) != '@')
	return 0;
      else{
	// cout << "pos = "<<(*pos)<<"count ="<<(*count)<< "term = "<<term<<endl;
	(*pos)+=2;
	while ((in_string.at(*pos) == ' ') || (in_string.at(*pos) == '\t')){
	  //cout << "whitespace...\n";
	  (*pos)++;
	  if (*pos >= length)
	    return 0;
	}
	term = length - *pos;
	*count = 0;
 	// cout << "pos = "<<(*pos)<<"count ="<<(*count)<< "term = "<<term<<endl;
     }
    }
    for (int i = 0; i< nlimits; i++){
      if (in_string.at((*pos)+(*count)) == delimiters.at(i)){
	// found at least one non delimiting character
	if (*count){
	  return 1;
	}
	// if you found a delimiter at pos 0, it's the element you want...
	else {
	  (*count)++;
	  return 2;
	}
      }
    }
  }
  //fell off the end and found something...
  if (*count){
    return 1;
  }
  // search failed/strings are empty
  return 0;
}

string findNreplace(string str1, string str2, string str3){
  size_t pos,length = str1.length();
  //cout <<"replacing " << str1 << " with " << str3 << " in " << str2<<endl;
  while (string::npos != (pos = str2.find(str1)))
    str2 = str2.substr(0,pos) + str3 + str2.substr(pos+length);
  return str2;
}

int isntNumber(const char* str){
  char* testptr;
  strtod(str,&testptr);
  if (str == testptr)
    return 1;
  return 0;
}

str_pkg::str_pkg(string term){
  next = prev = NULL;
  curr = term;
}

void str_pkg::print(){
  cout << curr << endl;
}

str_pr::str_pr(string term1,string term2){
  next = NULL;
  prev = NULL;
  left = term1;
  right = term2;
}
 
void str_pr::print(){
  cout << left << " " << right << endl;
}

void str_list::insert(string command){
  str_pkg *term = new str_pkg(command);
  str_pkg *temp;
  if (head){
    temp = head;
    while (temp){
      if (temp->curr == command){
	//already there...
	delete term;
	return;
      }
      if (temp->curr > command){
	//found the following term;
	if (temp->prev){
	  temp->prev->next = term;
	}
	else {
	  head = term;
	}
	term->prev = temp->prev;
	term->next = temp;
	temp->prev = term;
	return;
      } 
      temp = temp->next;
    }
    //tail insertion
    tail->next = term;
    term->prev = tail;
    tail = term;
  }
  //empty list
  else {
    head = tail = term;
  }
}

void str_list::print(){
  str_pkg *temp = head;
  while(temp){
    temp->print();
    temp = temp->next;
  }   
}

int str_list::is_in(string term){
  str_pkg *temp = head;
  while(temp){
    if (term == temp->curr)
      return 1;
    temp = temp->next;
  }   
  return 0;
}

void str_list::substitute(str_pr *arg){
  str_pkg *temp = head;
  while(temp){
    if (temp->curr == arg->left){
      if (temp->next)
	temp->next->prev = temp->prev;
      else
	tail = temp->prev;
      if (temp->prev)
	temp->prev->next = temp->next;
      else
	head = temp->next;
      delete temp;
      insert(arg->right);
      return;
    }
    temp = temp->next;
  }   
}

void str_list::replace(str_pr *arg){
  str_pkg *temp = head;
  while(temp){
    temp->curr = findNreplace(arg->left,temp->curr,arg->right);
    temp = temp->next;
  }   
}

str_list::~str_list(){
  str_pkg *temp = head, *temp2;
  while(temp){
    temp2 = temp->next;
    delete temp;
    temp = temp2;
  }   
}

str_list::str_list(){
  head = tail = NULL;
}


str_list::str_list(str_list *old_set){
  head = tail = NULL;
  str_pkg *temp = old_set->head;
  while (temp){
    insert(temp->curr);
    temp = temp->next;
  }
}

bool pr_list::empty(){
  if ((head==tail) && (head==NULL))
    return true;
  return false;
}

void pr_list::insert(string term1,string term2){
  str_pr *term = new str_pr(term1, term2);
  str_pr *temp;
  if (head){
    temp = head;
    while (temp){
      if (temp->left == term1){
	//already there...
	if (temp->right == "X")
	  temp->right = term2;
	delete term;
	return;
      }
      if (temp->left > term1){
	//found the following term;
	if (temp->prev){
	  temp->prev->next = term;
	}
	else {
	  head = term;
	}
	term->prev = temp->prev;
	term->next = temp;
	temp->prev = term;
	return;
      } 
      temp = temp->next;
    }
    //tail insertion
    tail->next = term;
    term->prev = tail;
    tail = term;
  }
  //empty list
  else {
    head = tail = term;
  }
}

void pr_list::tail_insert(string term1,string term2){
  str_pr *term = new str_pr(term1, term2);
  if (head){
    //tail insertion
    tail->next = term;
    term->prev = tail;
    tail = term;
  }
  //empty list
  else {
    head = tail = term;
  }
}

// void pr_list::substitute(str_pr *arg){
//   str_pr *temp = head;
//   string temp_right;
//   while(temp){
//     if (temp->left == arg->left){
//       temp_right = temp->right;
//       if (temp->next)
// 	temp->next->prev = temp->prev;
//       else
// 	tail = temp->prev;
//       if (temp->prev)
// 	temp->prev->next = temp->next;
//       else
// 	head = temp->next;
//       delete temp;
//       insert(arg->right,temp->right);
//       return;
//     }
//     temp = temp->next;
//   }   
// }

void pr_list::substitute(str_pr *arg){
  str_pr *temp = head;
  string temp_right,temp_left;
  while(temp){
    if (((temp->left).find(arg->left))!=string::npos){
      //cout << "found "<< arg->left <<" in "<< temp->left << "\n";
      temp_left = findNreplace(arg->left,temp->left,arg->right);
      //cout <<temp_left<<"\n";
      temp_right = temp->right;
      if (temp->next)
	temp->next->prev = temp->prev;
      else
	tail = temp->prev;
      if (temp->prev)
	temp->prev->next = temp->next;
      else
	head = temp->next;
      delete temp;
      insert(temp_left,temp_right);
      temp = head;
    }
    else
      temp = temp->next;
  }   
}

str_pr *pr_list::lookup(string word){
  str_pr *temp = head;
  //cout << "comparing " << word <<"\n";
  while (temp){
    //cout << "  against "<<temp->left<<"\n";
    if (temp->left == word)
      break;
    temp = temp->next;
  }
  return temp;
}

void pr_list::print(){
  str_pr *temp = head;
  while(temp){
    temp->print();
    temp = temp->next;
  }   
}
pr_list::pr_list(){
    head = tail = NULL;
}
pr_list::pr_list(pr_list *old_set){
  head = tail = NULL;
  str_pr *temp = old_set->head;
  while (temp){
    insert(temp->left,temp->right);
    temp = temp->next;
  }
}

pr_list::~pr_list(){
  str_pr *t1= head, *t2;
  while (t1){
    t2 = t1->next;
    delete t1;
    t1 = t2;
  }
}

void transition::insert(string command){
  assigns->insert(command);
}

void transition::bool_insert(string command){
  bool_assigns->insert(command);
}

void transition::rate_insert(string command){
  rate_assigns->insert(command);
}

void transition::merge(transition *mergee){
  str_pkg *temp = mergee->assigns->head;
  while (temp){
    assigns->insert(temp->curr);
    temp = temp->next;
  }
  temp = mergee->bool_assigns->head;
  while (temp){
    bool_assigns->insert(temp->curr);
    temp = temp->next;
  }
  temp = mergee->rate_assigns->head;
  while (temp){
    rate_assigns->insert(temp->curr);
    temp = temp->next;
  }
}

void transition::replace(str_pr *arg){
  label = findNreplace(arg->left,label,arg->right);
  place = findNreplace(arg->left,place,arg->right);
  priority = findNreplace(arg->left,priority,arg->right);
  enabling = findNreplace(arg->left,enabling,arg->right);
  if ((enabling == "TRUE")||(enabling =="true"))
    enabling = "";
  //must change to do search and replace
  lower = findNreplace(arg->left,lower,arg->right);
  upper = findNreplace(arg->left,upper,arg->right);
//   if (lower == arg->left)
//     lower = arg->right;
//   if (upper == arg->left)
//     upper = arg->right;
  assigns->replace(arg);
  bool_assigns->replace(arg);
  rate_assigns->replace(arg);
}

transition::transition(string expr, string low, string up){
  next = NULL;
  lower = low;
  upper = up;
  enabling = expr;
  assigns = new str_list;
  bool_assigns = new str_list;
  rate_assigns = new str_list;
  label = "";
  place = "";
  priority = "";
}

transition::transition(transition *old_trans){
  next = NULL;
  label = old_trans->label;
  place = old_trans->place;
  priority = old_trans->priority;
  enabling = old_trans->enabling;
  lower = old_trans->lower;
  upper = old_trans->upper;
  assigns = new str_list(old_trans->assigns);
  bool_assigns = new str_list(old_trans->bool_assigns);
  rate_assigns = new str_list(old_trans->rate_assigns);
}

void transition::predicate(string pred){
  if (enabling == "")
    enabling = pred;
  else {
    enabling = pred + "&(" + enabling + ")";
  }
}

void transition::prioritize(string exp){
  priority=exp;
}

void transition::add_label(string str){
  label=str;
}
void transition::add_place(string str){
  place=str;
}

void transition::delay(string l_b, string u_b){
  lower=l_b;
  upper=u_b;
}

transition::~transition(){
  if (assigns)
    delete assigns;
    delete bool_assigns;
    delete rate_assigns;
}

void transition::print(){
  cout << "{" << enabling << "}" << endl;
  if (upper != "")
    cout << '[' << lower << ',' << upper << "]\n";
  else 
    cout << lower << endl;
  if (priority != "")
    cout << "priority "<< priority << endl;
  if (label != "")
    cout << "label "<< label << endl;
  cout << "<\n";
  //chain through assignments
  cout << "assignments" << endl;
  if (assigns)
    assigns->print();
  cout << "boolean assignments" << endl;
  if (bool_assigns)
    bool_assigns->print();
  cout << "rate assignments" << endl;
  if (rate_assigns)
    rate_assigns->print();
  cout << ">\n";
}

void leg::replace(str_pr *arg){
  transition *temp = head;
  label = findNreplace(arg->left,label,arg->right);
  target = findNreplace(arg->left,target,arg->right);
  while (temp){
    temp->replace(arg);
    temp = temp->next;
  }
}

void leg::print(){
  cout //<< "NEW LEG:\n"
       << label<<endl
       << target<<endl;
  transition* temp = head;
  //cout <<"TRANSITIONS:\n";
  while (temp) {
    temp->print();
    temp = temp->next;
  }
}

void leg::predicate(string pred){
  transition* temp = head;
  while (temp) {
    temp->predicate(pred);
    temp = temp->next;
  }
}


void leg::insert(transition *term){
  if (head)  {
    tail->next = term;
    tail = term;
  }
  else {
    head = tail = term;
  }
}

leg::leg(string name, string dest){
  label = name;
  target = dest;
  head = tail = NULL;
  next = NULL;
}

leg::leg(leg *old_leg){
  label = old_leg->label;
  target = old_leg->target;
  head = tail = NULL;
  next = NULL;
  transition *temp = old_leg->head;
  while (temp){
    insert(new transition(temp));
    temp = temp->next;
  }
}

leg::~leg(){ 
  transition *t2, *temp = head;
  while (temp) {
    t2 = temp->next;
    delete temp;
    temp = t2;
  }
}

int candidate::match(string check){
  if (check == choice)
    return 1;
  if (next == NULL)
    return 0;
  return next->match(check);
}

candidate::~candidate(){
  if (next != NULL)
    delete next;
}

void candidate::print(){
  cout << choice << " ";
  if (next != NULL)
    next ->print();
}

candidate::candidate(string new_choice){
  choice = new_choice;
  next = NULL;
}

void constraint::add_candidate(string new_cand){
  if (head == NULL)
    head = tail = new candidate(new_cand);
  else {
    tail->next = new candidate(new_cand);
    tail = tail->next;
  }
}

void constraint::print(){
  if (bound == 1){
    cout << variable << " BOUND " << lower << " "  << upper << endl;
  }
  else {
    cout << variable << " MATCH ";
    head->print();
    cout << endl;
  }
  if (next != NULL)
    next->print();
}

constraint::~constraint(){
  if (head){
    delete head;
  }
  if (next){
    delete next;
  }
}

constraint::constraint(string newvar, int lbound, int ubound){
  bound = 1;
  variable = newvar;
  lower = lbound;
  upper = ubound;
  head = tail = NULL;
  next = NULL;
}

constraint::constraint(string newvar, string choices, int base){
  int step1,step2;
  string cand;
  // set var and type
  bound = 0;
  variable = newvar;
  lower = 0;
  upper = 0;
  head = tail = NULL;
  next = NULL;
  step1 = base;
  // peel off candidates and add to set
  while (step1 != choices.npos +1){
    step2 = choices.find(" ",step1);
    cand = choices.substr(step1,step2-step1);
    add_candidate(cand);
    step1 = step2+1;
  }
}

// search and replace all left elements with right elements
void instruction::replace(pr_list *args){
  str_pr *t_pr = args->head;
  while (t_pr){
    vars->substitute(t_pr);
    sigs->substitute(t_pr);
    conts->substitute(t_pr);
    pattern = findNreplace(t_pr->left, pattern, t_pr->right);
    leg * t_leg = first;
    while (t_leg){
      t_leg->replace(t_pr);
      t_leg = t_leg->next;
    }
    t_pr = t_pr->next;
  }
}

void instruction::print(){
  if (label != "")
    cout <<label<<" ";
  if (mnemonic == "")
    cout << "THIS IS  A BOGUS INSTRUCTION\n";
  cout << mnemonic <<" " << pattern<<"\n";
  if (marked)
    cout << "marked\n";
  if (head != NULL){
    cout << "CONSTRAINTS:\n";
    head->print();
  }
  cout << "variables: \n";
  vars->print();
  cout << "signals: \n";
  sigs->print();
  cout << "continuous variables: \n";
  conts->print();
  cout << "LEGS:\n";
  leg *temp = first;
  while (temp){
    temp->print();
    temp = temp->next;
  }
  cout <<"\n";
}

instruction::~instruction(){
  //delete constraints
  if (head != NULL){
    delete head;
  }
  //  cout << "processing vars\n";
  delete vars;
  //cout << "processing sigs\n";
  delete sigs;
  //cout << "processing conts\n";
  delete conts;
  //cout << "processing legs\n";
  leg *temp = first, *t2;
  while (temp){
    t2 = temp->next;
    delete temp;
    temp = t2;
  }
}

void instruction::predicate(string pred){
  leg *temp = first;
  while (temp){
    temp->predicate(pred);
    temp = temp->next;
  }
}

int instruction::merge(string test){
  //cout << "current instruction:\n";
  //this->print();
  if ((next ==NULL) || (next->first->head->enabling != test)){
    return 0;
  }
  //change link target
  first->target = next->first->target;
  // merge declarations
  str_pr *tmp = next->sigs->head;
  while (tmp){
    sigs->insert(tmp->left,tmp->right);
    tmp = tmp->next;
  }
  tmp = next->vars->head;
  while (tmp){
    vars->insert(tmp->left,tmp->right);
    tmp = tmp->next;
  }
  tmp = next->conts->head;
  while (tmp){
    conts->insert(tmp->left,tmp->right);
    tmp = tmp->next;
  }
  // merge assignments
  first->tail->merge(next->first->tail);
  //delete merged instruction
  instruction *temp = next;
  next = next->next;
  if (next)
    next->prev = this;
  delete temp;
  return 1;
}

int instruction::set_val(string term){
  string word1 = getword(term), word2 = stripword(term);
  //cout << mnemonic<< " "<<pattern << " setting "<< word1 << " to  " << word2 << "\n";
  str_pr *tmp_pr = vars->lookup(word1);
  if (!tmp_pr){
    //cout <<"set_val: "<< word1 << " wasn't a variable!\n";
    return 0;
  }
  //cout << "found "<<word1<<"\n";
  tmp_pr->right = word2;
  return 1;
}

int instruction::set_sig(string term){
  string word1 = getword(term), word2 = stripword(term);
  //cout << mnemonic<< " "<<pattern << " setting "<< word1 << " to  " << word2 << "\n";
  str_pr *tmp_pr = sigs->lookup(word1);
  if (!tmp_pr){
    //cout <<"set_sig: " << word1 << " wasn't a signal!\n";
    return 0;
  }
  //cout << "found "<<word1<<"\n";
  tmp_pr->right = word2;
  return 1;
}

int instruction::set_rate(string term){
  string word1 = getword(term), word2 = stripword(term);
  //cout << "setting "<< word1 << " to  " << word2 << "\n";
  str_pr *tmp_pr = conts->lookup(word1);
  if (!tmp_pr){
    //cout <<"set_rate: " << word1 << " wasn't a continuous variable!\n";
    return 0;
  }
  tmp_pr->right = word2;
  return 1;
}

void instruction::add_leg(leg* new_leg){
  if (first==NULL)
    first = last = new_leg;
  else {
    last->next = new_leg;
    last = new_leg;
  }
}

void instruction::add_vars(string var_list){
  string word;
  // read type
  word = getword(var_list);
  var_list = stripword(var_list);
  if (word == "#i"){
    // cout << "integer line\n";
    word = getword(var_list);
    var_list = stripword(var_list);
    while (word != ""){
      vars->insert(word,"undef");
      word = getword(var_list);
      var_list = stripword(var_list);
    }
  }
  if (word == "#b"){
    // cout << "boolean line\n";
    word = getword(var_list);
    var_list = stripword(var_list);
    while (word != ""){
      //cout << "stripping "<< word << "\n";
      sigs->insert(word,"X");
      word = getword(var_list);
      var_list = stripword(var_list);
    }
    //sigs->print();
  }
  if (word == "#r"){
    // cout << "real line\n";
    word = getword(var_list);
    var_list = stripword(var_list);
    while (word != ""){
      vars->insert(word,"undef");
      conts->insert(word,"0");
      word = getword(var_list);
      var_list = stripword(var_list);
    }
  }
  if (word == "#u"){
    // cout << "unique line\n";
    word = getword(var_list);
    var_list = stripword(var_list);
    while (word != ""){
      uniques->insert(word,"undef");
      word = getword(var_list);
      var_list = stripword(var_list);
    }
  }
}

void instruction::add_constraint(string new_constraint){
  string var, type, lbound, ubound;
  int step1, step2,lower, upper;
  constraint *temp;
  //  peel out var and type.  
  step1 = 0;
  step2 = new_constraint.find(" ",step1);
  var = new_constraint.substr(step1,step2-step1);
  step1 = step2+1;
  step2 = new_constraint.find(" ",step1);
  type = new_constraint.substr(step1,step2-step1);
  // bounded integer; peel bounds
  if (type == "BOUND"){
    step1 = step2+1;
    step2 = new_constraint.find(" ",step1);
    lbound = new_constraint.substr(step1,step2-step1);
    step1 = step2+1;
    step2 = new_constraint.find(" ",step1);
    ubound = new_constraint.substr(step1,step2-step1);
    lower = atoi(lbound.c_str());
    upper = atoi(ubound.c_str());
    temp = new constraint(var,lower,upper);
    temp->str = new_constraint;
  }
  else {
    // send forward string of candidate values
    temp = new constraint(var,new_constraint,step2+1);
    temp->str = new_constraint;
  }
  // shouldn't be possible
  if (temp == NULL) return;
  // add to constraint set;
  if (head == NULL){
    head = tail = temp;
  }
  else {
    tail->next = temp;
    tail = temp;
  }
}

//empty constructor
instruction::instruction(string instr, string args){
  label = "";
  mnemonic = instr;
  pattern = args;
  marked = 0;
  failed = 0;
  first = last = NULL;
  head =  tail = NULL;
  next = prev = NULL;
  sigs = new pr_list;
  vars = new pr_list;
  conts = new pr_list;
  uniques = new pr_list;
}

//copy constructor
instruction::instruction(instruction *old_inst){
  label = old_inst->label;
  mnemonic = old_inst->mnemonic;
  pattern = old_inst->pattern;
  marked = old_inst->marked;
  failed = old_inst->failed;
  first = last = NULL;
  next = prev = NULL;
  head =  tail = NULL;
  //copy constraints
  constraint *old_const = old_inst->head;
  while (old_const){
    add_constraint(old_const->str);
    old_const = old_const->next;
  }
  //copy legs
  leg *old_leg = old_inst->first;
  while (old_leg){
    add_leg(new leg(old_leg));
   old_leg = old_leg->next;
  }
  sigs = new pr_list(old_inst->sigs);
  vars = new pr_list(old_inst->vars);
  conts = new pr_list(old_inst->conts);
  uniques = new pr_list(old_inst->uniques);
}

void language::insert(instruction *new_inst){
  instruction *temp;
  //empty insertion
  if (head == NULL)
    head = tail = new_inst;
  else{
    //lexically ordered insertion:
    //head insertion:
    if (head->mnemonic > new_inst->mnemonic){
      head->prev = new_inst;
      new_inst->next = head;
      head = new_inst;
    }
    else {
      temp = head;
      while ((temp != NULL) &&(temp->mnemonic <= new_inst->mnemonic))
	temp = temp->next;
      //found first element after new_inst
      if (temp != NULL){
	new_inst->next = temp;
	new_inst->prev = temp->prev;
	temp->prev->next = new_inst;
	temp->prev = new_inst;
      }
      //ran off end; tail insertion
      else {
	tail->next = new_inst;
	new_inst->prev = tail;
	tail =  new_inst;
      }
    }
  }
}

void language::tail_insert(instruction *new_inst){
  //empty insertion
  if (head == NULL)
    head = tail = new_inst;
  else{
    //started with tail insertion, know it works...
    tail = tail->next = new_inst;
    return;
  }
}

void language::print(){
  instruction *temp = head;
  int i,j;
  cout << "LANGUAGE("<<filename<<"):\n";
  cout << "delimiters["<< nlimits <<"]: ";
  for  (i = 0, j = delimiters.length();i<j;i++){
    switch (delimiters.at(i)){
    case ' ':
      cout << "\\ ";
      break;
    case '\t':
      cout << "\\t";
      break;
    default:
      cout << delimiters.at(i);
    }
  }
  cout <<endl;
  cout << "merge code = "<<merger<<endl;
  while (temp){
    temp->print();
    temp = temp->next;
  }
}

language::~language(){
  instruction *temp = head,*t2;
  while (temp){
    t2 = temp->next;
    delete temp;
    temp = t2;
  }
}

void language::predicate(string pred){
  instruction *temp = head;
  while (temp){
    temp->predicate(pred);
    temp = temp->next;
  }
}

void language::merge(){
  instruction *temp = head;
  if (merger != ""){
    while (temp){
      if (!(temp->merge(merger)))
	temp = temp->next;
    }
  }
}

int language::set_val(string term){
  instruction *temp = head;
  int r_val = 0;
  while (temp){
    if (temp->set_val(term))
      r_val++;
    temp = temp->next;
  }
  return r_val;
}

int language::set_sig(string term){
  instruction *temp = head;
  int r_val = 0;
  while (temp){
    if (temp->set_sig(term))
      r_val++;
    temp = temp->next;
  }
  return r_val;
}

int language::set_rate(string term){
  instruction *temp = head;
  int r_val = 0;
  while (temp){
    if (temp->set_rate(term))
      r_val++;
    temp = temp->next;
  }
  return r_val;
}

bool language::empty(){
  if (head==NULL)
    return true;
  return false;
}


language::language(){
  delimiters = "";
  filename = "";
  merger = "";
  nlimits = 0;
  head = tail = NULL;
  next = NULL;
}
  
instruction *language::find_inst(string command, string args){
  // unique identifier
  char unid[32]; 
  // iterator
  instruction *temp = head;
  // return value
  instruction *r_val =  NULL;
  // string processing values
  int pos1_1, pos1_2, pos2_1, pos2_2, res1, res2;
  // list of command arguments
  pr_list *arg_list;
  // string lengths
  int l1 = args.length(),l2;
  // flag
  int failed;
  // parsed substrings
  string str_1,str_2;
  while (temp){
    failed = 1;
    // look for matching mnemonic
    if (temp->mnemonic == command){
/*       temp->print(); */
/*       cout << temp->mnemonic << "\n"; */
/*       cout << "matching |" << temp->pattern <<"| |"<<args<<"|\n";  */
      pos1_1 = 0;
      pos2_1 = 0;
      l2 = temp->pattern.length();
      //check for argument match
      failed = 0;
      arg_list = new pr_list;
      while (!failed){
	// Both strings are empty?
	if ((pos1_1 >= l1) && (pos2_1 >= l2)){
	  break;
	}
	//get next argument pair
	res1 = next_element(args,&pos1_1,&pos1_2,delimiters,nlimits);
	str_1 = args.substr(pos1_1,pos1_2);
	
	res2 = next_element(temp->pattern,&pos2_1,&pos2_2,delimiters,nlimits);
	str_2 = temp->pattern.substr(pos2_1,pos2_2);
	//cout << "str_1 = "<< str_1 << " str_2 = "<< str_2 << endl;
	pos1_1 += pos1_2;
	pos2_1 += pos2_2;
	//reached end of both of the strings?
	if (!res1 && !res2)
	  break;
	//reached end of only one of the strings?
	if ((!res1 && res2) || (res1 && !res2)){
	  failed++;
	  break;
	}
	// check for string inequality
	if (str_1 !=str_2){
	  // check for str_2 = @NUM and str_1 is not a delimiter
	  //cout << "gothere "<<str_1<<" "<<str_2<<" "<<res1<<" "<<res2<<"\n";
	  if ((res1 == 1) && (str_2.at(0) == '@')){
	      arg_list->insert(str_2,str_1);
	  }//fi
	  else {
	    delete arg_list;
	    //found an arg mismatch; need to break out...
	    failed++;
	  }//esle
	}//fi
      }//elihw
    } //fi
    if (failed){
      temp = temp->next;
    }
    else
      break;
  }//elihw
  if (temp){
    //create copy
    r_val = new instruction(temp);
    if (!r_val->uniques->empty()){ 
      str_pr *tmp = r_val->uniques->head;
      while (tmp){
	//cout << "adding unique ID " << library::unique <<"\n";
	sprintf(unid,"%d",(library::unique)++);
	arg_list->insert(tmp->left,unid);
	tmp=tmp->next;
      }
    }
    // substitute string pairs
    r_val->replace(arg_list);
    delete arg_list;
    // return copy...
  }
  return r_val;
}

language *load_language(string fname){
  language *in_language = new language();
  string mnemonic, pattern,temp,t1,t2,t3,priority,place;
  instruction *inst;
  leg *curr_leg;
  transition *curr_trans;
  int lower, upper, inst_done;
  //open input file
  fstream instfile(fname.c_str(),fstream::in);
  if (!instfile.is_open()){
    cout << "failed to open file " << fname <<endl;
    return NULL;
  } 
  if (!instfile.eof()){
    //load delimiters
    temp = cpp_pop_line(&instfile);
    upper = temp.length();
    in_language->delimiters = "";
    for (lower = 0;lower < upper;lower++){
      if (temp.at(lower) == '\\'){
	switch (temp.at(lower+1)){
	case 't':
	  in_language->delimiters += '\t';
	  lower++;
	  break;
	case ' ':
	  in_language->delimiters += '\040';
	  lower++;
	  break;
	}
      }
      else {
	in_language->delimiters += temp.at(lower);
      }
    }
    in_language->nlimits = in_language->delimiters.length();
  }  
  temp = cpp_pop_line(&instfile);
  if (temp != ""){
    //    cout << "found merger = "<< temp<<endl;
    in_language->merger = temp;
    temp = cpp_pop_line(&instfile);
  }
  while (!instfile.eof()){
    // read instruction
    mnemonic = cpp_pop_line(&instfile);
    if (mnemonic == "")
      continue;
    // read operand pattern
    pattern = cpp_pop_line(&instfile);
    inst = new instruction(upconvert(mnemonic), pattern);
    // get pattern constraints
    while (instfile.peek() == '@'){
      //get argument constraints
      temp = cpp_pop_line(&instfile);
      inst->add_constraint(temp);
    }
    while (instfile.peek() == '#'){
      //get variables.  
      temp = cpp_pop_line(&instfile);
      //cout << "adding vars: " << temp << "\n";
      inst->add_vars(temp);
    }
    //loop reading each leg
    inst_done = 0;
    //read leg label
    temp = cpp_pop_line(&instfile);
    //read leg destination
    t2 = cpp_pop_line(&instfile);
    while (!inst_done) {
      curr_leg = new leg(temp,t2);
      // loop reading transitions
      while (instfile.peek() == '{') {
	//read enabling condition 
	temp = cpp_pop_line(&instfile);
	lower = temp.find("{");
	upper = temp.find("}");
	t1 = temp.substr(lower+1,upper-lower-1);
	// insert default 
	t2 = "0";
	t3 = "INF";
	//build new transition
	curr_trans = new transition(t1,t2,t3);
	curr_leg->insert(curr_trans);
	//add new elements here
	temp = cpp_pop_line(&instfile);
	while (temp != "<") {
	  //cout << "temp = " << temp<<endl;
	  if ((temp.length()>0) && (temp.at(0) == '[')){
	    // parse timing bounds
	    lower = temp.find("[");
	    upper = temp.find(",");
	    t2 = temp.substr(lower+1,upper-lower-1);
	    lower = upper;
	    upper = temp.find("]");
	    t3 = temp.substr(lower+1,upper-lower-1);
	    curr_trans->delay(t2,t3);
	  }
	  else if (getword(temp) == "priority"){
	    priority = getword(stripword(temp));
	    // cout << "adding priority " << priority <<endl;
	    curr_trans->prioritize(priority);
	  }
	  else if (getword(temp) == "place"){
	    place = getword(stripword(temp));
	    // cout << "adding place label  " << place <<endl;
	    curr_trans->add_place(place);
	  } 
	  else {
	    t2 = getword(temp);
	    t3 = "";
	    curr_trans->delay(t2,t3);
	  }
	  temp = cpp_pop_line(&instfile);
	}
	//start assignment reading
	// 	temp = cpp_pop_line(&instfile);
	// 	// assignment list should start with "<" line
	// 	if (temp != "<")
	// 	  cout <<"parse error:"<<temp<<"\n";
	temp = cpp_pop_line(&instfile);
	// assignment list should end with ">" line
	while (temp != ">"){
	  //check for boolean assignments
	  if ((temp.at(0) == '#')&&(temp.at(1) == 'b')){
	    curr_trans->bool_insert(temp.substr(3));
	  }
	  else if ((temp.at(0) == '#')&&(temp.at(1) == 'r')){
	    curr_trans->rate_insert(temp.substr(3));
	  }
	  else{
	    curr_trans->insert(temp);
	  }
	  temp = cpp_pop_line(&instfile);
	}
	//if no assignments, the "<" and ">" lines should still be there
      }
      inst->add_leg(curr_leg);
      //read leg label
      temp = cpp_pop_line(&instfile);
      // empty line separates instructions
      if ((temp == "") || (instfile.eof()))
	inst_done = 1;
      else //get next leg target
	t2 = cpp_pop_line(&instfile);	
    }    
    in_language->insert(inst);
  }
  instfile.close();
  return in_language;
}

language *library::lookup(string fname){
  language *temp = head;
  while (temp){
    if (temp->filename == fname)
      break;
    temp = temp->next;
  }
  if (temp){
    // cout << "found " << temp->filename <<"\n";
  }
  return temp;
}

void library::insert(language *new_lang){
  if (head){
    tail->next = new_lang;
    tail = new_lang;
  }
  else {
    head = tail = new_lang;
  }
}

library::library(){
  head = tail = NULL;
}

library::~library(){
  language *temp = head,*t2;
  while (temp){
    t2 = temp->next;
    delete temp;
    temp = t2;
  }
}

void lhpn::print(string l_name){
  ofstream l_file;
  transition *temp;
  str_pkg *tmp_str;
  str_pr *tmp_pr;
  string builder;
  int init;
  l_file.open(l_name.c_str());
  if (!l_file.is_open()){
    cout << "did not open\n";
    return;
  }
  // outputs
  if (sigs->head){
    l_file << ".outputs";
    tmp_pr = sigs->head;
    while (tmp_pr){
      l_file << " " << tmp_pr->left;
      tmp_pr = tmp_pr->next;
    }
    l_file << endl;
  }
  // dummy transitions
  l_file << ".dummy";
  temp = head;
  while (temp != NULL){
    l_file << " " << temp->label;
    temp = temp->next;
  }
  l_file << "\n";
  // variables
  if (vars->head){
    l_file << "#@.variables";
    tmp_pr = vars->head;
    while (tmp_pr){
      l_file << " " << tmp_pr->left;
      tmp_pr = tmp_pr->next;
    }
    l_file << endl;
  }
  // fail_set
  if (fail_set->head){
    l_file << "#@.failtrans ";
    tmp_str = fail_set->head;
    while (tmp_str != NULL){
      l_file << " " << tmp_str->curr;
      tmp_str = tmp_str->next;
    }
     l_file << " \n";
  }
  // init state 
  if (sigs->head){
    l_file << "#@.init_state [";
    tmp_pr = sigs->head;
    while (tmp_pr){
      if (upconvert(tmp_pr->right) == "FALSE")
	l_file <<  "0";
      else if (upconvert(tmp_pr->right) == "TRUE")
	l_file <<  "1";
      else 
	l_file <<  tmp_pr->right;
      tmp_pr = tmp_pr->next;
    }
    l_file <<"]"<< endl;
  }
  // places
  if (places->head){
    l_file << "#|.places";
    tmp_str = places->head;
    while (tmp_str){
      l_file << " " << tmp_str->curr;
      tmp_str = tmp_str->next;
    }
    l_file << endl;
  }
  // flow relation
  l_file << ".graph\n";
  tmp_pr = flow->head;
  while (tmp_pr != NULL){
    l_file << tmp_pr->left << " " << tmp_pr->right <<"\n";
    tmp_pr = tmp_pr->next;
  }
  // marking
  if (marking->head){
    l_file << ".marking {";
    tmp_str = marking->head;
    while (tmp_str != NULL){
      l_file << " " << tmp_str->curr;
      tmp_str = tmp_str->next;
    }
    l_file << " }\n";
  }
  // init vals 
  init = 0;
  tmp_pr = vars->head;
  while (tmp_pr){
    if (tmp_pr->right != "undef"){
      if (!init){
	l_file  << "#@.init_vals {";
	init++;
      }
      l_file << "<" <<tmp_pr->left << "=" << tmp_pr->right <<">";
    }
    tmp_pr = tmp_pr->next;
  }
  if (init){
    l_file  << "}\n";
    init--;
  }
  // init rates
  init = 0;
  tmp_pr = conts->head;
  while (tmp_pr){
    if (tmp_pr->right != "undef"){
      if (!init){
	l_file  << "#@.init_rates {";
	init++;
      }
      l_file << "<" <<tmp_pr->left << "=" << tmp_pr->right <<">";
    }
    tmp_pr = tmp_pr->next;
  }
  if (init){
    l_file  << "}\n";
    init--;
  }
  // enablings
  temp = head;
  builder = "";
  while (temp != NULL){
    if (temp->enabling != ""){
      builder += "<" + temp->label + "=["+temp->enabling+"]>";
    }
    temp = temp->next;
  }
  if (builder!=""){
    l_file << "#@.enablings {";
    l_file << builder;
    l_file << "}\n";
  }
  // assignments
  builder = "";
  temp = head;
  while (temp != NULL){
    tmp_str = temp->assigns->head;
    while (tmp_str != NULL){
      builder += "<" + temp->label + "=["+tmp_str->curr+"]>";
      tmp_str = tmp_str->next;
    }
    temp = temp->next;
  }
  if (builder!=""){
    l_file << "#@.assignments {";
    l_file << builder;   
    l_file << "}\n";
  }
  // boolean assignments
  builder = "";
  temp = head;
  while (temp != NULL){
    tmp_str = temp->bool_assigns->head;
    while (tmp_str != NULL){
      //cout << "varname = |"<<tmp_str->curr<<"|\n";
      builder += "<" + temp->label + "=["+tmp_str->curr+"]>";
      tmp_str = tmp_str->next;
    }
    temp = temp->next;
  }
  if (builder!=""){
    l_file << "#@.boolean_assignments {";
    l_file << builder;   
    l_file << "}\n";
  }
  // rate_assignments
  builder = "";
  temp = head;
  while (temp != NULL){
    tmp_str = temp->rate_assigns->head;
    while (tmp_str != NULL){
      builder += "<" + temp->label + "=["+tmp_str->curr+"]>";
      tmp_str = tmp_str->next;
    }
    temp = temp->next;
  }
  if (builder!=""){
    l_file << "#@.rate_assignments {";
    l_file << builder;   
    l_file << "}\n";
  }
  // delay_assignments
  l_file << "#@.delay_assignments {";
  temp = head;
  while (temp != NULL){
    l_file << "<" << temp->label;
    l_file << "=[uniform(" << temp->lower<<","<<temp->upper<<")]>";
    temp = temp->next;
  }
  l_file << "}\n";
  // continuous
  if (conts->head){
    l_file << "#@.continuous";
    tmp_pr = conts->head;
    while (tmp_pr){
      l_file << " " << tmp_pr->left;
      tmp_pr = tmp_pr->next;
    }
    l_file << endl;
  }
  l_file << ".end\n";
  l_file.close();
};

void lhpn::insert(transition *new_node){
  if (head == NULL){
    head = tail = new_node;
  }
  else{
    tail = tail->next = new_node;
  }
};

void lhpn::update(language *prog){
  if (prog == NULL){
    cout << "NULL Language!!\n";
    return;
  }
  //markers for places, instructions, and transitions
  string t = "t",p="p",i="i";
  char t_num[255], p_num[255], i_num[255];
  string curr_inst_label, next_inst_label;
  string curr_label, next_label, trans_label;
  //iterators
  instruction *curr_inst = prog->head;
  leg *curr_leg;
  transition *curr_trans;
  str_pr *tmp_pr;
  if (curr_inst->label == ""){
    sprintf(i_num,"%d",inst_num++);
    curr_inst->label = next_inst_label = i + i_num;
  }    
  //marking->insert(curr_inst->label);
  // loop over instructions
  while (curr_inst){
    //cout << curr_inst->label<<" "<<curr_inst->mnemonic<<" "<<curr_inst->pattern<<"\n";
    //update vars;
    if (curr_inst->marked)
      marking->insert(curr_inst->label);
    tmp_pr = curr_inst->vars->head;
    while (tmp_pr){
      if (isntNumber(tmp_pr->left.c_str()))
	vars->insert(tmp_pr->left,tmp_pr->right);
      //      else
      //	cout << "discarding var " << tmp_pr->left << "\n";
      tmp_pr = tmp_pr->next;
    }
    //update sigs;
    tmp_pr = curr_inst->sigs->head;
    while (tmp_pr){
      if (isntNumber(tmp_pr->left.c_str()))
	sigs->insert(tmp_pr->left,tmp_pr->right);
      //      else
      //	cout << "discarding sig " << tmp_pr->left << "\n";
      tmp_pr = tmp_pr->next;
    }
    tmp_pr = curr_inst->conts->head;
    while (tmp_pr){
      if (isntNumber(tmp_pr->left.c_str()))
	conts->insert(tmp_pr->left,tmp_pr->right);
      //else
      //	cout << "discarding cont " << tmp_pr->left << "\n";
      tmp_pr = tmp_pr->next;
    }
    //create place; if instruction doesn't have a name, create an iX
    curr_inst_label = curr_inst->label;
    if (curr_inst_label == ""){
      sprintf(i_num,"%d",inst_num++);
      curr_inst_label = i + i_num;
    }
    places->insert(curr_inst_label);
    // loop over legs
    curr_leg = curr_inst->first;
    while (curr_leg){
      // set up end transition target
      next_inst_label = curr_leg->target;
      // @next is placeholder for no branch
      if ((next_inst_label == "@next") && (curr_inst->next)){
	next_inst_label = curr_inst->next->label;
	if (next_inst_label == ""){
	  sprintf(i_num,"%d",inst_num++);
	  curr_inst->next->label = next_inst_label = i + i_num;
	}
      }
      // @first is placeholder for first instruction (RTI)
      if (next_inst_label == "@first"){
	next_inst_label = prog->head->label;
      }
      // loop over transitions
      curr_trans = curr_leg->head;
      curr_label = curr_inst_label;
      sprintf(t_num,"%d",tr_num++);
      trans_label = t + t_num;
      if (curr_trans->place != ""){
	next_label = curr_trans->place;
      }
      else if (curr_trans->next){
	sprintf(p_num,"%d",pl_num++);
	next_label = p + p_num;
      }
      else{
	next_label = next_inst_label;
      }
      while (curr_trans){
	if (curr_label != curr_inst_label)
	  places->insert(curr_label);
	// create new place
	curr_trans->label = trans_label;
	insert(new transition(curr_trans));
	flow->tail_insert(curr_label,trans_label);
	//cout << trans_label << "->" << next_label <<endl;
	if (next_label != "@next")
	  flow->tail_insert(trans_label,next_label);
	// set up next iteration
	curr_trans = curr_trans->next;
	if (curr_trans){
	  curr_label = next_label;
	  sprintf(t_num,"%d",tr_num++);
	  trans_label = t + t_num;
	  if (curr_trans->place != ""){
	    next_label = curr_trans->place;
	  }
	  else if (curr_trans->next){
	    sprintf(p_num,"%d",pl_num++);
	    next_label = p + p_num;
	  }
	  else{
	    next_label = next_inst_label;
	  }
	}
      }
      if (curr_inst->failed){
	fail_set->insert(curr_leg->head->label);
      }
     curr_leg = curr_leg->next;
    }
    curr_inst = curr_inst->next;
  }
};

env::env(){
  in_language = NULL;
  out_language = NULL;
  instance = "";
  globals = new str_list();
  portmaps = new pr_list();
  pragmas = new pr_list();
  mark = true;
  fail_trans = false;
}

env::env(env* old){
  in_language = old->in_language;
  out_language = NULL;
  instance = "";
  globals = new str_list(old->globals);
  portmaps = new pr_list(old->portmaps);
  pragmas = new pr_list(old->pragmas);
  mark = true;
  fail_trans = false;
}

env::~env(){
  delete pragmas;
  delete globals;
  delete portmaps;
  delete out_language;
  if (progfile.is_open())
    progfile.close();
}


env_stack::env_stack(){
  head = tail = NULL;
}

env_stack::~env_stack(){
  env* temp = head;
  while (temp){
    delete temp;
    temp = pop();
  }
}

bool env_stack::empty(){
  return (head == NULL);
}

void env_stack::push(env* add){
  if (head != NULL){
    add->next = head;
    head = add;
  }
  else{
    head = tail = add;
  }
}

env *env_stack::pop(){
  env *temp = head;
  if (head != NULL)
    head = head->next;
  return temp;
}
