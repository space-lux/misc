#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>

using namespace std;

void includeFiles(string& text) {
    size_t found=text.find("%inc% ");
    size_t efound;
    ifstream ifile;
    string line;
    string filec;
    while(found!=string::npos) {
        filec="";
        efound=text.find("\n",found);

        ifile.open(text.substr(found+6,efound-found-6).c_str());
        while(getline(ifile,line)) {
            filec+=line+"\n";
        }
        ifile.close();
        includeFiles(filec);

        text.replace(found,efound-found+1,filec);

        found=text.find("%inc% ");
    }
}

void removeComments(string& text) {
    size_t found=text.find("%rem%");
    size_t efound;
    while(found!=string::npos) {
        efound=text.find("\n",found);
        text.erase(found,efound-found);
        found=text.find("%rem%");
    }
}

void replaceConsts(string& text) {
    size_t found=text.find("%var% ");
    size_t efound;
    size_t elfound;
    string label;
    string content;
    while(found!=string::npos) {
        efound=text.find("\n",found);
        elfound=text.find(" ",found+6);

        label=text.substr(found+6,elfound-found-6);
        content=text.substr(elfound+1,efound-elfound-1);

        text.replace(found,efound-found+1,"");

        found=text.find(label);
        while(found!=string::npos) {
            text.replace(found,label.length(),content);
            found=text.find(label);
        }

        found=text.find("%var% ");
    }
}

void formatMacroContent(string& content,string& fcontent,vector<string>& args,vector<int>& iargs) {
    char lic[]="0123456789";
    fcontent=content.substr();
    string acontent;
    string cfc;
    size_t found=fcontent.find("%d");
    size_t elfound;
    size_t efound;
    size_t argc=args.size();
    size_t argn;
    while(found!=string::npos) {
        elfound=fcontent.find_first_not_of(lic,found+2);

        if(fcontent[elfound]=='%') {
            cfc=fcontent.substr(found+2,elfound-found-2);
            sscanf(cfc.c_str(),"%u",&argn);
            if(argn<argc) {
                fcontent.replace(found,1+elfound-found,args[argn]);
            } else {
                fcontent.replace(found,1+elfound-found,"");
            }
        }

        found=fcontent.find("%d",found+1);
    }
    found=fcontent.find("%r");
    while(found!=string::npos) {
        elfound=fcontent.find_first_not_of(lic,found+2);
        if(fcontent[elfound]=='(') {
            cfc=fcontent.substr(found+2,elfound-found-2);
            sscanf(cfc.c_str(),"%u",&argn);
            efound=fcontent.find(")%",elfound+1);
            acontent=fcontent.substr(elfound+1,efound-elfound-1);
            fcontent.erase(found,efound-found+2);
            if(argn<argc) {
                for(size_t i=0;i<iargs[argn];i++) {
                    fcontent.insert(found,acontent);
                }
            }
        }

        found=fcontent.find("%r",found+1);
    }
}
void parseArg(string& arg,int& iarg) {
    if(!sscanf(arg.c_str(),"%u",&iarg)) {
        iarg=0;
    }
    if(!arg.substr(0,2).compare("{(")) {
        arg.replace(arg.length()-3,2,"");
        arg.replace(0,2,"");
    }
}
void endOfArg(string& text,const size_t cursor1,size_t& cursor2) {
    if(text.substr(cursor1,2)=="{(") {
        cursor2=text.find(")}",cursor1)+2;
    } else {
        cursor2=text.find_first_of(",)",cursor1);
    }
}
void parseMacro(string& text,string& label,string& content) {
    size_t found=text.find(label+"(");
    size_t cursor1;
    size_t cursor2;
    string fcontent;
    string arg;
    int iarg;
    vector<string> args;
    vector<int> iargs;
    while(found!=string::npos) {
        cursor1=found+label.length()+1;
        cursor2=cursor1;

        while(text[cursor2]!=')') {
            endOfArg(text,cursor1,cursor2);
            arg=text.substr(cursor1,cursor2-cursor1);
            parseArg(arg,iarg);
            args.push_back(arg);
            iargs.push_back(iarg);
            cursor1=cursor2+1;
        }

        formatMacroContent(content,fcontent,args,iargs);
        text.replace(found,cursor2+1-found,fcontent);

        found=text.find(label+"(",found+1);
        args.clear();
        iargs.clear();
    }
}
void parseMacros(string& text) {
    size_t found=text.find("%def% ");
    size_t efound;
    size_t elfound;
    string label;
    string content;

    while(found!=string::npos) {
        efound=text.find("\n)}",found);
        elfound=text.find("{(\n",found);

        label=text.substr(found+6,elfound-found-6);
        content=text.substr(elfound+3,efound-elfound-3);

        text.replace(found,efound-found+4,"");

        parseMacro(text,label,content);

        found=text.find("%def% ");
    }
}

int main(int argc, char **argv) {
    string text;

    bool ifileb=false;
    string ifilename;
    ifstream ifile;
    bool ofileb=false;
    string ofilename;
    ofstream ofile;
    string line;

    bool showHelp=false;

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
                    }
				}
			}
		} else if (!strcmp(arg, "--help"))
			showHelp = true;
    }

	if(showHelp) {
        cout<<"MaKro -- macro parser for almost everything"<<endl;
        cout<<"Usage : makro [-i INPUT_FILENAME] [-o OUTPUT_FILENAME]"<<endl;
        cout<<endl;
        cout<<"Defining const :"<<endl;
        cout<<"%var% NAME CONTENT"<<endl;
        cout<<endl;
        /*cout<<"Using const :"<<endl;
        cout<<"NAME"<<endl;
        cout<<endl;*/
        cout<<"Defining macro :"<<endl;
        cout<<"%def% NAME{("<<endl;
        cout<<"CONTENT"<<endl;
        cout<<")}"<<endl;
        cout<<endl;
        cout<<"Using macro parameters - N is param offset starting from 0 :"<<endl;
        cout<<"Display : %dN%"<<endl;
        cout<<"Repeat : %rN%"<<endl;
        cout<<endl;
        cout<<"Calling macro :"<<endl;
        cout<<"NAME(PARAM1[,PARAM2[,PARAM3[,...]]])"<<endl;
        cout<<endl;
        cout<<"NAME format :"<<endl;
        cout<<"letters (a-zA-Z) and underscore (_)"<<endl;
        cout<<endl;
        cout<<"PARAMS format :"<<endl;
        cout<<"STRING      - w/o comma (,) or closing parenthesis ()) - display only"<<endl;
        cout<<"{(STRING)}  -                                          - display only"<<endl;
        cout<<"INTEGER     -                                          - display or repeat"<<endl;
        cout<<"Spaces between parameters are *parts* of those params, and automatically turn them into non-protected strings"<<endl;
        cout<<endl;
        cout<<"Including files :"<<endl;
        cout<<"%inc% FILENAME"<<endl;
        cout<<endl;
        cout<<"Using comments :"<<endl;
        cout<<"%rem% COMMENT"<<endl;
        cout<<endl;
        cout<<"Parsing order :"<<endl;
        cout<<"including files->removing comments->replacing consts->parsing macros"<<endl;
        cout<<""<<endl;
	}

	if(ifileb) {
        ifile.open(ifilename.c_str());
        while(getline(ifile,line)) {
            text+=line+"\n";
        }
        ifile.close();
	} else if(!showHelp) {
        while(getline(cin,line)) {
            text+=line+"\n";
        }
	}

    includeFiles(text);
    removeComments(text);
    replaceConsts(text);
    parseMacros(text);

	if(ofileb) {
        ofile.open(ofilename.c_str());
        ofile<<text;
        ofile.close();
	} else {
        cout<<text;
	}

    return 0;
}
