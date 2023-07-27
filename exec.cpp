//SOMEWHAT WORKS
// #include <unistd.h>
// #include <stdlib.h>
// #include <ncurses.h>

// #define LOCAL_ESCAPE_KEY 27

// int main(void)
// {

// int key;

// // initscr();
// crmode();
// keypad(stdscr, TRUE);
// noecho();
// key = getch();

// while(key != ERR && key != 'q') {
	

// 	if ((key >= 'A' && key <= 'Z')||(key >= 'a' && key <= 'z')){
// 	printw("Key was %c", (char)key);
// 	}
// 	else {
// 	switch(key){
// 	case LOCAL_ESCAPE_KEY: printw("%s", "Escape key"); break;
// 	case KEY_END: printw("%s", "END key"); break;
// 	case KEY_RIGHT: printw("%s", "RIGHT key"); break;
// 	case KEY_LEFT: printw("%s", "LEFT key"); break;
// 	case KEY_UP: printw("%s", "UP key");break;
// 	case KEY_DOWN: printw("%s", "DOWN key"); break;
// 	default: printw("Unmatched - %d", key); break;
// 	}/* switch */
// 	}/*else*/
	
// 	refresh();
// 	key = getch();
// 	}/* while */
// 	endwin();
// exit(EXIT_SUCCESS);
// };


// //}

#include <iostream>
#include <termios.h>
#include <sstream>
#include <vector>
#define STDIN_FILENO 0
using namespace std;
int main()
{
// Black magic to prevent Linux from buffering keystrokes.
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

// Once the buffering is turned off, the rest is simple.
    cout << "Enter a character: ";
    // vector <string> Commands;
    // Commands.push_back();
    
    string x="Apple is there";
    string y="Ball";
    string z="UGWHO";
    vector <string> commands;
    commands.push_back(x);
    commands.push_back(y);
    commands.push_back(z);
    int size_arr=commands.size();
    int i=size_arr-1;
    char c,d,e;
    while(c!='\n')
    {cin >> c;
    cin >> d;
    cin >> e;
    cout << "\nYour character was ";
    if ((c==27)&&(d==91)) {
        if (e==65) {  //Up

            if (i==0)
               {i=0;
                cout << "UP"<<endl;cout<<commands[i]<<endl;
               }
            // else if(i==size_arr-1)
            // {

            //     cout << "UP"<<endl;cout<<commands[i]<<endl;
            //     i--;

            // }
            else{                
            cout << "UP"<<endl;cout<<commands[i]<<endl;
            --i;
            }
            }
        if (e==66) { //Down
            if (i==size_arr-1)
                {
                    i=size_arr-1;
                    cout << "DOWN"<<endl;cout<<commands[i]<<endl;
                }
            else
            {
                ++i;
                cout << "DOWN"<<endl;cout<<commands[i]<<endl;
            }
            
            }
        if (e==67) { cout << "RIGHT";}
        if (e==68) { cout << "LEFT";}
    }

    }
    
    return 0;
}

