#include "tokens.hpp"

int Tokens::num_args() {
    return tokens.size() - 1;
}
int Tokens::isSpace( string str) {
    int res = str.find_first_not_of(" \t\n\v\f\r");
    return res==-1;
}

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