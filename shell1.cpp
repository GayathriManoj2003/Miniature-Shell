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
#include <cstdlib>
#include <errno.h>
using namespace std;

class Tokens {
    public:
        vector <string> tokens;
        Tokens(string);
        string operator[](int index);
        int num_args();
        // void hi()
        // {
        //     cout << "hi " << endl;
        // }
};

int Tokens::num_args() {
    return tokens.size() - 1;
}

class Cat{
    string myText;
    string input;
    public:
    void cat_command(string x)
    {
        input = x;
        char current_dir[4096];
        getcwd(current_dir, sizeof(current_dir));
        char *str = new char[x.size() + 1];

        ifstream catFile(input);
        if(!catFile) {
            cout << "cat: " + input + ": No such file or directory" << endl;
        }
        while(getline(catFile, myText))
        {
             cout << myText <<endl;
        }
        // cout << endl;
        catFile.close();
    };
    void cat_command(string x,string y)
    {   input=y;
        char current_dir[4096];
        getcwd(current_dir, sizeof(current_dir));
        if (x=="-n")
            {
                // cout<<"N"<<endl;
                
                ifstream catfile (input);
                int i=1;
                while (getline(catfile, myText)) 
                {
                    cout <<i<< myText<<endl;
                    i++;
                }

                catfile.close();

                    }
        else
            {   
                // cout<<"IN ELSE"<<endl;
                //Redirection
                // ifstream MyReadFile (x);
                // int i=1;
                // ofstream write_file(y);
                // ifstream io;
                // while (getline(MyReadFile, myText)) 
                // {
                //     write_file<< myText<<endl;
                    
                // }
                
                // write_file.close();
                // MyReadFile.close();
            }
    }

};

class ChangeDir{
    public:
        void execute(string dir_path)
        {
            // chdir(dir_path.c_str());+
            
            try {
                std::filesystem::current_path(dir_path); //setting path
            }
            catch ( const std::filesystem::filesystem_error& ex) {
                 cout << "Error: " << ex.what() << endl;
            }
            // const char* path = dir_path.c_str();
            // const char* curDir = std::getenv("PWD");
            // cout << curDir << endl;
            // if (curDir == nullptr) {
            //     std::cout << "Failed to get the current working directory" << std::endl;
            // }
            // else if (setenv("PWD", path, 1) != 0) {
            //     std::cout << "Failed to change directory" << std::endl;
            // }
        };
};

class Clear{
    public:
        Clear() {
        }
        void execute( Tokens& obj)
        {
            if( obj.num_args() > 0 )
            {
                cout << "clear does not accept any arguments." << endl;
                return;
            }
            // ANSI Escape Sequences used
            cout << "\033[2J\033[1;1H";
        };
};

class Fork {
    public:
        int execute(Tokens& obj) {
            vector <string> cmd_tok=obj.tokens;
            
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
                
                char *args[cmd_tok.size()+1];
                int index=0;int i=0;int k=0;
                for (i=0;i<cmd_tok.size();i++)
                {   char *exec_name= new char[cmd_tok[i].size()+1];
                    for (index=0;index<cmd_tok[0].size();index++)
                    {
                         exec_name[index]=cmd_tok[i][index];
                     }
                     args[i]=exec_name;

                }
                args[i]=NULL;
                
                execvp(args[0],args);
                exit(0);
            }
            return res;
        }
};


Tokens::Tokens(string cmd) {

    string tok;
    stringstream cmd_stream(cmd);

    while( cmd_stream >> tok ) {
        tokens.push_back(tok);
    }
}
string Tokens::operator[]( int index) {
    return tokens[index];
} 

class SimpleShell {
    const string prompt = " $ ";

    public:
        void printPrompt() {
            char current_dir[4096];
            getcwd(current_dir, sizeof(current_dir));
            // const char* current_dir = std::getenv("PWD");

            cout << "\e[1m" << current_dir << prompt << "\e[0m";
        }
        void execute() {
            int flag = 1;
            while(flag) {
                string cmd;
                printPrompt();
                getline(cin, cmd);
                flag = executeCommand(cmd);
            }
        }
        int executeCommand(string cmd) {

            
            Tokens tokens1(cmd);
            if (tokens1.tokens[0]=="cat")
            {
                Cat().cat_command(tokens1.tokens[1]);
            }
            else if (tokens1.tokens[0].rfind( "./", 0) == 0) // execute program
            {
                Fork().execute(tokens1);
            }
            else if (tokens1.tokens[0] == "cd") // cd command
            {
                ChangeDir().execute(tokens1.tokens[1]);
            }
            else if (tokens1.tokens[0] == "clear") // clear command
            {
                Clear().execute(tokens1);
            }
            else if (tokens1.tokens[0]=="exit") // exit command
            {
                return 0;
            }
            else
            {
                cout << "Command " + tokens1.tokens[0] + " not found." << endl;
            }
            return 1;
        }

};
int main()
{
    SimpleShell shell;
    shell.execute();
	return 0;
}