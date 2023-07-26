#include<iostream>
#include<stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Cat{
    string myText;
    string inp;

    public:
    void cat_command(string x)
    {inp=x;
        ifstream MyReadFile (inp);
      
        while (getline(MyReadFile, myText)) 
        {
             cout << myText;
        }

        MyReadFile.close();
    };
    void cat_command(string x,string y)
    {inp=y;
        if (x=="-n")
            {
                // cout<<"N"<<endl;
                
                ifstream MyReadFile (inp);
                int i=1;
                while (getline(MyReadFile, myText)) 
                {
                    cout <<i<< myText<<endl;
                    i++;
                }

                MyReadFile.close();

                    }
        else
            {cout<<"IN ELSE"<<endl;
                ifstream MyReadFile (x);
                int i=1;
                ofstream write_file(y);
                ifstream io;
                while (getline(MyReadFile, myText)) 
                {
                    write_file<< myText<<endl;
                    
                }
                
                write_file.close();
                MyReadFile.close();
            }
    }
};
int main()
{vector <string> tokens;
	string str;
    // vector <string> t{' ','\t',};
	cout << "Enter command \n";
	getline(cin, str);
	
 stringstream check1(str);
     
    string intermediate;
     
    while(getline(check1, intermediate,' '))
    {
        tokens.push_back(intermediate);
    }
     
    // Tokens
    // for(int i = 0; i < tokens.size(); i++)
    //     cout << tokens[i] << '\n';

if (tokens[0]=="cat") //cat command
{   cout<<"Here"<<endl;
    Cat c1;
    if (tokens.size()==2)
    {
        c1.cat_command(tokens[1]);
    }
    if (tokens.size()==3)
    {
        // cout<<444<<endl;
    c1.cat_command(tokens[1],tokens[2]);

    }
        
}


	return 0;
}
