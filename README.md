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

**Running a command in C**  
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

**Child and parent processes**  
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

**Piping**  
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

**What is a file descriptor?**  
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

**Redirecting the file descriptors**  
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


💡 **Note**: A **stack** can be seen as a pile of objects that are stacked on top
of each other. Think of a stack of plates. You would only be able to remove the
plate on the top -- if you try to remove a plate from anywhere else, the pile of plates
would potentially fall.

---

### **The rules of the project**

In the context of push_swap, we start off with a set of numbers and two stacks:
- Numbers 🔢 – A set of numbers. E.g. {5, 2, 1, -3, 7, 3}
- Stack A 📚 – Initially holds all the unsorted numbers.
- Stack B 📦 – Used as a temporary workspace to assist with sorting.

Since the numbers are placed in a stack, only the number on the top can be accessed.
Luckily, we have some operations to assist us in bringing our desired number to the top.
- sa, sb, ss 🔄 – Swap the top two elements of a stack.
- pa, pb 📤📥 – Push an element from one stack to another.
- ra, rb, rr 🔃 – Rotate (shift elements upward, moving the first to the last).
- rra, rrb, rrr 🔃 – Reverse rotate (shift elements downward, moving the last to the first).

The program is structured with separate sorting algorithms for different *amounts* of numbers.
- "Small sort algorithms" are implemented for 2, 3, 4 and 5 numbers.
- A "big sort algorithm" is implemented for > 5 numbers.  

**Error handling ❌**  
The program prints "Error\n" if: 
- There were duplicate numbers (3 3 5 4).
- Non-integer arguments were found (10 A 32 9).
- Any numbers are outside of integer range (5 241412515151524214 124 -1).  

If the numbers are already sorted, there is no output, because no operations
were needed.  

---

### **The turk algorithm**

`push_swap` is limited both when it comes to which operations are allowed, but
also *how many* operations are allowed for a set amount of numbers.
The subject gave the following requirements:
```bash
- For a minimalist validation (that implies a minimal grade of 80), you must be
able to sort 100 random numbers in fewer than 700 operations.
- For maximal project validation you must fulfill the first step above, but also for 500 random numbers, 
there should be no more than 5500 operations.
```

After some research online, I came across an algorithm called the **turk algorithm**.
It turned out to be super efficient, and secured me a maximum score in the push_swap project (100).

#### **These are the key points of the turk algorithm 🔑**  

Since the numbers in stack a and stack b are stored in a *linked list structure*, I will
refer to numbers as *nodes*.

**Main structure:**
1. Push all nodes from stack a to b, until there are *only three nodes left in stack a*.
2. During this process, the nodes are sorted in *descending order* in stack b.
3. When there are only three nodes left in stack a, sort them in *ascending order*.
4. Push the nodes back from stack b to stack a, so they are all sorted in *ascending order*.  

💡 **Note**: To determine which nodes are getting pushed, we use the concepts *target node*
and *cheapest node*.

**Target node:**
- When pushing a node from stack a to stack b -- the target node is the *closest smaller* node in stack b to the
node from stack a. If no node in stack b is smaller than the node from stack a, then the target
node is the *max* value.
- When pushing a node from stack b to stack a -- the target node is the *closest bigger* node in stack a to the
node from stack b. If no node in stack a is bigger than the node from stack b, then the target
node is the *min* value.
- Clarification: The node we want to push should be on the *top* of the target node.

**Cheapest node:**
- When a node is getting pushed from one stack to the other, we want to calculate the cheapest
node to push.
- The cheapest node means that it requires *the least amount of operations*.
```bash
Say we want to push a node from stack a to stack b.
We iterate through our stack a to see which node is cheapest to push, depending
on where it needs to go in stack b, to maintain the descending order in stack b.

The calculation will be based on this:
1. How many operations are needed to get the node in stack a to the top.
2. How many operations are needed to get the target node in stack b to the top.

Once we have iterated through all of stack a, and found the node that requires
the least amount of operations -- we do the operations and push the node from stack
a to stack b. 
```

The algorithm can be a bit hard to wrap your head around, especially without a more visual explanation.
Please have a look at these resources for more information:

- A. Yigit Ogun came up with the "turk algorithm" while working with push_swap. Have a look at this
Medium article, where both the push_swap project and the algorithm is thoroughly explained:
https://medium.com/@ayogun/push-swap-c1f5d2d41e97  
- I followed this YouTube tutorial by @Thuggonaut: https://www.youtube.com/watch?v=wRvipSG4Mmk&t=512s.
The tutorial gives an excellent walkthrough of the key concepts, and also gives some ideas about how to
code it.

---

### **Testing out push_swap**
As mentioned earlier, when the program is ran, it only prints out a list of operations.
This is not very rewarding, and can also be cryptic when dealing with large sets of numbers.

One way to get a better visualization is to see how many operations were done 
by piping the output into `wc -l`.
```bash
$> ./push_swap 9 2 1 5 3 | wc -l
```
In this repo there is also a `checker_linux` file, which will check if the operations
done output by `push_swap` are actually sorting the numbers.
```bash
$>ARG="4 67 3 87 23"; ./push_swap $ARG | ./checker_linux $ARG
```
If you want to print out the numbers AFTER they are sorted, follow these steps:
1. Go to the file `src/helper_functions/sorting_check.c`
2. In the first function `ft_finish_sort()`, add a print statement.
```bash
void	ft_finish_sort(t_list **a)
{
	ft_free_stack(*a);
	exit(0);
}
```
```bash
void	ft_finish_sort(t_list **a)
{
	t_list	*current; // Make a temporary pointer to not lose access to address of a.

	current = *a;
	while (current) // Add this while loop. The program will first print operations, then the numbers.
	{
		ft_printf("%i\n", current->value);
		current = current->next;
	}
	ft_free_stack(*a);
	exit(0);
}
```
-> Check out this webpage to avoid typing the numbers you want to test manually:
https://www.calculatorsoup.com/calculators/statistics/random-number-generator.php

-> Also check out this repo by @o-reo for a true visualization of what is actually happening:
https://github.com/o-reo/push_swap_visualizer

---

## 🧰 **Included Files**  
- All source files in directory `src`
- Two header files in `inc`
- A Makefile
- `checker_linux` used for testing if sorting is correct.

---

## 🧑‍💻 **Cloning the Repository**

To get started, first clone the repository.
This command will clone the repo, and move you to the right directory:
```bash
git clone https://github.com/busedame/push_swap/ push_swap && cd push_swap
```
---

## 🖥️ **How to Compile and Run**  

1. Compile using this command:
	```bash
	make
	```
2. Run using this command:
	```bash
	./push_swap <numbers, separated by space>
 	```

*This project was finished March 18th 2024.*