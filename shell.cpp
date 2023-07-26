#include <iostream>
#include<stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class Cat{
    string myText;
    string inp;
    public:
    void cat_command(string x)
    {
        inp=x;
        ifstream MyReadFile (inp);
        while (getline(MyReadFile, myText))
        {
             cout << myText;
        }

        MyReadFile.close();
    };
};
int main()
{
    while(true)
    {
        vector <string> tokens;
        string str;

        cout << "$ ";
        getline(cin, str);

        stringstream check1(str);
        string intermediate;

        while(getline(check1, intermediate, ' '))
        {
            tokens.push_back(intermediate);
        }

        // Printing the token vector
        for(int i = 0; i < tokens.size(); i++)
            cout << tokens[i] << '\n';

        if (tokens[0]=="cat") //cat command
        {
            Cat c1;
            c1.cat_command(tokens[1]);
        }
        if (tokens[0]=="exit") //exit command
        {
            exit(0);
        }

    }

	return 0;
}
