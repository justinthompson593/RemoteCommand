//
//  main.cpp
//  RemoteCommand
//
//  Created by Justin Thompson on 9/2/17.
//  Copyright © 2017 Justin Thompson. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

void printCrontabHelp(){
	
	cout << "A crontab entry looks like this:\n\n3 */3 * * 0 /Path/To/executable\n\nThe first five entries of your crontab command must be numbers or *s. The one above will run \"executable\" located in /Path/To/ every 3rd hour at the 3rd minute on Sunday. Note that not all systems support the \"/\" syntax." << endl << endl;
	
	
	
	cout << "*     *     *   *    *        command to be executed" << endl;
	cout << "-     -     -   -    -" << endl;
	cout << "|     |     |   |    |" << endl;
	cout << "|     |     |   |    +----- day of week (0 - 6) (Sunday=0)" << endl;
	cout << "|     |     |   +------- month (1 - 12)" << endl;
	cout << "|     |     +--------- day of month (1 - 31)" << endl;
	cout << "|     +----------- hour (0 - 23)" << endl;
	cout << "+------------- min (0 - 59)" << endl << endl;
	
	
	
	cout << "min  hour  day     month  day  Execution time" << endl;
	cout << "30   0     1       1,6,12 *    00:30 hrs on Jan, Jun & Dec 1st" << endl;
	cout << "0    20    *       10     1-5  8:00 PM every weekday (Mon-Fri) in Oct." << endl;
	cout << "0    0     1,10,15 *      *    midnight on 1st,10th,15th of every month" << endl;
	cout << "5,7  1     10      *      1    1:05,1:07 every Monday & 10th of every month" << endl;
	
	cout << endl << "http://www.adminschoice.com/crontab-quick-reference" << endl;
}


// Assumes that both $HOME/RemoteCommand/RemoteCommand/oldCommand
// and $HOME/RemoteCommand/RemoteCommand/newCommand are defined
bool compareOldAndNew(){
	
	bool out = false;
	
	system("echo $HOME/RemoteCommand/RemoteCommand/ > RemCmdDir");
	ifstream pathFile("RemCmdDir");
	string Path((istreambuf_iterator<char>(pathFile)), istreambuf_iterator<char>());
	
	pathFile.close();
	Path = Path.substr(0, Path.length()-1);
	
	char buff[2048];
	sprintf(buff, "%soldCommand", Path.c_str());
	ifstream oldFile(buff);
	sprintf(buff, "%snewCommand", Path.c_str());
	ifstream newFile(buff);
	
	string oldF((istreambuf_iterator<char>(oldFile)), istreambuf_iterator<char>());
	string newF((istreambuf_iterator<char>(newFile)), istreambuf_iterator<char>());
	
	oldFile.close();
	newFile.close();

	if( oldF.compare(newF) == 0 )
		out = true;
	
	system("rm RemCmdDir");
	
	return out;
}

void installRemoteCommand(){
	
	
	char usrIn = getchar();
	string toCrontab = "";
	bool copyCrontabToClip = true;
	
	// Get email prefix for new branch name ( joesmith@web.net --> newBranchName joesmith )
	system("echo \"$(git config user.email)\" > gitEmail");
	ifstream ifs("gitEmail");
	string email((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	ifs.close();
	if( email.length() < 1 ){
		cout << "ERROR: Cannot find git user email. Installation aborted." << endl;
		return;
	}
	else{system("rm gitEmail");}
	string branchName = email.substr(0,email.find("@"));
	
	// Buffer for commands to system
	char toSystem[4096];
	
	// Ask user to install? Abort if q or Q
	sprintf(toSystem, "clear && echo \"\n\n\n\n\nInstalling RemoteCommand\n\nThis will create a directory named RemoteCommand in $HOME\n\nHit any key to continue or q to quit\"");
	system(toSystem);
	system("stty raw");
	
	system("stty cooked");
	if( strncmp(&usrIn, "q", 1) == 0 || strncmp(&usrIn, "Q", 1) == 0 ){
		cout << endl << endl << "Quitting." << endl;
		return;
	}
	
	// Clone RemoteCommand & push new branch
	sprintf(toSystem, "cd $HOME && git clone -b master https://github.com/justinthompson593/RemoteCommand.git && cd RemoteCommand && git branch %s && git checkout %s && echo -e \"# %s\nCreated on $(date)\" > README.md && git add -A && git commit -m \"New branch: %s\" && git push -u origin %s", branchName.c_str(), branchName.c_str(), branchName.c_str(), branchName.c_str(), branchName.c_str());
	system(toSystem);
	
	
	// Build RemoteCommand executable
	sprintf(toSystem, "cd $HOME/RemoteCommand/RemoteCommand && g++ main.cpp -o RemoteCommand");
	system(toSystem);
	
	// Create blank oldCommand and newCommand in directory with main.cpp
	sprintf(toSystem, "cd $HOME/RemoteCommand/RemoteCommand && touch oldCommand && chmod +x oldCommand && touch newCommand && chmod +x newCommand");
	system(toSystem);
	
	// Get crontab entry to paste to clipboard. The user prompt is:
	//
	//
	//	How often would you like to check for updates?
	//		m  every minute
	//		M  every n minutes (n will be requested next)
	//		h  every hour
	//		H  every n hours
	//		s  at a specific minute(s) each hour
	//		S  at a specific hour(s) each day
	//		e  something else (crontab help will be displayed)
	//		x  set crontab manually (crontab will be opened in nano)
	//
	//	Enter a letter shown above or any other key to set crontab manually.
	//
	//
	sprintf(toSystem, "echo \"\n\nHow often would you like to check for updates?\n\tm  every minute\n\tM  every n minutes (n will be requested next)\n\th  every hour\n\tH  every n hours\n\ts  at a specific minute each hour\n\tS  at a specific hour each day\n\te  something else (crontab help will be displayed)\n\nEnter a letter shown above or any other key to set crontab manually.\"");
	system(toSystem);
	system("stty raw");
	usrIn = getchar();
	system("stty cooked");

	switch (usrIn) {
  case 'm':
		{}
			break;
  case 'M':
		{
			// Get n number of min to check    USE int n = stoi(&usrIn);
		}
			break;
  case 'h':
		{}
			break;
  case 'H':
		{}
			break;
  case 's':
		{}
			break;
  case 'S':
		{}
			break;
  case 'e':
		{
			// Print crontab help
			//
			//
			//
			copyCrontabToClip = false;
			printCrontabHelp();
			sprintf(toSystem, "echo \"\nYou will be editing crontab manually. The path to the executable, RemoteCommand, is $HOME/RemoteCommand/RemoteCommand/. So your crontab entry should look something like this:\n\n0 * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand\"\n\nThe one above will check for a new command every hour on the hour. Edit the five preceeding entries to suit your needs.");
			system(toSystem);
		}
			break;
  default:
		{
			copyCrontabToClip = false;
			sprintf(toSystem, "echo \"\nYou have chosen to edit crontab manually. The path to the executable, RemoteCommand, is $HOME/RemoteCommand/RemoteCommand/. So your crontab entry should look something like this:\n\n0 * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand\"");
			system(toSystem);
		}
			break;
			
  
	}
	
	
	
	
	if(copyCrontabToClip){
		// Copy crontab entry to clipboard (OSX & unix & linux)
#if defined(__APPLE__) && defined(__MACH__)
		sprintf(toSystem, "echo \"%s\" | pbcopy", toCrontab.c_str());
		system(toSystem);
#elif defined(__unix__) || defined(__linux__)
		sprintf(toSystem, "echo \"%s\" > RemCmdClip && xsel --clipboard < RemCmdClip && rm RemCmdClip");
		system(toSystem);
#endif
		
		// Have user set crontab
		sprintf(toSystem, "clear && echo \"\n\n\nCalling crontab -e. The following line should be copied to your clipboard.\n\n%s\n\nWhen you continue, the nano editor will open. Paste the line into the editor (Command+v in OSX or Ctrl+Shift+V in *nix) to call RemoteCommand every minute, or edit it to suit your needs. When you're done, hit Ctrl+o to save and Ctrl+x to exit. To turn off mail notifications, add MAILTO=\\\"\\\" to the line above the one you're about to paste.\n\nHit any key to continue. This will open your crontab in the nano editor.\"", toCrontab.c_str());
		system(toSystem);
	}
	
	
	
	
	
	
	
	system("stty raw");
	usrIn = getchar();
	system("stty cooked");
	
	// Call crontab in nano
	sprintf(toSystem, "env EDITOR=nano crontab -e");
	system(toSystem);
	
	sprintf(toSystem, "echo \"\nTo change your crontab with nano again, call\n\nenv EDITOR=nano crontab -e\n\nin your teminal.\n\nInstallation complete. To run a \"");
	system(toSystem);
	
	
//	switch (updateFlag.at(1)) {
//  case 'o':																	// Online
//		{
//			
//			
//			
//			
//		}
//			break;
//  case 'l':																	// Local
//		{
//			
//			
//			// Have user set crontab
//			sprintf(toSystem, "clear && echo \"\n\n\nCalling crontab -e. The following lines should be copied to your clipboard.\n\n* * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand %s%s\nWhen you continue, the nano editor will open. Paste the lines into the editor (Command+v in OSX or Ctrl+Shift+V in *nix) to call RemoteCommand every minute, or edit it to suit your needs. When you're done, hit Ctrl+o to save and Ctrl+x to exit. To turn off mail notifications, add MAILTO=\\\"\\\" to the line above the ones you're about to paste.\n\nHit any key to continue. This will open your crontab in the nano editor.\"", updateFlag.c_str(), UpdateOptions.c_str());
//			system(toSystem);
//			
//			system("stty raw");
//			usrIn = getchar();
//			system("stty cooked");
//			
//			// Call crontab in nano
//			sprintf(toSystem, "env EDITOR=nano crontab -e");
//			system(toSystem);
//			
//			sprintf(toSystem, "echo \"\nTo change your crontab with nano again, call\n\nenv EDITOR=nano crontab -e\n\nin your teminal.\n\nInstallation complete. To run a \"");
//			system(toSystem);
//		}
//			break;
//
//  default:
//		{
//			sprintf(toSystem, "clear && echo \"\n\n\nCalling env EDITOR=nano crontab -e. Hit any key to continue.\"");
//			
//			system("stty raw");
//			usrIn = getchar();
//			system("stty cooked");
//			
//			// Call crontab in nano
//			sprintf(toSystem, "env EDITOR=nano crontab -e");
//			system(toSystem);
//			
//			sprintf(toSystem, "echo \"\nTo change your crontab with nano again, call\n\nenv EDITOR=nano crontab -e\n\nin your teminal.\n\nInstallation complete.\"");
//			system(toSystem);
//		}
//			break;
//	}

	
	
	
}

int main(int argc, const char * argv[]) {
	
	
	printCrontabHelp();
	
	return 222;
	
	for(int i=0; i<argc; i++){
		
		if( strncmp(argv[i], "--install", 9) == 0 ){
			installRemoteCommand();
			return 1;
		}
		
	}
	
	
	
	
	return 0;
}
