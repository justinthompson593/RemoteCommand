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
	system("echo \"$(git config user.email)\" > gitEmail");
	ifstream ifs("gitEmail");
	string email((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	ifs.close();
	
	if( email.length() < 1 ){
		cout << "ERROR: Cannot find git user email. Installation aborted." << endl;
		return;
	}
	else{
		system("rm gitEmail");
	}
	
	string branchName = email.substr(0,email.find("@"));
	
	char toSystem[4096];
	
	sprintf(toSystem, "clear && echo -e \"\n\n\n\n\nInstalling RemoteCommand\n\nThis will create a directory named RemoteCommand in $HOME\n\nHit any key to continue or q to quit\"");
	system(toSystem);
	
	
	system("stty raw");
	char usrIn = getchar();
	system("stty cooked");
	
	if( strncmp(&usrIn, "q", 1) == 0 || strncmp(&usrIn, "Q", 1) == 0 ){
		cout << endl << endl << "Quitting." << endl;
		return;
	}
	
	
	sprintf(toSystem, "cd $HOME && git clone -b master https://github.com/justinthompson593/RemoteCommand.git && cd RemoteCommand && git branch %s && git checkout %s && echo -e \"# %s\nCreated on $(date)\" > README.md && git add -A && git commit -m \"New branch: %s\" && git push -u origin %s", branchName.c_str(), branchName.c_str(), branchName.c_str(), branchName.c_str(), branchName.c_str());
	system(toSystem);
	
	sprintf(toSystem, "cd $HOME/RemoteCommand/RemoteCommand && touch oldCommand");
}

int main(int argc, const char * argv[]) {
	
	for(int i=0; i<argc; i++){
		
		if( strncmp(argv[i], "--install", 9) == 0 )
			installRemoteCommand();
		
	}
	
	
	
	
	return 0;
}
