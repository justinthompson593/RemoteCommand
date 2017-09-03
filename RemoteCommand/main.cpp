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


void installRemoteCommand(){
	
	// Get email prefix for new branch name ( joesmith@web.net --> newBranchName joesmith
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
	sprintf(toSystem, "clear && echo -e \"\n\n\n\n\nInstalling RemoteCommand\n\nThis will create a directory named RemoteCommand in $HOME\n\nHit any key to continue or q to quit\"");
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
	
	// Create blank oldCommand in directory with main.cpp
	sprintf(toSystem, "cd $HOME/RemoteCommand/RemoteCommand && touch oldCommand && chmod +x oldCommand");
	
	sprintf(toSystem, "clear && echo -e \"\n\n\n\n\nCalling crontab -e. This will open your crontab in the nano editor. \n\nHit any key to continue\"");
	system(toSystem);
	
	system("stty raw");
	usrIn = getchar();
	system("stty cooked");
}

int main(int argc, const char * argv[]) {
	
	for(int i=0; i<argc; i++){
		
		if( strncmp(argv[i], "--install", 9) == 0 )
			installRemoteCommand();
		
	}
	
	
	
	
	return 0;
}
