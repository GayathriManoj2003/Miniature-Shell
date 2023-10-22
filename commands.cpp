#include "commands.hpp"
#include <sys/stat.h>
#include <utime.h>

int Cat::cat_command(string x)
{
    input = x;
    char *str = new char[x.size() + 1];
    ifstream catFile(input);
    stringstream stream;
    if(!catFile) {
        stream << "cat: " + input + ": No such file or directory" << endl;
        output = output + stream.str();
        catFile.close();
        return -1;
    }
    stream << catFile.rdbuf();
    output = output + stream.str();
    catFile.close();
    return 0;
}
int Cat::cat_command(string x,string y)
{   
    input=y;
    if (x=="-n") {
        ifstream catfile (input);
        if(!catfile) {
            cout << "cat: " + input + ": No such file or directory" << endl;
            return -1;
        }
        int i=1;
        stringstream stream;
        while (getline(catfile, myText)) {
            stream <<i<< myText<<endl;
            i++;
        }
        output = stream.str();
        catfile.close();
    }
    else
    {
        ifstream catFile1(x);
        if (!catFile1)
        {
            cout << "cat: " + x + ": No such file or directory" << endl;
        }
        while (getline(catFile1, myText))
        {
            cout << myText << endl;
        }
        cout << "\b";
        ifstream catFile2(input);
        if (!catFile2)
        {
            cout << "cat: " + y + ": No such file or directory" << endl;
        }
        while (getline(catFile2, myText))
        {
            cout << myText << endl;
        }
        catFile2.close();

        catFile1.close();
    }
    return 0;
}
int Cat::execute(Tokens &obj)
{   
    if(obj.num_args() < 1) return -1;
    string file;
    int ind = 1;
    int n = obj.num_args();
    int res = 0;
    for(; ind <= n ; ind++) {
        file = obj[ind];
        res = cat_command(file);
    }
    return res;
}
string Cat::getOutput() {
    return output;
}


int WordCount::process_args(Tokens& obj){
    int ind = -1;
    if(obj.num_args() == 1) {
        w_flag = c_flag = l_flag = true;
        if(obj[1][0] != '-')
            ind = 1;
        input = obj.tokens[1];
        return ind;
    }
    else {
        for(int i = 0; i < obj.num_args(); i++) {
            if(obj[i+1] == "-w")
                w_flag = true;
            else if(obj[i+1] == "-l")
                l_flag = true;
            else if(obj[i+1] == "-c")
                c_flag = true;
            else if(obj[i+1][0] != '-')
                ind = i + 1;
        }
        if(ind != -1)
            input = obj.tokens[ind];
    }
    return ind;
}
int WordCount::execute( Tokens& obj) {
    int ind = process_args(obj);
    if(ind == -1) {
        cout << "wc: invalid arguments passed" << endl;
        return -1;
    }
    string x = obj.tokens[ind];
    char *str = new char[x.size() + 1];
    ifstream wcFile(input);
    if(!wcFile) {
        cout << "wc: " + input + ": No such file or directory" << endl;
        return -1;
    }
    stringstream stream;
    stream << wcFile.rdbuf();
    string file_content;
    file_content = stream.str();

    int lines = 0;
    int words = 0;
    int characters = 0;
    for(auto c: file_content) {
        characters++;
        if(c == '\n') {
            lines++;
        }
    }
    if(w_flag) {
        string word;
        while(stream >> word) {
            words++;
        }
    }
    stringstream stream_op;
    if(l_flag) stream_op << lines << " ";
    if(w_flag) stream_op << words << " ";
    if(c_flag) stream_op << characters;
    // stream_op << endl;
    wcFile.close();
    output = stream_op.str();
    return 0;
}

int ListDir::execute( Tokens& obj)
{
    int status;
    struct stat stats;
    stringstream stream;
    DIR *dirp;
    struct dirent *file_p;
    int l = 0, a = 0;
    int ind = -1;
    
    for(int i = 0; i < obj.num_args(); i++) {
        if(obj[i+1] == "-a")
            a = 1;
        else if(obj[i+1] == "-l")
            l = 1;
        else if(obj[i+1] == "-al")
            a = l = 1;
        else if(obj[i+1][0] != '-')
            ind = i + 1;
    }
    string p_str;
    if(ind != -1)
        p_str = obj.tokens[ind];
    else {
        p_str = filesystem::current_path();
    }
    
    string path(p_str);

    dirp = opendir(path.c_str());
    if (dirp == NULL) 
    {   
        stream << "No such File or Directory." << endl;
        return -1;
    }
    if(l) {
        stream <<left
        << setw(10) << "Permissions" 
        << right
        << setw(8) << "Size "
        << left
        << setw(15) << " Access Time" 
        << "File Name" << endl;
    }
    file_p = new struct dirent();
    do 
    {
        file_p = readdir(dirp);
        if(file_p != NULL) {
            if(!strcmp(file_p->d_name, "."))
                continue;
            if(!strcmp(file_p->d_name, ".."))
                continue;
            if(!a && file_p->d_name[0] == '.')
                continue;
            string sl = "/";
            string file_path = path + sl + string(file_p->d_name);
            string perm;
            if(l) {
                status = stat(file_path.c_str(), &stats);
                struct tm file_time = *(localtime(&stats.st_atime));
                mode_str(stats.st_mode, perm);
                stream << left
                << setw(10) << perm 
                << right
                << setw(8) << (long)stats.st_size 
                << "  "
                << left
                << setw(15) << getTimeString(file_time) 
                << file_p->d_name << endl;
            }
            else { 
                stream << file_p->d_name << "\t";
            }
        }
    }while (file_p != NULL);

    output = stream.str();
    closedir(dirp);
    delete file_p;
    file_p = NULL;
    return 0;
};
string ListDir::getTimeString(struct tm &file_time){
    string month;
    stringstream timestrm;
    switch(file_time.tm_mon) {
        case 0: month = "Jan"; break;
        case 1: month = "Feb"; break;
        case 2: month = "Mar"; break;
        case 3: month = "Apr"; break;
        case 4: month = "May"; break;
        case 5: month = "Jun"; break;
        case 6: month = "Jul"; break;
        case 7: month = "Aug"; break;
        case 8: month = "Sep"; break;
        case 9: month = "Oct"; break;
        case 10: month = "Nov"; break;
        case 11: month = "Dec"; break;
    }

    timestrm << month + " " 
    << setw(2) << setfill('0') << file_time.tm_mday 
    << " " 
    << setw(2) << setfill('0') << file_time.tm_hour 
    << ":" 
    << setw(2) << setfill('0') << file_time.tm_min;
    return timestrm.str();

}
void ListDir::mode_str(mode_t m, string &str) {
    str.push_back((m & S_IRUSR)? 'r':'-');
    str.push_back((m & S_IWUSR)? 'w':'-');
    str.push_back((m & S_IRGRP)? 'r':'-');
    str.push_back((m & S_IXUSR)? 'x':'-');
    str.push_back((m & S_IWGRP)? 'w':'-');
    str.push_back((m & S_IXGRP)? 'x':'-');
    str.push_back((m & S_IROTH)? 'r':'-');
    str.push_back((m & S_IWOTH)? 'w':'-');
    str.push_back((m & S_IXOTH)? 'x':'-');
}

int PrintDir::execute( Tokens& obj) {
    if( obj.num_args() > 0 )
    {
        cout << "pwd does not accept arguments." << endl;
        return -1;
    }
    output = filesystem::current_path();
    return 0;
};

int Fork::execute(Tokens& obj) {
    vector <string> cmd_tok=obj.tokens;
    
    pid_t p;
    int res;
    p = fork();
    if(p > 0) {
        res = wait(NULL);
    }
    else if(!p)
    {                
        char *args[cmd_tok.size()+1];
        int index=0;int i=0;int k=0;
        for (i=0;i<cmd_tok.size();i++)
        {   
            char *exec_name= new char[cmd_tok[i].size()+1];
            for (index=0;index<cmd_tok[0].size();index++)
            {
                exec_name[index]=cmd_tok[i][index];
            }
            args[i]=exec_name;
        }
        args[i]=NULL;
        
        execvp(args[0],args);
        exit(0);
    }
    return res;
}

int MakeDirectory::execute(Tokens& obj) {
    if(obj.num_args() < 1)
        return -1;
    int res = 0;
    int n = obj.num_args();
    int ind = 1;
    for(;ind <= n; ind++) {
        int result = createDir(obj[ind]);
        if( result != 0 ) res = result;
    }
    return res;
}

int MakeDirectory::createDir(string str) {
    DIR *dirp;
    string p_str(str);
    string path = "";
    string dir_name = "";

    int n = p_str.size();
    bool flag = false;
    int i = n -1;
    while( i >= 0) {
        char c = p_str[i];
        if(c == '/') {
            flag = true;
            break;
        }
        i--;
        dir_name = c + dir_name;
    }
    if(flag && i>0) {
        int j = 0;
        while( j <= i) {
            path.push_back(p_str[j]);
            j++;
        }
        if(path[0] != '/') {
            path = "/" + path;
            string cur = filesystem::current_path();
            path = cur + path;
        }
    }
    else {
        path = filesystem::current_path();
    }
    
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {   
        cout << dir_name + ": cannot create directory." << endl;
        return -1;
    }

    string path_dir = "";
    if(flag)
        path_dir = path + dir_name;
    else
        path_dir = path + "/" + dir_name;

    if (stat(path_dir.c_str(), &st) == 0) {
        cout << dir_name + ": directory already exists" << endl;
        return -1;
    }

    filesystem::create_directory(path_dir.c_str());

    return 0;
}

string MakeDirectory::getOutput() {
    return output;
}

int ChangeDir::execute(string dir_path)
{
    try {
        filesystem::current_path(dir_path);
    }
    catch ( const filesystem::filesystem_error& exception) {
            cout << "error: " << exception.what();
            cout << endl;
            return -1;
    }
    return 0;
};

void Clear::execute( Tokens& obj)
{
    if( obj.num_args() > 0 )
    {
        cout << "clear does not accept any arguments." << endl;
        return;
    }

    // ANSI Escape Sequences used
    cout << "\033[2J\033[1;1H";
};

string Move::getFileName(const string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != string::npos) {
        return path.substr(pos + 1);
    }
    return path;
}
int Move::execute(Tokens& obj) {
        if (obj.num_args() != 2) {
            cout << "mv: missing source and/or destination arguments" << endl;
            return -1;
        }

        string source = obj[1];
        string destination = obj[2];

        struct stat sourceStat, destStat;
        if (stat(source.c_str(), &sourceStat) != 0) {
            cout << "mv: cannot access '" << source << "': No such file or directory" << endl;
            return -1;
        }

        if (stat(destination.c_str(), &destStat) == 0 && S_ISDIR(destStat.st_mode)) {
            // Destination is a directory, move the source into the directory
            string newDestination = destination + "/" + getFileName(source);
            if (renameFile(source, newDestination) == 0) {
                cout << "Moved '" << source << "' to '" << newDestination << "'" << endl;
                return 0;
            } else {
                cout << "mv: error moving file" << endl;
                return -1;
            }
        } else {
            // Destination is a file or a non-existent path, rename the source
            if (renameFile(source, destination) == 0) {
                cout << "Renamed '" << source << "' to '" << destination << "'" << endl;
                return 0;
            } else {
                cout << "mv: error renaming file" << endl;
                return -1;
            }
        }
    }

int Touch::execute( Tokens& obj) {

    vector<string>input_tok=obj.tokens;
    string dir;

    int count=1;
    int flag=0;
    int args = obj.num_args();
    int max = args;

    count=1;
    int res = 0;
    while (count<=max)
    {
        string file=obj.tokens[count];
        struct stat st;
        string path(file);
        
        if (stat(path.c_str(), &st) == 0) {
            struct timespec ts;    
            clock_gettime(CLOCK_REALTIME, &ts);
            struct utimbuf updated_time;
            updated_time.actime = ts.tv_sec;
            updated_time.modtime = ts.tv_sec;
            if (utime(path.c_str(), &updated_time) != 0) {
                return -1;
            }
        }
        else {
            string p_str(file);
            string path = "";
            string file_name = "";

            bool flag = false;
            int n = p_str.size();
            int i = n -1;
            while( i >= 0) {
                char c = p_str[i];
                if(c == '/') {
                    flag = true;
                    break;
                }
                i--;
                file_name = c + file_name;
            }
            i++;
            if(flag && i > 0) {
                int j = 0;
                while( j < i) {
                    path.push_back(p_str[j]);
                    j++;
                }
                if(path[0] != '/') {
                    path = "/" + path;
                    string cur = filesystem::current_path();
                    path = cur + path;
                }
            }
            else path = filesystem::current_path();

            if(stat(path.c_str(), &st) != 0) {
                cout << "touch: directory " + path + " does not exist" << endl;
                res = -1;
            }
            else {
                string path_dir = "";
                if(flag)
                    path_dir = path + file_name;
                else
                    path_dir = path + "/" + file_name;

                try {
                    ofstream Mfile(path_dir);
                    Mfile.close();
                }
                catch( const exception & e) {
                    cout << e.what() ;
                    res = -1;
                }
            }
        }
        count++;
    }
        
    return res;
}