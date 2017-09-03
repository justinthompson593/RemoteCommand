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
	email = email.substr(0,email.length()-1);
	
	system("echo \"$HOME/RemoteCommand\" > installDir");
	ifs.open("installDir");
	string installDir((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	ifs.close();
	installDir = installDir.substr(0,installDir.length()-1);
	
	cout << endl  << "Installing RemoteCommand for " << email << " in " << installDir << endl << endl;
}

int main(int argc, const char * argv[]) {
	
	installRemoteCommand();
	
	
	
	return 0;
}
