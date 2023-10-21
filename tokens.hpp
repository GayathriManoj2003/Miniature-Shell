#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
class Tokens {
    public:
        vector <string> tokens;
        Tokens(string);
        string operator[](int index);
        int num_args();
        int isSpace(string);
};
