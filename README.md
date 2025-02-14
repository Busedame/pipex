# 🔧 **pipex - An introduction to redirections and pipes!**

**In this README:**
1. Introduction to the project
2. Running a command in C
3. Child and parent processes
4. Piping
5. Redirection
4. Included files
5. Cloning the repository
6. How to compile and run

## 🚀 **Project Overview**  
The project involves redirecting input and output, executing and piping commands.

The program is run like this:
```bash
$> ./pipex file1 cmd1 cmd2 file2
```
This works the same way as this shell command:
```bash
$>  < file1 cmd1 | cmd2 > file2
```

- `file1` The input file. Where is the input to the program coming from. It should be the name of the file, e.g. "infile".
- `cmd1` The first command. E.g. "cat".
- `cmd2` The second command. E.g. "wc -l".
- `file2` The output file. Where is the output of the program redirected to. It should be the name of the file, e.g. "outfile".

**Running a command in C ▶️💻**  
A shell command is a binary file/an executable, and can essentially be seen as a program. E.g. `cat`, `grep` and `awk` are only a few of many shell commands.  
When I started working with Pipex, I had a moment of being scared that I would have to recode all the shell commands. I was happy when
I discovered that this was not the case, and that there is a function that takes care of all of this for you.  
The function is declared like this:
```bash
	int execve(const char *pathname, char *const argv[], char *const envp[]);
```
- `const char *pathname`: The absolute path to the executable/command. E.g. /usr/local/sbin
- `char *const argv[]`: An array of arguments to the command. E.g. in `ls -l` --> `-l` would be the argument.
- `char *const envp[]`: An array of the environmental variables (passed as an argument to the main program itself).
- **Return value**: Does NOT return upon success, because it takes over the current process. Returns -1 upon error,
with errno set to indicate the cause of error.  

The catch about `execve()` is that it takes over the whole process. Once we call the function once, our process ends, 
because a different program takes over. This would have been excellent if we only wanted to execute one command. However in
the context of Pipex, two commands are getting executed. How can we work around this, when `execve()` takes over the
whole process?

**Child and parent processes 👨‍👧**  
When a program is ran, it is one process. You can see it as a **parent process/main process**, or whatever you would like
to call it. In order to avoid that `execve()` takes over our whole program - we can create a **child process**. You can think of it
as a kind of parallel universe -- the processes are very similar, but will probably do different operations.  
Instead of calling `execve()` in the parent process, we call it in a child process, ensuring that our main process continues
running.
- **Parent process**: The "main" process - which can creates one or more child processes. It controls and monitors them, often waiting for their execution to complete.
- **Child process**: A process created by a parent process. It is a copy of the parent, but the parent and child have separate memory spaces. Changes (like variables, memory, execution) made in the child process will not affect the parent process.  

Even though the parent and child are running quite independently from one another, there are some exceptions:
- If they use inter-process communication (IPC) (e.g., signals), they can affect each other.
- If they share resources (e.g., files, pipes), changes in one process can be visible to both.  

Now, how do we create a child process? This is done by calling the following function:
```bash
	pid_t fork(void);
```
`pid_t`: The process ID. This is a crucial value, to be able to distinguish between a child and parent process.
As the program moves on after the `fork()` function is called, we most likely don't want the parent and child to do the
same thing.  
This is how we can read the return value from `fork()`:
- `0`: This is a child process.
- `>0`: This is a parent process (because the value is the pid of the child).  
- `-1`: An error occured (no child process is created). Errno is set appropriately.

To make it less cryptic:
```bash
int	main()
{
	pid_t	pid; // Stores the process ID.

	pid = fork(); // Creates the child process.
	if (!pid)
			printf("This is a child process.\n");
	else if (pid > 0)
			printf("This is the parent process. The created child has pid: %d\n", pid);
	else
	{
			fprintf(stderr, "Something went wrong.\n");
			return (1);
	}
	return (0);
}
```
If we run this example, it will give us this output:
```bash
	$> ./a.out 
	This is the parent process. The created child has pid: 28566
	This is a child process.   
```

**Piping 🚰**  
A pipe `|` can be seen as a one-way communication channel between processes. It has two ends - one for reading
and one for writing.  
In the context of two commands (e.g. `ls | wc -l`): 
- The pipe uses the *output* of `ls` as *input* for `wc -l`.  

In C, a pipe can be created like this:
```bash
	int	fd[2];

	pipe(fd); // A pipe gets created for fd, fd[1] is the write-end, and fd[0] is the read-end.
```
The pipe process can be seen as follows:
```bash
	cmd1 ---> fd[1] ---> fd[0] ---> cmd2

	1. Data from cmd1 gets output, and is written into the write-end (fd[1]).
	2. The data can be read by the read-end (fd[0]), it generates
	an output, which is used as input for cmd2.

	Example:
	ls | wc -l

	ls -> Writes to fd[1] (A list of the files/directories in the current directory).
	wc -l -> Reads from fd[0] (Which now contains the output of 'ls') Uses this as 
	input to count how many lines this consists of, and outputs this.
```

**What is a file descriptor? 📂**  
A file descriptor (fd) makes the operating system able to identify an open file or resource. It is represented by a small positive integer value.
When a process/program is ran, the standard file descriptors (0, 1, 2) are opened by default.
By default -- standard output (1) is used by `printf()`, and standard input (0) is used by `scanf()`.

- **Standard input (STDIN, 0)** is the input from the keyboard - or typically what you write in the terminal while a program is running. The program
*reads* from the input.  
- **Standard output (STDOUT, 1)** is the output - which gets printed directly in the terminal. The program *writes* to the output.  
- **Standard error (STDERR, 2)** is the error output - which also gets printed directly in the terminal. It is reserved for error messages. The program
*writes* to the output.  
An example:
```bash
int	main()
{
	int	x;
	printf("Enter your age here:");
	scanf("%i", &x);
	fprintf(stderr, "You are too old\n");
}
```
In this example:
- Output is "Enter your age". 
- User is prompted to input their age (in the terminal).
- "You are too old" is output to stderr.
```bash
	$> Enter your age: 42
	$> You are too old
```
In C, if you want to open more file descriptors - like a file "test.txt" - it has to be opened using the `open()` function from <fcntl.h>.
This function essentially connects the file and the file descriptor.  
Each opened file will get its unique fd. Since 0, 1 and 2 are reserved for stdin, stdout and stderr - the first opened file will have fd=3, next one fd=4, etc.  

**Redirecting the file descriptors 🔀**  
So we know that file descriptors 0, 1 and 2 are already reserved by default, and that if you open a new file, it will be assigned different file descriptor values.
What if I told you there is a way to *replace* fd 0, 1 and 2 with a file of your choice?
Redirection involves telling the operating system, "Hey! Instead of reading from the terminal, read from this file instead!". Or "Write everything I print with printf
to this file, instead of to the terminal!".  

To achieve this, you can use the `dup2()` function from <unistd.h>.
It is declared like this:
```bash
	int dup2(int oldfd, int newfd);
```
An example of how to use it:
```bash
	int		input;

	input = open(pipex->argv[1], O_RDONLY); // file1 is opened and given an fd(3).
	if (dup2(input, STDIN_FILENO) == -1) // Input source is redirected from stdin(0) to fd(3).
		print_error_first_child(pipex, 3);
	// Now, file1 has fd(0) instead of fd(3), and the file will be used as input.
```

---

## 🧰 **Included Files**  
- All source files in directory `src`
- Two header files in `inc`
- A Makefile

---

## 🧑‍💻 **Cloning the Repository**

To get started, first clone the repository.
This command will clone the repo, and move you to the right directory:
```bash
git clone https://github.com/busedame/pipex/ pipex && cd pipex
```
---

## 🖥️ **How to Compile and Run**  

1. Compile using this command:
	```bash
	make
	```
2. Run using this command:
	```bash
	./pipex <file1> <cmd1> <cmd2> <file2>
 	```

*This project was finished March 18th 2024.*