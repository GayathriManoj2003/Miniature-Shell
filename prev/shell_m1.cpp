#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>

#include <cstring>
#include <ctime>
#include <filesystem>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <errno.h>
#include <termios.h>
#include <sys/select.h>
#include <dirent.h>
#include <cstdio>
using namespace std;
char getch() {
    char ch;
    struct termios old = {0};
    tcgetattr(0, &old);
    struct termios new_termios = old;
    new_termios.c_lflag &= ~ICANON; // Turn off canonical mode
    new_termios.c_lflag &= ~ECHO;   // Turn off echoing of input characters
    tcsetattr(0, TCSANOW, &new_termios);

    read(0, &ch, 1);

    tcsetattr(0, TCSANOW, &old);
    return ch;
}
class Tokens {
    public:
        vector <string> tokens;
        Tokens(string);
        string operator[](int index);
        int num_args();
        int isSpace(string);
};

int Tokens::num_args() {
    return tokens.size() - 1;
}
int Tokens::isSpace( string str) {
    return str.find_first_not_of(" \t\n\v\f\r") ==-1;
}
int isSpace( string str) {
    return str.find_first_not_of(" \t\n\v\f\r")==-1;
}

class Cat{
    string myText;
    string input;
    string output;
    public:
        int cat_command(string x)
        {
            input = x;
            char *str = new char[x.size() + 1];
            ifstream catFile(input);
            if(!catFile) {
                cout << "cat: " + input + ": No such file or directory" << endl;
                return -1;
            }
            stringstream stream;
            stream << catFile.rdbuf();
            output = stream.str();
            // cout << output;
            catFile.close();
            return 0;
        }
        int cat_command(string x,string y)
        {   
            input=y;
            char current_dir[4096];
            getcwd(current_dir, sizeof(current_dir));
            if (x=="-n") {                
                ifstream catfile (input);
                if(!catfile) {
                    cout << "cat: " + input + ": No such file or directory" << endl;
                    return -1;
                }
                int i=1;
                stringstream stream;
                while (getline(catfile, myText)) {
                    stream <<i<< myText<<endl;
                    i++;
                }
                output = stream.str();
                catfile.close();
            }
            return 0;
        }
        string getOutput() {
            return output;
        }
};

class ChangeDir{
    public:
        int execute(string dir_path)
        {
            try {
                std::filesystem::current_path(dir_path);
            }
            catch ( const std::filesystem::filesystem_error& exception) {
                 cout << "error: " << exception.what();
                 cout << endl;
                 return -1;
            }
            return 0;
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

class WordCount{
    string output;
    public:
        int execute( Tokens& obj) {
            return 0;
        }
};

class Touch{
    string output;
    public:
        int execute( Tokens& obj) {
            return 0;
        }
};

class ListDir{
    string output;
    public:
        int execute( Tokens& obj)
        {
            int status;
            stringstream stream;
            struct stat stats;
            struct dirent *dp;
            DIR *dirp;
            int l = 0, a = 0;
            int ind = -1;

            for(int i = 0; i < obj.num_args(); i++) {
                if(obj[i+1] == "-a")
                    a = 1;
                else if(obj[i+1] == "-l")
                    l = 1;
                else if(obj[i+1] == "-al")
                    a = l = 1;
                else if(obj[i+1][0] != '-')
                    ind = i + 1;
            }
            string p_str;
            if(ind != -1)
                p_str = obj.tokens[ind];
            else {
                p_str = filesystem::current_path();
            }
            // cout << p_str;
            char *path = new char(p_str.size() + 1);
            int i = 0;
            for( char c: p_str)
                path[i++] = c;
            path[i] = '\0';
            dirp = opendir(path);

            if (dirp == NULL) {
                stream << "No such File or Directory." << endl;
                return -1;
            }
            if(l)
                stream << left << setw(30) << "Permissions" << "File Name" << endl;
            dp = (struct dirent*) malloc(sizeof(struct dirent));
            do {
                dp = readdir(dirp);
                if (dp != NULL && strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
                    if(!a && dp->d_name[0] == '.')
                        continue;
                    char *file_path = new char[strlen(dp->d_name) + strlen(path) + 2];
                    strcpy(file_path, path);
                    strcat(file_path, "/");
                    strcat(file_path, dp->d_name);
                    char ori[11];
                    if(l) {
                        status = stat(path, &stats);
                        mode_str(stats.st_mode, ori);
                        stream << left << setw(30) << ori << dp->d_name << endl;
                    }
                    else {
                        stream << dp->d_name <<"\t";
                    }
                    delete[] file_path;
                }
            } while (dp != NULL);
            if(!l)
                stream << endl;
            closedir(dirp);
            delete[] path;
            output = stream.str();
            free(dp);
            dp = NULL;
            return 0;
        };
        string& getOutput() {
            return output;
        }
        void mode_str(mode_t m, char *str) {
            str[0] = (m & S_IRUSR)? 'r':'-';
            str[1] = (m & S_IWUSR)? 'w':'-';
            str[3] = (m & S_IRGRP)? 'r':'-';
            str[2] = (m & S_IXUSR)? 'x':'-';
            str[4] = (m & S_IWGRP)? 'w':'-';
            str[5] = (m & S_IXGRP)? 'x':'-';
            str[6] = (m & S_IROTH)? 'r':'-';
            str[7] = (m & S_IWOTH)? 'w':'-';
            str[8] = (m & S_IXOTH)? 'x':'-';
            str[9] = '\0';
        }
};

class PrintDir{
    string output;
    public:
        int execute( Tokens& obj) {
            if( obj.num_args() > 0 )
            {
                cout << "pwd does not accept arguments." << endl;
                return -1;
            }
            output = filesystem::current_path();
            return 0;
        };
        string getOutput() {
            return output;
        }
};

class Fork {
    public:
        int execute(Tokens& obj) {
            vector <string> cmd_tok=obj.tokens;
            
            pid_t p;
            int res;
            p = fork();
            if(p > 0) {
                res = wait(NULL);
            }
            else if(!p)
            {                
                char *args[cmd_tok.size()+1];
                int index=0;int i=0;int k=0;
                for (i=0;i<cmd_tok.size();i++)
                {   
                    char *exec_name= new char[cmd_tok[i].size()+1];
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
    string current_dir;
    string input = "";
    vector<string> history;

    public:
        void printPrompt() {
            current_dir = filesystem::current_path();
            cout << "\e[1m" << current_dir << prompt << "\e[0m";
        }
        void execute() {
            int flag = 1;
            char ch;
            int len, i;
            string cmd;
            len = history.size();
            i = len - 1;
            while(flag) {
                printPrompt();
                ch = getch();
                // putc(ch, stdout); 
                if (ch == 27) { 
                    ch = getch();
                    if (ch == 91) { 
                        ch = getch();
                        if (ch == 65) {
                            string str = history[i];
                            if (i !=0){
                                --i; }            
                            cout << str << endl;
                            cmd = str;
                        }
                        else if (ch == 66) {
                            if (i!= len-1) i++;
                                string str = history[i];
                            cout << str << endl;
                            cmd = str; 
                        }
                    }
                }
                else {
                    getline(cin, cmd);
                    flag = processCmd(cmd);
                    history.push_back(cmd);
                    len = history.size();
                    i = len -1;
                }
            }
        }
        int processCmd(string cmd_chain) {
            string output;
            int res;
            if (cmd_chain.empty() || isSpace(cmd_chain)) {
                return 1;
            }
            if( cmd_chain.find('|') == -1 ) {
                res = executeCommand( cmd_chain, output, 1);
                if(res == -1 || res == 1)
                    return 1;
                if(output != "")
                    cout << output << endl;
            }
            else {
                int ind;
                int next = 0;
                string cmd;
                while ((ind = cmd_chain.find('|', next)) != std::string::npos) {
                    cmd = cmd_chain.substr(next, ind - next);
                    res = executeCommand( cmd, output);
                    if( res == -1 )
                        return -1;
                    input = output;
                    next = ind + 1;
                }
                if( next < (cmd_chain.size() -1) ) {
                    cmd = cmd_chain.substr(next, cmd_chain.size() - 1);
                    res = executeCommand( cmd, output);
                    if( res == -1 || res == 1)
                        return -1;
                }
                if(output!="")
                    cout << output << endl;
            }
            return 1;
        }
        int executeCommand(string cmd, string& out, int first = 0) {
            out = "";
            int flag = 0;
            Tokens tokens(cmd);
            if (tokens[0]=="\0") {
                return 1;
            }
            if(!first)
                if(input != "")
                    tokens.tokens.push_back(input);
            if (tokens[0]=="cat") {
                if(tokens.num_args() < 1) return -1;
                Cat c1;
                int res = -1;
                if(tokens.num_args() == 1) {
                    res = c1.cat_command(tokens[1]);
                    out = c1.getOutput();
                }
                else if(tokens.num_args() == 2) {
                    res = c1.cat_command(tokens[1], tokens[2]);
                    out = c1.getOutput();
                }
                if(out[out.size() - 1] == '\n')
                    out[out.size() - 1] = '\0';
                if( res == -1 )
                    return -1;
            }
            else if (tokens[0].rfind( "./", 0) == 0) // execute program
            {
                Fork().execute(tokens);
            }
            else if (tokens[0] == "cd") // cd command
            {
                if(tokens.num_args() < 1) return -1;
                ChangeDir cd;
                int res = cd.execute(tokens[1]);
                if( res == -1 ) return 1;
                out = "";
            }
            else if (tokens[0] == "clear") // clear command
            {
                Clear clr;
                clr.execute(tokens);
            }
            else if (tokens[0] == "pwd") // pwdcommand
            {
                PrintDir pwd;
                int res = pwd.execute(tokens);
                if( res == -1 ) return 1;
                out = pwd.getOutput() + "\n";
            }
            else if (tokens[0] == "ls") // ls command
            {
                ListDir ls;
                // cout << tokens[tokens.tokens.size() -1 ] <<endl;
                if(tokens.num_args() == 0)
                    tokens.tokens.push_back(current_dir);
                int res = ls.execute(tokens);
                if( res == -1 ) return 1;
                out = ls.getOutput();
            }
            else if (tokens[0]=="exit") // exit command
            {
                exit(0);
            }
            else {
                cout << "Command " + tokens[0] + " not found." << endl;
                return -1;
            }
            return 0;
        }
};
int main()
{
    SimpleShell shell;
    shell.execute();
	return 0;
}
