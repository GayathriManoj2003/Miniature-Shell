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
        int isSpace(string);
        // void hi()
        // {
        //     cout << "hi " << endl;
        // }
};

int Tokens::num_args() {
    return tokens.size() - 1;
}
int Tokens::isSpace( string str) {
    int i = 0;
    while( str[i++] != '\0')
        if( !isspace(str[i]) ) return 0;
    return 1;
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
             cout << myText;
        }
        // cout << endl;
        catFile.close();
    };
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
                // obj.hi();
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
    string tok;
    if(isSpace(cmd)) {
        tokens.push_back("\0");
    }
    else {
        stringstream cmd_stream(cmd);
        while( cmd_stream >> tok ) {
            tokens.push_back(tok);
        }
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
                cmd.append("\0");
                flag = executeCommand(cmd);
            }
        }
        int executeCommand(string cmd) {

            Tokens tokens(cmd);
            cout << tokens.num_args();
            if (tokens[0]=="\0") {
                return 1;
            }
            if (tokens[0]=="cat")
            {
                Cat().cat_command(tokens[1]);
            }
            else if (tokens[0].rfind( "./", 0) == 0) // execute program
            {
                Fork().execute(tokens);
            }
            else if (tokens[0] == "cd") // cd command
            {
                ChangeDir().execute(tokens[1]);
            }
            else if (tokens[0] == "clear") // clear command
            {
                Clear().execute(tokens);
            }
            else if (tokens[0]=="exit") // exit command
            {
                return 0;
            }
            else
            {
                cout << "Command " + tokens[0] + " not found." << endl;
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
