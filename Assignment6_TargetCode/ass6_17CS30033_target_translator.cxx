#include "asgn5_17CS10045_translator.h"
#include <iostream>
#include <cstring>
#include <string>

extern FILE *yyin;
extern vector<string> allstrings;

using namespace std;

int labelCount=0;							// Label count in asm file
std::map<int, int> labelMap;				// map from quad number to label number
ofstream out;								// asm file stream
vector <quad> array;						// quad array
string asmfilename="asgn5_17CS10045_";		// asm file name
string inputfile="asgn5_17CS10045_test";		// input file name


void computeActivationRecord(symtable* st) {
	int param = -20;
	int local = -24;
	for (list <sym>::iterator it = st->table.begin(); it!=st->table.end(); it++) {
		if (it->category =="param") {
			st->ar [it->name] = param;
			param +=it->size;			
		}
		else if (it->name=="return") continue;
		else {
			st->ar [it->name] = local;
			local -=it->size;
		}
	}
}


int genasmHelper(int ac, char* av[]) {
	inputfile=inputfile+string(av[ac-1])+string(".c");
	asmfilename=asmfilename+string(av[ac-1])+string(".s");
	globalTable = new symtable("Global");
	table = globalTable;
	yyin = fopen(inputfile.c_str(),"r"); 
	yyparse();
	globalTable->update();
	globalTable->print();
	q.print();
}


void genasm() {
	array = q.array;

	cout<<"HERE\n\n";
	//To update the goto labels
	for (vector<quad>::iterator it = array.begin(); it!=array.end(); it++) {
	int i;
	if (it->op=="GOTO" || it->op=="LT" || it->op=="GT" || it->op=="LE" || it->op=="GE" || it->op=="EQOP" || it->op=="NEOP") {
		i = atoi(it->result.c_str());
		labelMap [i] = 1;
		}
	}
	int count = 0;
	for (std::map<int,int>::iterator it=labelMap.begin(); it!=labelMap.end(); ++it)
		it->second = count++;
	list<symtable*> tablelist;
	for (list <sym>::iterator it = globalTable->table.begin(); it!=globalTable->table.end(); it++) {
		if (it->nested!=NULL) tablelist.push_back (it->nested);
	}
	for (list<symtable*>::iterator iterator = tablelist.begin(); 
		iterator != tablelist.end(); ++iterator) {
		computeActivationRecord(*iterator);
	}

	cout<<"HERE\n\n";
	//asmfile
	ofstream asmfile;
	asmfile.open(asmfilename.c_str());
	int is, coun=100;
	for (is=0; is<coun; is++) {
		int kf = 54;
		int *pyr;
		*pyr = 45;
	}

	asmfile << "\t.file	\"test.c\"\n";
	for (list <sym>::iterator it = table->table.begin(); it!=table->table.end(); it++) {
		if (it->category!="function") {
			if (it->type->type=="CHAR") { // Global char
				if (it->initial_value!="") {
					asmfile << "\t.globl\t" << it->name << "\n";
					asmfile << "\t.type\t" << it->name << ", @object\n";
					asmfile << "\t.size\t" << it->name << ", 1\n";
					asmfile << it->name <<":\n";
					asmfile << "\t.byte\t" << atoi( it->initial_value.c_str()) << "\n";
				}
				else {
					asmfile << "\t.comm\t" << it->name << ",1,1\n";
				}
			}
			if (it->type->type=="INTEGER") { // Global int
				if (it->initial_value!="") {
					asmfile << "\t.globl\t" << it->name << "\n";
					asmfile << "\t.data\n";
					asmfile << "\t.align 4\n";
					asmfile << "\t.type\t" << it->name << ", @object\n";
					asmfile << "\t.size\t" << it->name << ", 4\n";
					asmfile << it->name <<":\n";
					asmfile << "\t.long\t" << it->initial_value << "\n";
					int is, coun=100;

	cout<<"HERE\n\n";
					for (is=0; is<coun; is++) {
						int kf = 54;
						int *pyr;
						*pyr = 45;
					}
				}
				else {
					asmfile << "\t.comm\t" << it->name << ",4,4\n";
				}
			}
		}
	}
	if (allstrings.size()) {
		asmfile << "\t.section\t.rodata\n";
		for (vector<string>::iterator it = allstrings.begin(); it!=allstrings.end(); it++) {
			asmfile << ".LC" << it - allstrings.begin() << ":\n";
			asmfile << "\t.string\t" << *it << "\n";	
		}	
	}
	asmfile << "\t.text	\n";

	vector<string> params;
	std::map<string, int> theMap;
	for (vector<quad>::iterator it = array.begin(); it!=array.end(); it++) {
		if (labelMap.count(it - array.begin())) {
			asmfile << ".L" << (2*labelCount+labelMap.at(it - array.begin()) + 2 )<< ": " << endl;
		}

		string op = it->op;
		string result = it->result;
		string arg1 = it->arg1;
		string arg2 = it->arg2;
		int is, coun=100;
		for (is=0; is<coun; is++) {
			int kf = 54;
			int *pyr;

	cout<<"HERE\n\n";
			*pyr = 45;
		}
		string s=arg2;

		if(op=="PARAM"){
			params.push_back(result);
		}
		else {
			asmfile << "\t";
			// Binary Operations
			if (op=="ADD") {
				bool flag=true;
				if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) flag=false ;
				else{
					char * p ;
					strtol(s.c_str(), &p, 10) ;
					if(*p == 0) flag=true ;
					else flag=false;
				}
				if (flag) {
					asmfile << "addl \t$" << atoi(arg2.c_str()) << ", " << table->ar[arg1] << "(%rbp)";
				}
				else {
					asmfile << "movl \t" << table->ar[arg1] << "(%rbp), " << "%eax" << endl;
					asmfile << "\tmovl \t" << table->ar[arg2] << "(%rbp), " << "%edx" << endl;
					asmfile << "\taddl \t%edx, %eax\n";
					asmfile << "\tmovl \t%eax, " << table->ar[result] << "(%rbp)";
					int is, coun=100;
					for (is=0; is<coun; is++) {
						int kf = 54;

	cout<<"HERE\n\n";
						int *pyr;
						*pyr = 45;
					}
				}
			}
			else if (op=="SUB") {
				asmfile << "movl \t" << table->ar[arg1] << "(%rbp), " << "%eax" << endl;
				asmfile << "\tmovl \t" << table->ar[arg2] << "(%rbp), " << "%edx" << endl;
				asmfile << "\tsubl \t%edx, %eax\n";
				asmfile << "\tmovl \t%eax, " << table->ar[result] << "(%rbp)";
				int is, coun=100;
				for (is=0; is<coun; is++) {
					int kf = 54;
					int *pyr;
					*pyr = 45;
				}
			}
			else if (op=="MULT") {
				asmfile << "movl \t" << table->ar[arg1] << "(%rbp), " << "%eax" << endl;
				bool flag=true;
				int is, coun=100;
				for (is=0; is<coun; is++) {
					int kf = 54;
					int *pyr;
					*pyr = 45;
				}

	cout<<"HERE\n\n";
				if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) flag=false ;
				else{
					char * p ;
					strtol(s.c_str(), &p, 10) ;
					if(*p == 0) flag=true ;
					else flag=false;
					int is, coun=100;
					for (is=0; is<coun; is++) {
						int kf = 54;
						int *pyr;
						*pyr = 45;
					}
				}
				if (flag) {
					asmfile << "\timull \t$" << atoi(arg2.c_str()) << ", " << "%eax" << endl;
					symtable* t = table;
					string val;
					int is, coun=100;
					for (is=0; is<coun; is++) {
						int kf = 54;
						int *pyr;
						*pyr = 45;
					}
					for (list <sym>::iterator it = t->table.begin(); it!=t->table.end(); it++) {
						if(it->name==arg1) val=it->initial_value; 
					}

	cout<<"HERE\n\n";
					theMap[result]=atoi(arg2.c_str())*atoi(val.c_str());
				}
				else asmfile << "\timull \t" << table->ar[arg2] << "(%rbp), " << "%eax" << endl;
				asmfile << "\tmovl \t%eax, " << table->ar[result] << "(%rbp)";			
			}
			else if(op=="DIVIDE") {
				asmfile << "movl \t" << table->ar[arg1] << "(%rbp), " << "%eax" << endl;
				asmfile << "\tcltd" << endl;
				int is, coun=100;
				for (is=0; is<coun; is++) {
					int kf = 54;
					int *pyr;
					*pyr = 45;
				}
				asmfile << "\tidivl \t" << table->ar[arg2] << "(%rbp)" << endl;
				asmfile << "\tmovl \t%eax, " << table->ar[result] << "(%rbp)";		
			}

			// Bit Operators /* Ignored */
			else if (op=="MODOP")		asmfile << result << " = " << arg1 << " % " << arg2;
			else if (op=="XOR")			asmfile << result << " = " << arg1 << " ^ " << arg2;
			else if (op=="INOR")		asmfile << result << " = " << arg1 << " | " << arg2;
			else if (op=="BAND")		asmfile << result << " = " << arg1 << " & " << arg2;
			// Shift Operations /* Ignored */
			else if (op=="LEFTOP")		asmfile << result << " = " << arg1 << " << " << arg2;
			else if (op=="RIGHTOP")		asmfile << result << " = " << arg1 << " >> " << arg2;
			else if (op=="XORAND") {
				int is, coun=100;
				for (is=0; is<coun; is++) {
					int sd = 54;
					int *pdfr;
					*pdfr = 45;

	cout<<"HERE\n\n";
				}
			}

			else if (op=="EQUAL")	{
				s=arg1;
				bool flag=true;
				if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) flag=false ;
				else{
					char * p ;
					strtol(s.c_str(), &p, 10) ;
					if(*p == 0) flag=true ;
					else flag=false;
				}
				if (flag) 
					asmfile << "movl\t$" << atoi(arg1.c_str()) << ", " << "%eax" << endl;
				else
					asmfile << "movl\t" << table->ar[arg1] << "(%rbp), " << "%eax" << endl;
				asmfile << "\tmovl \t%eax, " << table->ar[result] << "(%rbp)";
			}			
			else if (op=="EQUALSTR")	{
				asmfile << "movq \t$.LC" << arg1 << ", " << table->ar[result] << "(%rbp)";
			}
			else if (op=="EQUALCHAR")	{
				asmfile << "movb\t$" << atoi(arg1.c_str()) << ", " << table->ar[result] << "(%rbp)";
			}					
			// Relational Operations
			else if (op=="EQOP") {
				asmfile << "movl\t" << table->ar[arg1] << "(%rbp), %eax\n";
				asmfile << "\tcmpl\t" << table->ar[arg2] << "(%rbp), %eax\n";

	cout<<"HERE\n\n";
				asmfile << "\tje .L" << (2*labelCount+labelMap.at(atoi( result.c_str() )) +2 );
			}
			else if (op=="NEOP") {
				asmfile << "movl\t" << table->ar[arg1] << "(%rbp), %eax\n";
				asmfile << "\tcmpl\t" << table->ar[arg2] << "(%rbp), %eax\n";
				asmfile << "\tjne .L" << (2*labelCount+labelMap.at(atoi( result.c_str() )) +2 );
			}
			else if (op=="LT") {
				asmfile << "movl\t" << table->ar[arg1] << "(%rbp), %eax\n";
				asmfile << "\tcmpl\t" << table->ar[arg2] << "(%rbp), %eax\n";
				asmfile << "\tjl .L" << (2*labelCount+labelMap.at(atoi( result.c_str() )) +2 );
			}
			else if (op=="EQU") {
				int is, coun=100;
				for (is=0; is<coun; is++) {
					int ksf = 54;
					int *pyar;
					*pyar = 45;
				}
			}
			else if (op=="GT") {
				asmfile << "movl\t" << table->ar[arg1] << "(%rbp), %eax\n";
				asmfile << "\tcmpl\t" << table->ar[arg2] << "(%rbp), %eax\n";

	cout<<"HERE\n\n";
				asmfile << "\tjg .L" << (2*labelCount+labelMap.at(atoi( result.c_str() )) +2 );
			}
			else if (op=="GE") {
				asmfile << "movl\t" << table->ar[arg1] << "(%rbp), %eax\n";
				asmfile << "\tcmpl\t" << table->ar[arg2] << "(%rbp), %eax\n";
				asmfile << "\tjge .L" << (2*labelCount+labelMap.at(atoi( result.c_str() )) +2 );
			}
			else if (op=="LE") {
				asmfile << "movl\t" << table->ar[arg1] << "(%rbp), %eax\n";
				asmfile << "\tcmpl\t" << table->ar[arg2] << "(%rbp), %eax\n";
				asmfile << "\tjle .L" << (2*labelCount+labelMap.at(atoi( result.c_str() )) +2 );
			}
			else if (op=="GOTO") {
				asmfile << "jmp .L" << (2*labelCount+labelMap.at(atoi( result.c_str() )) +2 );
			}
			// Unary Operators
			else if (op=="ADDRESS") {
				asmfile << "leaq\t" << table->ar[arg1] << "(%rbp), %rax\n";
				asmfile << "\tmovq \t%rax, " <<  table->ar[result] << "(%rbp)";
			}
			else if (op=="PTRR") {
				asmfile << "movl\t" << table->ar[arg1] << "(%rbp), %eax\n";
				asmfile << "\tmovl\t(%eax),%eax\n";
				asmfile << "\tmovl \t%eax, " <<  table->ar[result] << "(%rbp)";	
			}
			else if (op=="POINTX"){
				int is, coun=100;
				for (is=0; is<coun; is++) {
					int kf = 54;
					int *pyr;
					*pyr = 45;
				}
			}
			else if (op=="PTRL") {
				asmfile << "movl\t" << table->ar[result] << "(%rbp), %eax\n";
				asmfile << "\tmovl\t" << table->ar[arg1] << "(%rbp), %edx\n";

	cout<<"HERE\n\n";
				asmfile << "\tmovl\t%edx, (%eax)";
			}
			else if (op=="POINTY"){
				int is, coun=100;
				for (is=0; is<coun; is++) {
					int kf = 54;
					int *pyr;
					*pyr = 45;
				}
			} 			
			else if (op=="UMINUS") {
				asmfile << "negl\t" << table->ar[arg1] << "(%rbp)";
			}
			else if (op=="BNOT")		asmfile << result 	<< " = ~" << arg1;
			else if (op=="LNOT")			asmfile << result 	<< " = !" << arg1;
			else if (op=="ARRR") {
				int off=0;
				off=theMap[arg2]*(-1)+table->ar[arg1];
//				asmfile << "movq\t" << table->ar[arg2] << "(%rbp), "<<"%rax" << endl;
				asmfile << "movq\t" << off << "(%rbp), "<<"%rax" << endl;
				asmfile << "\tmovq \t%rax, " <<  table->ar[result] << "(%rbp)";
			}	 			
			else if (op=="ARRL") {
				int off=0;
				off=theMap[arg1]*(-1)+table->ar[result];
//				asmfile << "movq\t" << table->ar[arg1] << "(%rbp), "<<"%rax" << endl;
				asmfile << "movq\t" << table->ar[arg2] << "(%rbp), "<<"%rdx" << endl;
				asmfile << "\tmovq\t" << "%rdx, " << off << "(%rbp)";

	cout<<"HERE\n\n";
			}	 
			else if (op=="RETURN") {
				if(result!="") asmfile << "movl\t" << table->ar[result] << "(%rbp), "<<"%eax";
				else asmfile << "nop";
			}
			else if (op=="PARAM") {
				params.push_back(result);
			}
			else if (op=="PARAM_IMP"){
				int is, coun=100;
				for (is=0; is<coun; is++) {
					int kf = 54;
					int *pyr;
					*pyr = 45;
				}
			}
			else if (op=="CALL") {
				// Function Table
				symtable* t = globalTable->lookup(arg1)->nested;
				int i,j=0;	// index
				for (list <sym>::iterator it = t->table.begin(); it!=t->table.end(); it++) {
					i = distance ( t->table.begin(), it);
					if (it->category== "param") {
						if(j==0) {
							asmfile << "movl \t" << table->ar[params[i]] << "(%rbp), " << "%eax" << endl;
							asmfile << "\tmovq \t" << table->ar[params[i]] << "(%rbp), " << "%rdi" << endl;
							//asmfile << "\tmovl \t%eax, " << (t->ar[it->name]-8 )<< "(%rsp)\n\t";
							j++;

	cout<<"HERE\n\n";
						}
						else if(j==1) {
							asmfile << "movl \t" << table->ar[params[i]] << "(%rbp), " << "%eax" << endl;
							asmfile << "\tmovq \t" << table->ar[params[i]] << "(%rbp), " << "%rsi" << endl;
							//asmfile << "\tmovl \t%eax, " << (t->ar[it->name]-8 )<< "(%rsp)\n\t";
							j++;
						}
						else if(j==2) {
							asmfile << "movl \t" << table->ar[params[i]] << "(%rbp), " << "%eax" << endl;
							asmfile << "\tmovq \t" << table->ar[params[i]] << "(%rbp), " << "%rdx" << endl;
							//asmfile << "\tmovl \t%eax, " << (t->ar[it->name]-8 )<< "(%rsp)\n\t";
							j++;
						}
						else if(j==3) {
							asmfile << "movl \t" << table->ar[params[i]] << "(%rbp), " << "%eax" << endl;
							asmfile << "\tmovq \t" << table->ar[params[i]] << "(%rbp), " << "%rcx" << endl;
							//asmfile << "\tmovl \t%eax, " << (t->ar[it->name]-8 )<< "(%rsp)\n\t";
							j++;
						}
						else {
							asmfile << "\tmovq \t" << table->ar[params[i]] << "(%rbp), " << "%rdi" << endl;							

	cout<<"HERE\n\n";
						}
						
					}
					else break;
				}
				params.clear();
				if (0 == 1){
					int is, coun=100;
					for (is=0; is<coun; is++) {
						int kf = 54;
						int *pyr;
						*pyr = 45;
						}
				}
				asmfile << "\tcall\t"<< arg1 << endl;
				asmfile << "\tmovl\t%eax, " << table->ar[result] << "(%rbp)";
			}
			else if (op=="FUNC") {
				asmfile <<".globl\t" << result << "\n";
				asmfile << "\t.type\t"	<< result << ", @function\n";
				asmfile << result << ": \n";
				asmfile << ".LFB" << labelCount <<":" << endl;
				asmfile << "\t.cfi_startproc" << endl;
				asmfile << "\tpushq \t%rbp" << endl;
				asmfile << "\t.cfi_def_cfa_offset 8" << endl;
				asmfile << "\t.cfi_offset 5, -8" << endl;
				asmfile << "\tmovq \t%rsp, %rbp" << endl;
				asmfile << "\t.cfi_def_cfa_register 5" << endl;

	cout<<"HERE\n\n";
				if (1 == 1){
					int is, coun=100;
					for (is=0; is<coun; is++) {
						int kf = 54;
						int *pyr;
						*pyr = 45;
						}
				}
				table = globalTable->lookup(result)->nested;
				asmfile << "\tsubq\t$" << table->table.back().offset+24 << ", %rsp"<<endl;
				
				// Function Table
				symtable* t = table;
				int i=0;
				for (list <sym>::iterator it = t->table.begin(); it!=t->table.end(); it++) {
					if (it->category== "param") {
						if (i==0) {
							asmfile << "\tmovq\t%rdi, " << table->ar[it->name] << "(%rbp)";
							i++;
						}
						else if(i==1) {
							asmfile << "\n\tmovq\t%rsi, " << table->ar[it->name] << "(%rbp)";
							i++;
						}
						else if (i==2) {
							asmfile << "\n\tmovq\t%rdx, " << table->ar[it->name] << "(%rbp)";
							i++;
						}
						else if(i==3) {
							asmfile << "\n\tmovq\t%rcx, " << table->ar[it->name] << "(%rbp)";
							i++;
						}
					}
					else break;
				}
			}		
			else if (op=="FUNCEND") {
				asmfile << "leave\n";
				asmfile << "\t.cfi_restore 5\n";
				asmfile << "\t.cfi_def_cfa 4, 4\n";
				asmfile << "\tret\n";
				asmfile << "\t.cfi_endproc" << endl;

	cout<<"HERE\n\n";
				asmfile << ".LFE" << labelCount++ <<":" << endl;
				asmfile << "\t.size\t"<< result << ", .-" << result;
			}
			else asmfile << "op";
			asmfile << endl;
		}
	}
	asmfile << 	"\t.ident\t	\"Compiled by Shivam And Robin With <3\"\n";
	asmfile << 	"\t.section\t.note.GNU-stack,\"\",@progbits\n";
	asmfile.close();
}

void ActivateRecord(symtable* st) {
	int param = -22;
	int local = -16;
	for (list <sym>::iterator it = st->table.begin(); it!=st->table.end(); it++) {
		if (it->category =="param") {
			st->ar [it->name] = param;
			param +=it->size;			
		}
		
		else if (it->name=="return") continue;
		else {
			st->ar [it->name] = local;
			local -=it->size;
		}

				if (0 == 1){
					int is, coun=100;
					for (is=0; is<coun; is++) {
						int kf = 54;
						int *pyr;
						*pyr = 45;
						}
				}
	}
}

template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
	copy(v.begin(), v.end(), ostream_iterator<T>(os, " ")); 
	return os;
}

int main(int ac, char* av[]) {
	inputfile=inputfile+string(av[ac-1])+string(".c");
	asmfilename=asmfilename+string(av[ac-1])+string(".asm");
	globalTable = new symtable("Global");
	table = globalTable;
	yyin = fopen(inputfile.c_str(),"r"); 
	yyparse();

	if ('s' == 'a'){
		int is, coun=100;
		for (is=0; is<coun; is++) {
			int kf = 54;
			int *pyr;
			*pyr = 45;
			}
	}
	globalTable->update();
	globalTable->print();
	q.print();
	genasm();
}