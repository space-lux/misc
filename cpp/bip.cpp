#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <list>
#include <array>
#include <algorithm>

using namespace std;

enum OpType {
	OpNone,
	OpSet,
	OpAdd,
	OpPointerAdd,
	OpPrint,
	OpRead,
	OpLeftBra,
	OpRightBra,
	SpOpEnd
};

class BF_AST_Node {
public:
	BF_AST_Node() {
		op_type=OpNone;
		data=0;
	}
	BF_AST_Node(OpType op, int dat=1) {
		data=dat;
		op_type=op;
	}
	OpType op_type;
	int data;
};

int main(int argc, char **argv) {
    string text;
	string prgm;

	bool pfileb=false;
	string pfilename;
	ifstream pfile;
	
    bool ifileb=false;
    string ifilename;
    ifstream ifile;
	
    bool ofileb=false;
    string ofilename;
    ofstream ofile;
	
    string line;

    bool showHelp=false;
	
	bool print_size=false;
	bool print_memory=false;

    for (int argn = 1 ; argn < argc ; argn++) {
		char* arg = argv[argn];
		size_t argl = strlen(arg);
		if (arg[0] == '-' && argl > 1 && arg[1] != '-') {
			for (int nc = 1 ; nc < argl ; nc++) {
				switch (arg[nc]) {
                    case 'i': {
                        ifileb=true;
						if (nc == argl - 1
							&& argn < argc - 1) {
							argn++;
							ifilename=argv[argn];
						} else
							std::cerr << "-i requires a string right after." << std::endl;
                    } break;
                    case 'o': {
                        ofileb=true;
						if (nc == argl - 1
							&& argn < argc - 1) {
							argn++;
							ofilename=argv[argn];
						} else
							std::cerr << "-o requires a string right after." << std::endl;
                    } break;
                    case 'h': {
                        showHelp=true;
                    } break;
                    case 's': {
                        print_size=true;
                    } break;
                    case 'm': {
                        print_memory=true;
                    } break;
				}
			}
		} else if (!strcmp(arg, "--help")) {
			showHelp = true;
		} else {
			pfileb=true;
			pfilename=arg;
		}
    }

	if(showHelp) {
        cout<<"Brainfuck interpreter"<<endl;
        cout<<"usage : bip PROGAM_FILENAME [-i INPUT_FILENAME] [-o OUTPUT_FILENAME]"<<endl;
        cout<<""<<endl;
	}
	
	if(pfileb) {
		pfile.open(pfilename.c_str());
		while(getline(pfile,line)) {
			prgm+=line;
		}
		pfile.close();
	}
	if(ifileb) {
        ifile.open(ifilename.c_str());
        while(getline(ifile,line)) {
            text+=line;
        }
        ifile.close();
	} /*else if(!showHelp) {
        while(getline(cin,line)) {
            text+=line;
        }
	}*/
	
	
	// AST/ASL building
	
	list<BF_AST_Node> AST;
	
	OpType OpCode;
	int Dat;

	if(pfileb) {
		for(int i=0;i<prgm.length();i++) {
			char c=prgm[i];
			Dat=1;
			switch(c) {
				case '+':{
					OpCode=OpAdd;
				} break;
				case '-':{
					OpCode=OpAdd;
					Dat=-1;
				} break;
				case '<':{
					OpCode=OpPointerAdd;
					Dat=-1;
				} break;
				case '>':{
					OpCode=OpPointerAdd;
				} break;
				case '.':{
					OpCode=OpPrint;
				} break;
				case ',':{
					OpCode=OpRead;
				} break;
				case '[':{
					OpCode=OpLeftBra;
					if(prgm[i+1]=='-') {
						if(prgm[i+2]==']') {
							OpCode=OpSet;
							Dat=0;
							i+=2;
						}
					}
				} break;
				case ']':{
					OpCode=OpRightBra;
				} break;
				default: {
					OpCode=OpNone;
				}
			}
			if(OpCode!=OpNone) {
				AST.push_back(BF_AST_Node(OpCode,Dat));
			}
		}
	}
	
	list<BF_AST_Node> fetched_AST;
	for(auto &ast_node:AST) {
		if(AST.size()){
			bool add_data=false;
			switch(ast_node.op_type) {
				case OpAdd:
					if(fetched_AST.back().op_type==OpSet) {
						add_data=true;
					}
				case OpPointerAdd:
					if(ast_node.op_type==fetched_AST.back().op_type) {
						add_data=true;
					}
					break;
			}
			if(add_data) {
				fetched_AST.back().data+=ast_node.data;
			} else {
				fetched_AST.push_back(ast_node);
			}
			
		} else {
			fetched_AST.push_back(ast_node);
		}
	}
	fetched_AST.push_back(BF_AST_Node(SpOpEnd));
	
	int prgm_size=fetched_AST.size();
	
	BF_AST_Node array_AST[prgm_size];
	copy(fetched_AST.begin(),fetched_AST.end(),array_AST);

	for(int i=0;i<prgm_size;i++) {
		if(array_AST[i].op_type==OpLeftBra){
			int j=i;
			int level=1;
			while(level) {
				j++;
				if(array_AST[j].op_type==OpLeftBra){
					level++;
				}
				if(array_AST[j].op_type==OpRightBra){
					level--;
				}
			}
			array_AST[i].data=j;
			array_AST[j].data=i;
		}
	}
	
	// VIRTUAL MACHINE
	
	char memory[30000];
	int pc=0;
	int p=0;
	bool run=true;
	while(run) {
		BF_AST_Node op=array_AST[pc];
		switch(op.op_type) {
			case SpOpEnd: {
				run=false;
			} break;
			case OpSet: {
				memory[p]=char(op.data);
			} break;
			case OpAdd: {
				memory[p]+=char(op.data);
			} break;
			case OpPointerAdd: {
				p+=op.data;
			} break;
			case OpLeftBra: {
				if(!memory[p]) {
					pc=op.data;
				}
			} break;
			case OpRightBra: {
				pc=op.data;
				continue;
			} break;
			case OpPrint: {
				cout<<memory[p];
			} break;
			case OpRead: {
				cin>>memory[p];
			} break;
		}
		pc++;
	}
	
	
	// Debug
	
	if(print_size){
		cout<<prgm_size<<endl;
	}
	if(print_memory) {
		for(int i=0;i<20;i++) {
			cout<<memory[i];
		}
		cout<<endl;
	}
	
	/*
	if(ofileb) {
        ofile.open(ofilename.c_str());
        ofile<<text;
        ofile.close();
	} else {
        cout<<text;
	}
	*/
    return 0;
}
