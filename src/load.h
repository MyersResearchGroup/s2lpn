#ifndef _s2lpn_h
#define _s2lpn_h
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

//random string manipulation fxns
// strips assembly style comments (;) and returns the first non-comment line
string ass_pop_line(fstream *in_file);
// strips c++ style comments (//) and returns the first non-comment line
string cpp_pop_line(fstream *in_file);
//changes strings to all upper case
string upconvert(string in_string);
// copy first whitespace separated word from string
string getword(string in_string);
// delete first whitespace separated word from string
string stripword(string in_string);
// parse string based on delimiting elements
int next_element(string in_string, int *pos, int *count, 
		 string delimiters, int nlimits);
// find str1 in str2 and replace with str3
string findNreplace(string str1, string str2, string str3);
// check if string is a number.
//returns true if there is a leading digit!! (i.e. "6fred")
int isntNumber(const char* str);

// individual string package
class str_pkg{
 public:
  // string
  string curr;
  //pointer to next string
  str_pkg *next,*prev;
  //will only be created if there is a string to put in it...
  str_pkg(string term);
  void print();
};

// paired  string package
class str_pr{
 public:
  // string
  string left, right;
  //pointer to next string pair
  str_pr *next,*prev;
  //will only be created if there is a string pair to put in it...
  str_pr(string term1, string term2);
  //print then move down the string
  void print();
};

// linked list of string packages
class str_list{
 public:
  // linked list pointers
  str_pkg *head,*tail;
  //add a string
  void insert(string command);
  void print();
  void substitute(str_pr *arg);
  void replace(str_pr *arg);
  int is_in(string term);
  str_list();
  str_list(str_list *old_set);  
  ~str_list();
};

// linked list of string pairs
class pr_list{
 public:
  // linked list pointers
  str_pr *head,*tail;
  //add a string
  void insert(string term1, string term2);
  void tail_insert(string term1, string term2);
  void substitute(str_pr *arg);
  str_pr *lookup(string word);
  void print();
  pr_list();
  pr_list(pr_list *old_list);
  ~pr_list();
};

//class for transitions within an instruction
class transition{
 public:
  // transition label
  string label;
  // next place label
  string place;
  //enabling condition
  string enabling;
  //timing bounds
  string lower, upper;
  //priority expression
  string priority;
  //assignment linked list 
  str_list *assigns;
  //boolean assignment linked list 
  str_list *bool_assigns;
  //rate assignment linked list 
  str_list *rate_assigns;
  //add an assignment
  void insert(string command);
  void bool_insert(string command);
  void rate_insert(string command);
  void replace(str_pr *arr);
  void merge(transition *mergee);
  //pointer to next transition
  transition *next;
  //create new transition
  transition(string expr, string low, string up);
  transition(transition *old_trans);
  void print();
  void predicate(string pred);
  void prioritize(string exp);
  void add_label(string str);
  void add_place(string str);
  void delay(string l_b,string u_b);
  ~transition();
};

//branch path for transition
class leg{
 public:
  //path label
  string label,target;
  leg* next;
  transition *head,*tail;
  void print();
  void predicate(string pred);
  void replace(str_pr *arg);
  void insert(transition *term);
  leg(string name, string dest);
  leg(leg *old_leg);
  ~leg();
};

class candidate{
 public:
  string choice;
  candidate *next;
  int match(string check);
  void print();
  candidate(string new_choice);
  ~candidate();
};

class constraint{
 public:
  string variable, str;
  int bound,lower,upper;
  candidate *head, *tail;
  constraint *next;
  void add_candidate(string new_cand);
  void print();
  constraint(string newvar, int lbound, int ubound);
  constraint(string newvar, string choices, int base);
  ~constraint();
};

class instruction{
 public:
  string label,mnemonic,pattern;
  pr_list *sigs, *vars, *conts;
  leg *first, *last;
  int marked;
  int failed;
  constraint *head, *tail;
  instruction *next,*prev;
  void print();
  void predicate(string pred);
  int set_val(string term);
  int set_sig(string term);
  int set_rate(string term);
  void add_leg(leg* new_leg);
  void add_vars(string var_list); 
  void add_constraint(string new_constraint);
  void replace(pr_list *args);
  int merge(string test);
  instruction(string instr, string args);
  instruction(instruction *old_inst);
  ~instruction();
};

class language{
 public:
  string filename,delimiters,merger;
  int nlimits;
  instruction *head,*tail;
  language *next;
  void insert(instruction *new_inst);
  void tail_insert(instruction *new_inst);
  instruction *find_inst(string command, string args);
  void print();
  void merge();
  void predicate(string pred);
  int set_val(string term);
  int set_sig(string term);
  int set_rate(string term);
  language();
  ~language();
};

class library{
 public:
  language *head,*tail;
  language *lookup(string fname);
  void insert(language *new_lang);
  library();
  ~library();
};


class lhpn{
public:
  static int inst_num, pl_num, tr_num;
  str_list *places, *marking, *fail_set;
  pr_list *flow, *vars, *sigs, *conts;
  transition *head, *tail;
  void insert(transition *new_node);
  void print(string f_name);
  void update(language *prog);
  lhpn(){
    head = tail=NULL;
    places = new str_list();
    marking = new str_list();
    fail_set = new str_list();
    vars = new pr_list();
    sigs = new pr_list();
    conts = new pr_list();
    flow = new pr_list();
  };
};

language *load_language(string fname);

#endif  /* s2lpn.h */
