#include "commands.hpp"

using namespace std;

class SimpleShell {
    const string prompt = " $ ";
    string current_dir;
    string input = "";
    vector<string> history;

    public:
        void printPrompt();
        void execute();
        int processCmd(string);
        int executeCommand(string, string&, int first = 0);
};

int isSpace( string str);
