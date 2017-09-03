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
	string branchName = email.substr(0,email.find("@"));
	
	system("echo \"$HOME/RemoteCommand\" > installDir");
	ifs.open("installDir");
	string installDir((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	ifs.close();
	installDir = installDir.substr(0,installDir.length()-1);
	
	cout << endl  << "Installing RemoteCommand for " << branchName << " in " << installDir << endl << endl;
	
	char toSystem[4096];
	sprintf(toSystem, "cd $HOME && git clone -b master https://github.com/justinthompson593/RemoteCommand.git && cd RemoteCommand && git branch %s && git checkout %s && echo \"# %s\" > README.md && git add -A && git commit -m \"New branch: %s\" && git push -u origin %s", branchName.c_str(), branchName.c_str(), branchName.c_str(), branchName.c_str(), branchName.c_str());
	system(toSystem);
}

int main(int argc, const char * argv[]) {
	
	for(int i=0; i<argc; i++){
		
		if( strncmp(argv[i], "--install", 9) == 0 )
			installRemoteCommand();
		
	}
	
	
	
	
	return 0;
}
