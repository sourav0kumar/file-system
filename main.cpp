#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

class InMemoryFileSystem
{
private:
    string current_directory;
    unordered_map<string, string> filesystem;

public:
    InMemoryFileSystem() : current_directory("/") {}

    void mkdir(const string &directory_name)
    {
        string new_directory_path = get_absolute_path(directory_name);
        cout << new_directory_path << endl;
        if (filesystem.find(new_directory_path) == filesystem.end())
        {
            filesystem[new_directory_path] = "";
        }
        else
        {
            cout << "Directory '" << directory_name << "' already exists." << endl;
        }
    }

    void cd(const string &path)
    {

        if (path == "..")
        {
            size_t pos = current_directory.find_last_of('/');
            if (pos != string::npos)
            {
                current_directory = current_directory.substr(0, pos);
            }
            else
            {
                // If already at root, stay at root
                current_directory = "/";
            }
        }
        else if (path == "/")
        {
            current_directory = "/";
        }
        else
        {
            string new_directory_path = get_absolute_path(path);
            if (filesystem.find(new_directory_path) != filesystem.end())
            {
                current_directory = new_directory_path;
            }
            else
            {
                cout << "Directory '" << path << "' not found." << endl;
            }
        }
    }

    void ls()
    {
        cout << "Contents of " << current_directory << ": ";
        for (const auto &entry : filesystem)
        {
            if (entry.first.find(current_directory) == 0 && entry.first != current_directory)
            {
                cout << entry.first.substr(current_directory.length()) << " ";
            }
        }
        cout << endl;
    }

    void grep(const string &pattern)
    {
        bool flag=false;
        for (const auto &entry : filesystem)
        {
            if (entry.second.find(pattern) != string::npos)
            {
                flag=true;
                cout << "Pattern found in " << entry.first << endl;
            }
            
        }if(!flag){
            cout << " not found"<<endl;
        }
    }

    void cat(const string &file_name)
    {
        string file_path = get_absolute_path(file_name);
        if (filesystem.find(file_path) != filesystem.end())
        {
            cout << filesystem[file_path] << endl;
        }
        else
        {
            cout << "File '" << file_name << "' not found." << endl;
        }
    }

    void touch(const string &file_name)
    {
        string file_path = get_absolute_path(file_name);
        if (filesystem.find(file_path) == filesystem.end())
        {
            filesystem[file_path] = "";
        }
        else
        {
            cout << "File '" << file_name << "' already exists." << endl;
        }
    }

    void echo(const string &content, const string &file_name)
    {
        string file_path = get_absolute_path(file_name);
        if (filesystem.find(file_path) != filesystem.end())
        {
            filesystem[file_path] = content;
        }
        else
        {
            cout << "File '" << file_name << "' not found." << endl;
        }
    }

    void mv(const string &source, const string &destination)
    {
        string source_path = get_absolute_path(source);
        string destination_path = get_absolute_path(destination);

        if (filesystem.find(source_path) != filesystem.end())
        {
            filesystem[destination_path] = filesystem[source_path];
            filesystem.erase(source_path);
        }
        else
        {
            cout << "Source file/directory '" << source << "' not found." << endl;
        }
    }

    void cp(const string &source, const string &destination)
    {
        string source_path = get_absolute_path(source);
        string destination_path = get_absolute_path(destination);

        if (filesystem.find(source_path) != filesystem.end())
        {
            filesystem[destination_path] = filesystem[source_path];
        }
        else
        {
            cout << "Source file/directory '" << source << "' not found." << endl;
        }
    }

    void rm(const string &path)
    {
        string item_path = get_absolute_path(path);
        if (filesystem.find(item_path) != filesystem.end())
        {
            filesystem.erase(item_path);
        }
        else
        {
            cout << "Item '" << path << "' not found." << endl;
        }
    }

    string get_absolute_path(const string &path)
    {
        if (path.front() == '/')
        {
            return path;
        }
        else
        {
            return current_directory + '/' + path;
        }
    }

    void save_state(const string &file_path)
    {
        ofstream outfile(file_path);
        if (outfile.is_open())
        {
            for (const auto &entry : filesystem)
            {
                outfile << entry.first << "|" << entry.second << "\n";
            }
            outfile.close();
            cout << "File system state saved successfully." << endl;
        }
        else
        {
            cout << "Unable to save file system state." << endl;
        }
    }

    void load_state(const string &file_path)
    {
        ifstream infile(file_path);
        if (infile.is_open())
        {
            filesystem.clear();
            string line;
            while (getline(infile, line))
            {
                size_t pos = line.find("|");
                if (pos != string::npos)
                {
                    string path = line.substr(0, pos);
                    string content = line.substr(pos + 1);
                    filesystem[path] = content;
                }
            }
            infile.close();
            cout << "File system state loaded successfully." << endl;
        }
        else
        {
            cout << "Unable to load file system state." << endl;
        }
    }
};

int main(int argc, char *argv[])
{
    InMemoryFileSystem filesystem;

    if (argc > 1)
    {
        string input = argv[1];
        if (input.find("{'save_state': 'true'") != string::npos)
        {
            size_t pos = input.find("'path': '");
            string file_path = input.substr(pos + 9, input.find("'", pos + 9) - (pos + 9));
            filesystem.save_state(file_path);
            return 0;
        }
        else if (input.find("{'load_state': 'true'") != string::npos)
        {
            size_t pos = input.find("'path': '");
            string file_path = input.substr(pos + 9, input.find("'", pos + 9) - (pos + 9));
            filesystem.load_state(file_path);
        }
    }

    string command;
    while (true)
    {
        cout << "Enter command: ";
        getline(cin, command);

        // Tokenize the command based on space, but treat everything after 'echo' and before the next '>' as one token
        size_t echoPos = command.find("echo");
        vector<string> tokens;
        if (echoPos != string::npos)
        {
            string echoPart = command.substr(echoPos + 5); // Skip 'echo'

            size_t nextSpace = echoPart.find('>');
            
            if (nextSpace != string::npos)
            {
                string contentToken = echoPart.substr(0, nextSpace);      // Content before '>'
                string destinationToken = echoPart.substr(nextSpace + 2); // Content after '>'


                tokens.push_back("echo");       // Add 'echo' as the first token
                tokens.push_back(contentToken); // Treat content before '>' as one token
                tokens.push_back(destinationToken);

                
            }
        }
        else
        {
            // Standard tokenization for other commands
            istringstream iss(command);

            string token;
            while (iss >> token)
            {
                tokens.push_back(token);
            }
        }

        if (tokens.size() == 0)
        {
            continue;
        }

        if (tokens[0] == "mkdir")
        {
            
            if (tokens.size() > 1)
            {
                filesystem.mkdir(tokens[1]);
            }
            else
            {
                cout << "Invalid command. Usage: mkdir <directory_name>" << endl;
            }
        }
        else if (tokens[0] == "cd")
        {
          
            if (tokens.size() > 1)
            {
                filesystem.cd(tokens[1]);
            }
            else
            {
                cout << "Invalid command. Usage: cd <path>" << endl;
            }
        }
        else if (tokens[0] == "ls")
        {
            
            filesystem.ls();
        }
        else if (tokens[0] == "grep")
        {
        
            if (tokens.size() > 1)
            {
                filesystem.grep(tokens[1]);
            }
            else
            {
                cout << "Invalid command. Usage: grep <pattern>" << endl;
            }
        }
        else if (tokens[0] == "cat")
        {
            
            if (tokens.size() > 1)
            {
                filesystem.cat(tokens[1]);
            }
            else
            {
                cout << "Invalid command. Usage: cat <file_name>" << endl;
            }
        }
        else if (tokens[0] == "touch")
        {
            
            if (tokens.size() > 1)
            {
                filesystem.touch(tokens[1]);
            }
            else
            {
                cout << "Invalid command. Usage: touch <file_name>" << endl;
            }
        }

        else if (tokens[0] == "echo")
        {

            
            if (tokens.size() > 2)
            {

                string content = tokens[1];
                // Adjusted to handle content with spaces
                filesystem.echo(content, tokens[2]);
            }
            else
            {
                cout << "Invalid command. Usage: echo <content> > <file_name>" << endl;
            }
        }
        else if (tokens[0] == "mv")
        {
            if (tokens.size() > 2)
            {
                filesystem.mv(tokens[1], tokens[2]);
            }
            else
            {
                cout << "Invalid command. Usage: mv <source> <destination>" << endl;
            }
        }
        else if (tokens[0] == "cp")
        {
            if (tokens.size() > 2)
            {
                filesystem.cp(tokens[1], tokens[2]);
            }
            else
            {
                cout << "Invalid command. Usage: cp <source> <destination>" << endl;
            }
        }
        else if (tokens[0] == "rm")
        {
            if (tokens.size() > 1)
            {
                filesystem.rm(tokens[1]);
            }
            else
            {
                cout << "Invalid command. Usage: rm <path>" << endl;
            }
        }
        else if (tokens[0] == "exit")
        {
            break;
        }
        else
        {
            cout << "Invalid command. Try again." << endl;
        }
    }

    return 0;
}
