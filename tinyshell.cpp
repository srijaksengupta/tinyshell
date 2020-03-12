/*
   Srijak Sengupta
*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <ctime>
#include <time.h>
#include <sys/types.h> //needed for using type definitions ( e.g., pid_t )
#include <sys/wait.h> //needed for using waitpid()
#include <sys/utsname.h>
#include <unistd.h> //needed for using getpid()

using namespace std;

class Cmds
{
    public:
      string comm;
      double exectime;
      friend bool compare (const Cmds&, const Cmds&);
};//end of class definition

vector<Cmds>commandlist;
void execute(string, int);

bool compare (const Cmds& m, const Cmds& n)
{
	if(m.exectime <= n.exectime)
  {
		return true;
  }
}//end of compare

string handlespacing(string& str)
{
    str.erase(str.find_last_not_of(" ") + 1);
    str.erase(0, str.find_first_not_of(" "));
    return str;
}//end of handlespacing

void implementfork(int history, int sbu, string command)
{
        pid_t childpid = fork();
	      clock_t starttime;
	      clock_t endtime;
        if(childpid < 0)
        {
            cout << "Internal error : Cannot implement fork";
        }
        else
        if(childpid  == 0)
        {
            //This is the child
	        starttime = clock();
            if(history == 1)
            {
                execute("history", sbu);
            }
            else
            {
                execute(command, sbu);
            }
        }
	      else
	      {
	        //This is the parent
	        if ((childpid = waitpid(childpid, 0, 0)) < 0)
            cout<<"Internal error : Waiting for child not possible"<<endl;
	        else
	        {
		        Cmds x;
		        x.comm = command;
		        endtime = clock();
		        x.exectime =  (double)(endtime - starttime)/(double)(CLOCKS_PER_SEC);
	    	    commandlist.push_back(x);
	        }
        }
        cout << "tinyshell> ";
}//end of implementfork

void execute(string command, int sbu)
{
    if(command=="history")
    {
        int c;
        if(sbu == 1)
	    {
	        //Using the concept of reverse_iterator (r.begin() and r.end())
	  	    vector<Cmds>::reverse_iterator itr1;
	        c = 1;
		    vector<Cmds>sbucommands;
		    Cmds a;
	        for(itr1=commandlist.rbegin();itr1!=commandlist.rend();itr1++)
	        {
		        if(c <= 5)
		        {
			        a.comm = (*itr1).comm;
			        a.exectime = (*itr1).exectime;
		   	        sbucommands.push_back(a);
		        }
		        c++;
	        }
	        //We have to sort the commands based on execution duration
		    sort(sbucommands.begin(), sbucommands.end(), compare);
		    //Again using the concept of reverse_iterator (r.begin() and r.end())
		    vector<Cmds>::reverse_iterator itr2;
	        c = 1;
	        for(itr2=sbucommands.rbegin();itr2!=sbucommands.rend();itr2++)
	        {
		        if(c <= 5)
		        {
		   	        cout << (*itr2).comm << "  " << (*itr2).exectime << "s" << endl;
		            c++;
	            }
	        }
        }
        else
	    if(sbu == 0)
	    {
	        //Using the concept of reverse_iterator (r.begin() and r.end())
	        vector<Cmds>::reverse_iterator itr3;
	        c = 1;
	        for(itr3=commandlist.rbegin();itr3!=commandlist.rend();itr3++)
	        {
		        if(c <= 5)
		        {
		   	        cout << (*itr3).comm << "  " << (*itr3).exectime << "s" << endl;
		            c++;
	            }
	        }
	    }
	}
    else
    {
	    const char *cmd = command.c_str();
	    system(cmd);
    }
  exit(0);
}//end of execute


int main()
{
    string command = "";
    cout << "tinyshell> ";
    while(getline(cin, command))
    {
        handlespacing(command);
        int history = 0;
        int sbu = 0;
        char cstr[command.size()+1];
        strcpy(cstr,command.c_str());
        char delimiter[]=" ";
        char *token = strtok(cstr,delimiter);
        int x=0;

        while (token)
        {
          string s=token;
          if(x==0)
          {
              if(s == "exit")
              {
                  exit(0);
              }
              if(s == "history")
              {
                  history = 1;
              }
          }
          else
          if(x==1)
          {
            if(s == "-sbu")
              {
                  sbu = 1;
              }
          }
          token = strtok(NULL,delimiter);
          x=x+1;
        }
        implementfork(history,sbu,command);
    }
    exit(0);

}//end of main
