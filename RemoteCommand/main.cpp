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
	Path = Path.substr(0, Path.length()-2);
	
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
	char usrIn = getchar();
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
	
	
	// Ask user how to update
	string updateFlag = "-";				// -o Check online for updates / -l Check locally for updates
	string UpdateOptions = "\n";
	
	sprintf(toSystem, "echo \"\n\nHow would you like to check for updates?\n\to  check online\n\tl  check locally\n\nEnter a letter shown above or any other key to set crontab manually.\"");
	system(toSystem);
	system("stty raw");
	usrIn = getchar();
	system("stty cooked");
	updateFlag += usrIn;
	
	// Create blank oldCommand and newCommand in directory with main.cpp
	sprintf(toSystem, "cd $HOME/RemoteCommand/RemoteCommand && touch oldCommand && chmod +x oldCommand && touch newCommand && chmod +x newCommand");
	system(toSystem);
	
	switch (updateFlag.at(1)) {
  case 'o':																	// Online
		{
			// Create blank oldCommand in directory with main.cpp
			sprintf(toSystem, "cd $HOME/RemoteCommand/RemoteCommand && touch oldCommand && chmod +x oldCommand");
			system(toSystem);
			
			// Copy crontab entry to clipboard (OSX & unix & linux)
#if defined(__APPLE__) && defined(__MACH__)
			sprintf(toSystem, "echo \"* * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand -o\" | pbcopy");
			system(toSystem);
#elif defined(__unix__) || defined(__linux__)
			sprintf(toSystem, "echo \"* * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand -o\" > toClip && xsel --clipboard < toClip && rm toClip");
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
		}
			break;
  case 'l':																	// Local
		{
			
			
			
			
			// Copy crontab entry to clipboard (OSX & unix & linux)
#if defined(__APPLE__) && defined(__MACH__)
			sprintf(toSystem, "echo \"* * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand -l%s\" | pbcopy", UpdateOptions.c_str());
			system(toSystem);
#elif defined(__unix__) || defined(__linux__)
			sprintf(toSystem, "echo \"* * * * * $HOME/RemoteCommand/RemoteCommand/RemoteCommand -l%s\" > toClip && xsel --clipboard < toClip && rm toClip", localUpdateOptions.c_str());
			system(toSystem);
#endif
			
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
		
		if( strncmp(argv[i], "--install", 9) == 0 )
			installRemoteCommand();
		
	}
	
	
	
	
	return 0;
}
