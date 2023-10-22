#include "simpleshell.hpp"

int isSpace( string str) {
    int res = str.find_first_not_of(" \t\n\v\f\r");
    return res==-1;
}

void SimpleShell::printPrompt() {
    current_dir = filesystem::current_path();
    cout << "\e[1m" << current_dir << prompt << "\e[0m";
}
void SimpleShell::execute() {
    int flag = 1;
    char ch;
    int len, i;
    string cmd;
    len = history.size();
    i = len - 1;
    while(flag) {
        printPrompt();
        getline(cin, cmd);
        // cmd.append("\0");
        // cout << "cmd:"<<cmd<<"here"<<endl;
        flag = processCmd(cmd);
        history.push_back(cmd);
        len = history.size();
        i = len -1;
    }
}
int SimpleShell::processCmd(string cmd_chain) {
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
            res = executeCommand(cmd, output);
            if( res == -1 ) {
                cout << output << endl;
                return -1;
            }
            input = output;
            next = ind + 1;
        }
        if( next < (cmd_chain.size() -1) ) {
            cmd = cmd_chain.substr(next, cmd_chain.size() - 1);
            res = executeCommand(cmd, output);
            if( res == -1 || res == 1) {
                cout << output << endl;
                return -1;
            }
        }
        if(output!="")
            cout << output << endl;
    }
    return 1;
}     
int SimpleShell::executeCommand(string cmd, string& out, int first) {
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
        else {
            res = c1.execute(tokens);
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
        if(tokens.num_args() == 0)
            tokens.tokens.push_back(current_dir);
        int res;
        try {
            ls.execute(tokens);
        }
        catch ( const  exception& e) {
            cout << "error: " << e.what();
            cout << endl;
            return -1;
        }
        if( res == -1 ) return 1;
        out = ls.getOutput();
    }
    else if (tokens[0] == "wc") // wc command
    {
        WordCount wc;
        int res;
        if(tokens.num_args() < 1) {
            res = -1;
            cout << "error: Insufficient Arguments" << endl;
        }
        else {
            try {
                wc.execute(tokens);
            }
            catch ( const  exception& e) {
                cout << "error: " << e.what();
                cout << endl;
                return -1;
            }
            if( res == -1 ) return 1;
            out = wc.getOutput();
        }
    }
    else if (tokens[0] == "mkdir") // mkdir command
    {
        MakeDirectory mdir;
        int res;
        if(tokens.num_args() < 1) {
            res = -1;
            cout << "error: Insufficient Arguments" << endl;
        }
        else {
            try {
                mdir.execute(tokens);
            }
            catch ( const  exception& e) {
                cout << "error: " << e.what();
                cout << endl;
                return -1;
            }
            if( res == -1 ) return 1;
            out = mdir.getOutput();
        }
    }
    else if (tokens[0] == "mv") // mv command
    {
        Move mv;
        int res;
        if(tokens.num_args() < 2) {
            res = -1;
            cout << "error: Insufficient Arguments" << endl;
        }
        else {
            try {
                mv.execute(tokens);
            }
            catch ( const  exception& e) {
                cout << "error: " << e.what();
                cout << endl;
                return -1;
            }
            if( res == -1 ) return 1;
        }
    }
    else if (tokens[0]=="touch") // touch command
    {
        class Touch A;
        A.execute(tokens);
    }
    else if (tokens[0]=="exit") // exit command
    {
        exit(0);
    }
    else {
        // sys call here
        try {
            if(!first) {
                ofstream outFile("tempfile");
                outFile.close();
                cmd = cmd + " tempfile";
                system(cmd.c_str());
                system("rm tempfile");
            }
            else {
                system(cmd.c_str());
            }
        }
        catch ( const  exception& e) {
            cout << "error: " << e.what();
            cout << endl;
            return -1;
        }
        return 0;
        // cout << "Command " + tokens[0] + " not found." << endl;
        // return -1;
    }
    return 0;
}
