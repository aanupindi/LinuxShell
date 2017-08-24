nclude <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;

void shell_read_line(string& command){
	cout<<"aki>";
	getline(cin,command);
}

char** command_parse(const string& command){
	string localcopy = command;
	
	string buf; // Have a buffer string
    std::stringstream ss(localcopy); // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words
    char** cTokens;

    while (ss >> buf)
        tokens.push_back(buf);

    cTokens = (char**)malloc(tokens.size()*sizeof(char*));
    int i = 0;
    for(;i<tokens.size();++i){
    	cTokens[i] = (char*)tokens[i].c_str();
    }

    return cTokens;

}

//find the application and launch it
int sh_pr_launch(char** commands){
	 pid_t newPid, wpid;
  	 int status;

  	 newPid = fork(); //fork a new process.
  	 if(newPid == 0){
  	 	//name of the program and vector of string args.
  	 	//p means the OS will find the path of the program.
  	 	if(execvp(commands[0], commands) == -1)
  	 		cout<<"Could not launch new process"; //call this command with arguments
  	 	
  	 	exit(EXIT_FAILURE); //exit with failure so the parent knows that the child process is done.

  	 }
  	 else if(newPid > 0){
  	 	//parent process starts here.
  	 	do{
  	 		wpid = waitpid(newPid, &status, WUNTRACED);
  	 	}while (!WIFEXITED(status) && !WIFSIGNALED(status));
  	 }

  	 return 1;	

}

void loop_shell(){
	string command;
	char** args;

	while(1){
		shell_read_line(command);
		args = command_parse(command);
		int status = sh_pr_launch(args);
	}
}

int main(int argc, char const *argv[])
{
	loop_shell();
	return 0;
}
