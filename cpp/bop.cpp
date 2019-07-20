#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

string repeat(string s,unsigned int n) {
    string r;
    for(unsigned int i=0;i<n;i++) {
        r+=s;
    }
    return r;
}

void replace(string& text,const string s,const string r) {
    size_t found=text.find(s);
    while(found!=string::npos) {
       text.replace(found,s.length(),r);
       found=text.find(s);
    }
}

void strip(string& text,const string s) {
    size_t found=text.find(s);
    while(found!=string::npos) {
       text.erase(found,s.length());
       found=text.find(s);
    }
}

void group(string& text,char open) {
    char close;
    size_t found=text.find(open);
    size_t found1;
    size_t found2;
    string lc="+-z";
    string tmp;
    if(open=='<') {
        close='>';
    } else {
        close='<';
        open='>';
    }
    while(found!=string::npos) {
        found1=text.find_first_not_of(lc,found+1);
        if(text[found1]==close) {
            found2=text.find_first_not_of(lc,found1+1);
            tmp=text.substr(found,found1-found+1);
            text.erase(found,found1-found+1);
            text.replace(found+found2-found1-1,0,tmp);
        }
        found=text.find(open,found+1);
    }
}

int main(int argc, char **argv) {
    string lc="-+><[].,/";
    string text;
    char passes=3;

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
                    case 'l': {
                        ofileb=true;
                        if (nc == argl - 1
                            && argn < argc - 1) {
                            argn++;
                            lc=argv[argn];
                        } else
                            std::cerr << "-l requires a string right after." << std::endl;
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
        cout<<"BFOptimizer -- reduces BF/GF files size"<<endl;
        cout<<"usage : bop [-i INPUT_FILENAME] [-o OUTPUT_FILENAME] [-l LEGAL_CHARACTERS]"<<endl;
        cout<<""<<endl;
    }

    if(ifileb) {
        ifile.open(ifilename.c_str());
        while(getline(ifile,line)) {
            text+=line;
        }
        ifile.close();
    } else if(!showHelp) {
        while(getline(cin,line)) {
            text+=line;
        }
    }

    size_t found=text.find_first_not_of(lc);
    while(found!=string::npos) {
        text.erase(found,1);
        found=text.find_first_not_of(lc);
    }
    text+="\n";

    for(char i=0;i<passes;i++) {
        replace(text,"//","/");
        replace(text,"[+]","[-]");
        replace(text,"[-][-]","[-]");
        replace(text,"[-]","z");
        strip(text,"[]");
        strip(text,"><");
        strip(text,"<>");
        strip(text,"-+");
        strip(text,"+-");
        replace(text,repeat("+",128),repeat("-",128));
        strip(text,repeat("-",256));
        strip(text,"-+");
        strip(text,"+-");
        replace(text,repeat("-",128),repeat("+",128));
        strip(text,repeat("+",256));
        group(text,'<');
        strip(text,"><");
        strip(text,"<>");
        group(text,'>');
    }
    replace(text,"z","[-]");


    if(ofileb) {
        ofile.open(ofilename.c_str());
        ofile<<text;
        ofile.close();
    } else {
        cout<<text;
    }

    return 0;
}
