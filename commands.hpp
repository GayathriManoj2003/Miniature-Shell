#include <filesystem>
#include <fstream>
#include <sys/stat.h>

#include <cstring>
#include <sys/wait.h>
#include <dirent.h>

#include "tokens.hpp"

class Cat{
    string myText;
    string input;
    string output;
    public:
        int cat_command(string x);
        int cat_command(string x,string y);
        int execute(Tokens &);
        string getOutput();
};

class MakeDirectory{
    string output;
    public:
        int execute(Tokens& obj);
        int createDir(string);
        string getOutput();
};

class ChangeDir{
    public:
        int execute(string dir_path);
};

class Clear{
    public:
        Clear() {
        }
        void execute( Tokens& obj);
};

class WordCount{
    string output;
    string input;
    bool w_flag = false;
    bool c_flag = false;
    bool l_flag = false;

    public:
        int process_args(Tokens& obj);
        int execute( Tokens& obj);
        string getOutput() {
            return output;
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
        int execute( Tokens& obj);
        void mode_str(mode_t m, char *str);
        string& getOutput() {
            return output;
        }
};

class PrintDir{
    string output;
    public:
        int execute( Tokens& obj);
        string& getOutput() {
            return output;
        }
};

class Move {
    public:
        int execute(Tokens& obj);
        int renameFile(const string& source, const string& destination) {
            return rename(source.c_str(), destination.c_str());
        }
        string getFileName(const string& path);
};

class Fork {
    public:
        int execute(Tokens& obj);
};