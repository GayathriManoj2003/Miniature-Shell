#include<iostream>
#include<stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
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
class Fork
{public:
    int fork_t(string x)
    {
        char *n=new char[x.size()+1]; //convert string to char for exec parameter.
        int i=0;                        // otherwise string can't be given.
        for (i=0;i<x.size();i++)
        {
            n[i]=x[i];
        }
        n[i]='\0';
        
        pid_t p;
            int res;
            p = fork();
            // cout<<"3";
            if(p > 0)
            {
                res = wait(NULL);
            }
            else if(!p)
            {
                cout << "\nInside Child process.";
                char *args[2]={n,NULL};
                // char *args[2]={"./exec.out",NULL};
                execvp(args[0],args);
                exit(0);
            }
            delete[] n;
            return res;
    }
};
class my_exec
{   vector <string> token1;
    public:
    my_exec(vector<string>tokens)
    {
        token1=tokens;
        if (token1.size()==1)
        {   Fork proc;
        // cout<<2;
            proc.fork_t(token1[0]);

        }
    }
};

class my_cd{
    public:
    my_cd(string x)
    {
        auto path =filesystem::current_path(); //getting path
        // string x="/home/vboxuser/Summer_3/Day3";
        std::filesystem::current_path(x); //setting path
        char current_dir[100];
        getcwd(current_dir, sizeof(current_dir));
        cout<<current_dir;

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
    for(int i = 0; i < tokens.size(); i++)
        cout << tokens[i] << '\n';

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
if (tokens[0].rfind("./", 0) == 0) //execute program //should add parameters.
        {  cout<< "PROG";
        my_exec m1(tokens);  

        }
if (tokens[0]=="cd")  //cd
{
    
    my_cd c1(tokens[1]);
}


	return 0;
}
