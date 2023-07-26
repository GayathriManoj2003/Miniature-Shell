#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
using namespace std;

class Cat{
    string myText;
    string inp;
    public:
    void cat_command(string x)
    {
        inp=x;
        ifstream catFile (inp);
        if(!catFile) { 
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        while (getline( catFile, myText))
        {
             cout << myText;
        }
        catFile.close();
    };
};



class Tokens {
    vector <string> tokens;
    public:
        Tokens(string);
        string operator[](int index);
        void hi()
        {
            cout << "hi " << endl;
        }
};

class Fork {
    public:
        int execute(Tokens& obj) {
            pid_t p;
            int res;
            p = fork();
            if(p > 0)
            {
                res = wait(NULL);
            }
            else if(!p)
            {
                cout << "\nInside Child process.";
                obj.hi();
                // char * c = (obj[0]).c_str();
                // char * c = (obj[0]).data();
                // char * c = &(obj[0]);
                // str.data();
                char *args[2]={ "./script.sh" ,NULL};
                execvp(args[0],args);
                exit(0);
            }
            return res;
        }
};


Tokens::Tokens(string cmd) {
    stringstream stream(cmd);
    string intermediate;

    while( getline(stream, intermediate, ' ') )
    {
        tokens.push_back(intermediate);
    }
}
string Tokens::operator[]( int index) {
    return tokens[index];
} 
int main()
{
    string prompt(" $ ");
    string cwd;
    while(true) {
        char current_dir[4096];
        string str;
        getcwd(current_dir, sizeof(current_dir));
        cout << current_dir;
        cout << prompt;
        getline(cin, str);

        Tokens tokens(str);
        if (tokens[0]=="cat")
        {
            Cat().cat_command(tokens[1]);
        }
        else if (tokens[0].rfind( "./", 0) == 0) //execute program
        {
            Fork().execute(tokens);
        }
        else if (tokens[0]=="exit") //exit command
        {
            exit(0);
        }

    }

	return 0;
}
