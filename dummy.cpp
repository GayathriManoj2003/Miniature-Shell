#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include <filesystem>
using namespace std;
//cd
// int main() {
//     auto path = std::filesystem::current_path(); //getting path
//     std::filesystem::current_path("/home/vboxuser/Summer_3/Day3"); //setting path
//     char current_dir[4096];
//     getcwd(current_dir, sizeof(current_dir));
//     cout<<current_dir;
// }

//Process
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
        //A null terminated array of character
        //pointers
        char *args[2]={"./a.out",NULL};
        execvp(args[0],args);
     
        /*All statements are ignored after execvp() call as this whole
        process(execDemo.c) is replaced by another process (EXEC.c)
        */
        printf("Ending-----");
     
    return 0;
}


