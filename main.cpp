/*
* Joshua Ruymen
* CS470 Lab 1
* Vajda
* 
* README: compiled using g++ with the command "g++ -o shell main.cpp commands.cpp"
* I have no idea how to not have the forked child not produce an error on the cwushell commands, but they still work :(
*/

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "commands.h"

using namespace std;

int main() {
    string cmd;

    int exitVal = 0;
    int temp = 0;

    pid_t nRet;

    do {
        print_prompt();
        getline(cin, cmd);

        nRet = fork();

        if (nRet == 0) {
            system(cmd.c_str());
            exit(0);
        }

        else {
            if (nRet > 0) {
                wait(NULL);
                temp = process_cmd(cmd);

                if (temp != 9999)
                    exitVal = temp;
            }
            else {
                cout << "Error creating the child process." << endl;
                exit(-1);
            }
        }        
    } while (checkExit());

    return exitVal;
}