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
	
	// Get crontab entry to paste to clipboard. The user prompt is:
	//
	//
	//	How often would you like to check for updates?
	//		m  every minute
	//		M  every n minutes (n will be requested next)
	//		h  every hour
	//		H  every n hours
	//		s  at a specific minute each hour
	//		S  at a specific hour each day
	//		e  something else
	//		x  set crontab manually (crontab will be opened in nano)
	//
	//	Enter a letter shown above or any other key to set crontab manually.
	//
	//
	sprintf(toSystem, "echo \"\n\nHow often would you like to check for updates?\n\tm  every minute\n\tM  every n minutes (n will be requested next)\n\th  every hour\n\tH  every n hours\n\ts  at a specific minute each hour\n\tS  at a specific hour each day\n\te  something else\n\tx  set crontab manually (crontab will be opened in nano)\n\nEnter a letter shown above or any other key to set crontab manually.\"");
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
  default:
		{
			sprintf(toSystem, "echo \"\nYou have chosen to edit crontab manually. The path to the executable, RemoteCommand, is $HOME/RemoteCommand/RemoteCommand/. So your crontab entry should look something like this:\n\n0 * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand\"");
			system(toSystem);
		}
			break;
			
  
	}
	
	// Create blank oldCommand and newCommand in directory with main.cpp
	sprintf(toSystem, "cd $HOME/RemoteCommand/RemoteCommand && touch oldCommand && chmod +x oldCommand && touch newCommand && chmod +x newCommand");
	system(toSystem);
	
	
	// Copy crontab entry to clipboard (OSX & unix & linux)
#if defined(__APPLE__) && defined(__MACH__)
	sprintf(toSystem, "echo \"* * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand\" | pbcopy");
	system(toSystem);
#elif defined(__unix__) || defined(__linux__)
	sprintf(toSystem, "echo \"* * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand\" > toClip && xsel --clipboard < toClip && rm toClip");
	system(toSystem);
#endif
	
	
	// Have user set crontab
	sprintf(toSystem, "clear && echo \"\n\n\nCalling crontab -e. The following line should be copied to your clipboard.\n\n* * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand %s\n\nWhen you continue, the nano editor will open. Paste the line into the editor (Command+v in OSX or Ctrl+Shift+V in *nix) to call RemoteCommand every minute, or edit it to suit your needs. When you're done, hit Ctrl+o to save and Ctrl+x to exit. To turn off mail notifications, add MAILTO=\\\"\\\" to the line above the one you're about to paste.\n\nHit any key to continue. This will open your crontab in the nano editor.\"", updateFlag.c_str());
	system(toSystem);
	
	system("stty raw");
	usrIn = getchar();
	system("stty cooked");
	
	// Call crontab in nano
	sprintf(toSystem, "env EDITOR=nano crontab -e");
	system(toSystem);
	
	sprintf(toSystem, "echo \"\nTo change your crontab with nano again, call\n\nenv EDITOR=nano crontab -e\n\nin your teminal.\n\nInstallation complete. To run a \"");
	system(toSystem);
	
	
	switch (updateFlag.at(1)) {
  case 'o':																	// Online
		{
			
			
			
			
		}
			break;
  case 'l':																	// Local
		{
			
			
			// Have user set crontab
			sprintf(toSystem, "clear && echo \"\n\n\nCalling crontab -e. The following lines should be copied to your clipboard.\n\n* * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand %s%s\nWhen you continue, the nano editor will open. Paste the lines into the editor (Command+v in OSX or Ctrl+Shift+V in *nix) to call RemoteCommand every minute, or edit it to suit your needs. When you're done, hit Ctrl+o to save and Ctrl+x to exit. To turn off mail notifications, add MAILTO=\\\"\\\" to the line above the ones you're about to paste.\n\nHit any key to continue. This will open your crontab in the nano editor.\"", updateFlag.c_str(), UpdateOptions.c_str());
			system(toSystem);
			
			system("stty raw");
			usrIn = getchar();
			system("stty cooked");
			
			// Call crontab in nano
			sprintf(toSystem, "env EDITOR=nano crontab -e");
			system(toSystem);
			
			sprintf(toSystem, "echo \"\nTo change your crontab with nano again, call\n\nenv EDITOR=nano crontab -e\n\nin your teminal.\n\nInstallation complete. To run a \"");
			system(toSystem);
		}
			break;

  default:
		{
			sprintf(toSystem, "clear && echo \"\n\n\nCalling env EDITOR=nano crontab -e. Hit any key to continue.\"");
			
			system("stty raw");
			usrIn = getchar();
			system("stty cooked");
			
			// Call crontab in nano
			sprintf(toSystem, "env EDITOR=nano crontab -e");
			system(toSystem);
			
			sprintf(toSystem, "echo \"\nTo change your crontab with nano again, call\n\nenv EDITOR=nano crontab -e\n\nin your teminal.\n\nInstallation complete.\"");
			system(toSystem);
		}
			break;
	}

	
	
	
}

int main(int argc, const char * argv[]) {
	
	
	for(int i=0; i<argc; i++){
		
		if( strncmp(argv[i], "--install", 9) == 0 ){
			installRemoteCommand();
			return 1;
		}
		
	}
	
	
	
	
	return 0;
}
