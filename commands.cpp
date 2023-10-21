#include "commands.hpp"
#include <sys/stat.h>

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
    char current_dir[4096];
    getcwd(current_dir, sizeof(current_dir));
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
    char *path = new char[p_str.size() + 1];
    int i = 0;
    for( char c: p_str) {
        path[i++] = c;
    }
    path[i] = '\0';
    dirp = opendir(path);
    if (dirp == NULL) 
    {   
        stream << "No such File or Directory." << endl;
        return -1;
    }
    if(l)
        stream <<left<< setw(30) << "Permissions" << "File Name" << endl;
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
            char perm[10];
            if(l) {
                status = stat(file_path.c_str(), &stats);
                mode_str(stats.st_mode, perm);
                stream << left << setw(30) << perm << file_p->d_name << endl;
            }
            else { 
                // cout << file_p->d_name << endl;
                stream << file_p->d_name <<"\t";
            }
        }
    }while (file_p != NULL);

    output = stream.str();
    // cout << "output" << output;
    closedir(dirp);
    delete[] path;
    path = NULL;
    delete file_p;
    file_p = NULL;
    return 0;
};
void ListDir::mode_str(mode_t m, char *str) {
    str[0]= (m & S_IRUSR)? 'r':'-';
    str[1]= (m & S_IWUSR)? 'w':'-';
    str[3]= (m & S_IRGRP)? 'r':'-';
    str[2]= (m & S_IXUSR)? 'x':'-';
    str[4]= (m & S_IWGRP)? 'w':'-';
    str[5]= (m & S_IXGRP)? 'x':'-';
    str[6]= (m & S_IROTH)? 'r':'-';
    str[7]= (m & S_IWOTH)? 'w':'-';
    str[8]= (m & S_IXOTH)? 'x':'-';
    str[9]= '\0';
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
    int ind = 1;

    string p_str;
    p_str = str;
    string path = "";
    string rev_p_str = string(p_str.rbegin(), p_str.rend());
    bool flag = false;
    int index = -1;
    int i = p_str.size() - 1;
    string dir_name = "";
    while( i >= 0 ) {
        char c = rev_p_str[i];
        if(c == '/') {
            flag = true;
            break;
        }
        i--;
        dir_name.push_back(c);
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