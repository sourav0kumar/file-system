# In-Memory File System

This project implements an in-memory file system with various functionalities. The file system supports the following operations:

- **mkdir:** Create a new directory.
- **cd:** Change the current directory. Supports navigating to the parent directory using `..`, moving to the root directory using `/`, and navigating to a specified absolute path.
- **ls:** List the contents of the current directory or a specified directory.
- **grep:** Search for a specified pattern in a file.
- **cat:** Display the contents of a file.
- **touch:** Create a new empty file.
- **echo:** Write text to a file.
- **mv:** Move a file or directory to another location.
- **cp:** Copy a file or directory to another location.
- **rm:** Remove a file or directory.

## Implemented Functionalities

### mkdir
Create a new directory. Example: `mkdir directory_name`

### cd
Change the current directory. Examples:
- `cd directory_name`
- `cd /absolute/path`
- `cd ..` (move to parent directory)
- `cd /` (move to root)

### ls
List the contents of the current directory or a specified directory. Example: `ls`


### cat
Display the contents of a file. Example: `cat file_name`

### touch
Create a new empty file. Example: `touch file_name`

### echo
Write text to a file. Example: `echo 'content' > file_name`

### mv
Move a file or directory to another location. Example: `mv source_path destination_path`

### cp
Copy a file or directory to another location. Example: `cp source_path destination_path`

### rm
Remove a file or directory. Example: `rm file_or_directory_path`


## How to Run

To run the program locally:

1. Clone the repository: `git clone https://github.com/your-username/your-repo.git`
2. Navigate to the project directory: `cd your-repo`
3. Compile the C++ code: `g++ -o try try.cpp`
4. Run the compiled program: `./try`

## How to Test

To test the implemented functionalities, you can use the provided command-line interface. For example:

- Create a directory: `mkdir test_directory`
- Change to the new directory: `cd test_directory`
- Create a new file: `touch test_file.txt`
- Write to the file: `echo 'Hello, World!' > test_file.txt`
- Display the file contents: `cat test_file.txt`


