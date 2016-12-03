SnailShell
by Julius Freyra, Ling Cheng, Steven LitVack-Winkler

Features:
	Forking and executes commands (however leaves out spoons and knives)
	Reads multiple commands on one line
	Redirects using > and <

Attempted:
	Could not get pipes to work

Bugs:
	Redirecting only works if there is only one argument before the
		redirect symbol (you cant use flags if you redirect a program)
	When reading multiple commands on a line, errors arise if there is a
	     	space between the first program and the ';'
	Doubling up on ';' and multiple syntax errors will break the parser

Function Headers:

	void cd()
	      Inputs: char *commands[]
	     Returns: Nothing

	     Changes the working directory of the parent based on the path
	     	given in the arguments in commands
	     If change fails, prints an error message and does not change
	     	directory


	int redirect_index()
	      Inputs: char *args[]
	      	      char *c
	     Returns: Index of c in args

	     Finds and returns the index of char* c in args
	     Returns -1 if not found
	     	     (useless, needed an int to return if fail,
		      but < > and | are all guaranteed to be in
		      input before calling redirect_index())


	void redirect()
	      Inputs: char *path
	      	      char *args[]
		      int  *ind
	     Returns: Nothing

	     Redirects inputs and outputs from stdin and stdout using dup()
	     Execvps commands
	     If arguments for input and output are invalid, prints error
	     	message


	void main()
	      Inputs: Nothing
	     Returns: Nothing

	     Handles everything else
	     Parses lines of commands and splits them into command arrays
	     Execvps non-redirected commands
	     Handles exiting