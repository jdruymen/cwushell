#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "commands.h"

using namespace std;

string prompt = "cwushell> ";
bool useShell = true;

void print_prompt() {
    cout << prompt;
}

bool checkExit() {
    return useShell;
}

int process_cmd(string cmd) {
    if (cmd == "" || cmd == " ")
        return 0;

    string args[2];

    if (cmd.at(cmd.length() - 1) == ' ')
        cmd = cmd.substr(0, cmd.length() - 1);

    if (cmd.find(' ') != string::npos) {
        args[0] = cmd.substr(0, cmd.find(' '));
        args[1] = cmd.substr(cmd.find(' ') + 1, cmd.length());
    }
    else {
        args[0] = cmd;
        args[1] = " ";
    }

    return execute_cmd(args);
}

int execute_cmd(string args[]) {
    if (args[0] == "exit") {
        if (args[1] == " ") {
            useShell = false;
            return 9999;
        }
        else if (args[1] == "-h" || args[1] == "-help")
            exitManual();
        else if (args[1].find_first_not_of("0123456789") != string::npos) {
            cout << "Unknown exit command. Opening exit manual..." << endl;
            exitManual();
        }
        else {
            useShell = false;
            return stoi(args[1]);
        }
        return 0;
    }

    else if (args[0] == "prompt") {
        if (args[1] == " ") {
            prompt = "cwushell> ";
            return 1;
        }
        else if (args[1] == "-h" || args[1] == "-help")
            promptManual();
        else {
            prompt = args[1];
            return 1;
        }
        return 0;
    }

    else if (args[0] == "cpuinfo") {
        if (args[1] == "-c") {
            get_clocksp();
            return 1;
        }
        else if (args[1] == "-t") {
            get_vendor();
            return 1;
        }
        else if (args[1] == "-n") {
            get_cores();
            return 1;
        }
        else if (args[1] == "-h" || args[1] == "-help")
            cpuManual();
        else {
            cout << "Unknown cpuinfo command. Opening cpuinfo manual..." << endl;
            cpuManual();
        }

        return 0;
    }

    else if (args[0] == "meminfo") {
        if (args[1] == "-t") {
            get_memory();
            return 1;
        }
        else if (args[1] == "-u") {
            get_memory_used();
            return 1;
        }
        else if (args[1] == "-c") {
            get_cache();
            return 1;
        }
        else if (args[1] == "-h" || args[1] == "-help")
            memManual();
        else {
            cout << "Unknown meminfo command. Opening meminfo manual..." << endl;
            memManual();
        }

        return 0;
    }

    else if (args[0] == "manual") {
        manual();
    }

    return 0;
}

void get_clocksp() {
    ifstream cpuinfo("/proc/cpuinfo");
    string input;
    int speed;

    while (getline(cpuinfo, input)) {
        if (input.substr(0, 7) == "cpu MHz") {
            stringstream ss;
            ss << input;

            string temp;
            while (!ss.eof()) {
                ss >> temp;

                if (stringstream(temp) >> speed)
                    break;
            }

            break;
        }
    }

    cout << "Clock speed: " << (speed / 1000.0) << " GHz" << endl;
    cpuinfo.close();
}

void get_vendor() {
    ifstream cpuinfo("/proc/cpuinfo");
    string type;

    while (getline(cpuinfo, type)) {
        if (type.substr(0, 9) == "vendor_id") {
            type = type.substr(type.find(":") + 2, type.length());
            break;
        }
    }

    cout << "CPU type: " << type << endl;
    cpuinfo.close();
}

void get_cores() {
    ifstream cpuinfo("/proc/cpuinfo");
    string input;
    int cores;

    while (getline(cpuinfo, input)) {
        if (input.substr(0, 9) == "cpu cores") {
            stringstream ss;
            ss << input;

            string temp;

            while (!ss.eof()) {
                ss >> temp;

                if (stringstream(temp) >> cores)
                    break;
            }

            break;
        }
    }
    cout << "Number of cores: " << cores << endl;
}

void get_memory() {
    ifstream meminfo("/proc/meminfo");
    string memTotal;
    while (meminfo >> memTotal) {
        if (memTotal == "MemTotal:") {
            meminfo >> memTotal;
            break;
        }
    }

    cout << "Memory total: " << memTotal << " KB" << endl;

    meminfo.close();
}

void get_memory_used() {
    ifstream meminfo("/proc/meminfo");
    string input;
    string memTotal;
    string memFree;
    while (meminfo >> input) {
        if (input == "MemTotal:") {
            meminfo >> memTotal;
        }

        else if (input == "MemAvailable:") {
            meminfo >> memFree;
            break;
        }

        else {
            meminfo >> input;
        }
    }

    cout << "Used memory: " << (stoi(memTotal) - stoi(memFree)) << " KB" << endl;


    meminfo.close();
}

void get_cache() {
    ifstream cpuinfo("/proc/cpuinfo");
    string input;
    int cache;

    while (getline(cpuinfo, input)) {
        if (input.substr(0, 10) == "cache size") {
            stringstream ss;
            ss << input;

            string temp;

            while (!ss.eof()) {
                ss >> temp;

                if (stringstream(temp) >> cache)
                    break;
            }

            break;
        }
    }

    cout << "L2 cache size: " << cache << " KB" << endl;

    cpuinfo.close();
}

void manual() {
    cout << "- - - - - - - CWUSHELL MANUAL - - - - - - -" << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "\tA simple Linux shell built for CWU CS470 by Joshua Ruymen" << endl;
    cout << "COMMANDS:" << endl;
    cout << "\texit (n): " << "Exits the shell program with the provided exit value. If no value is given, exits with the value of the last executed command, or 0 if no commands were executed." << endl;
    cout << "\tprompt [new_prompt]: " << "Changes the prompt to new_prompt. If no new prompt is given, reverts back to the default of 'cwushell>'." << endl;
    cout << "\tcpuinfo -[switch]: " << "Switches: " << endl << "\t\t-c: prints the cpu clock speed." << endl << "\t\t-t: prints the cpu type." << endl << "\t\t-n: prints the number of cpu cores." << endl;
    cout << "\tmeminfo -[switch]: " << "Switches: " << endl << "\t\t-t: prints the total system RAM in kilobytes." << endl << "\t\t-u: prints the amount of used RAM in kilobytes." << endl << "\t\t-c: prints the size of the L2 processor cache in kilobytes." << endl;
    cout << endl << "!!! FOR HELP WITH COMMANDS, TYPE [command] -h OR [command] -help !!!" << endl;
}

void exitManual() {
    cout << "- - - - - - - EXIT MANUAL - - - - - - -" << endl;
    cout << "PROLOG:" << endl;
    cout << "\tThis manual page is a part of the CWUSHELL manual. Type manual for more information." << endl;
    cout << "NAME:" << endl;
    cout << "\texit -- exits the cwushell program." << endl;
    cout << "SYNOPSIS:" << endl;
    cout << "\texit [num]" << endl;
    cout << "\texit" << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "\tThe exit command exits the shell program with the provided exit value. If no value is give, the command will exit with the value of the last executed command, or 0 if no commands were executed." << endl;
}

void promptManual() {
    cout << "- - - - - - - PROMPT MANUAL - - - - - - -" << endl;
    cout << "PROLOG:" << endl;
    cout << "\tThis manual page is a part of the CWUSHELL manual. Type manual for more information." << endl;
    cout << "NAME:" << endl;
    cout << "\tprompt -- changed the prompt for the shell." << endl;
    cout << "SYNOPSIS:" << endl;
    cout << "\tprompt [new_prompt]" << endl;
    cout << "\tprompt" << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "\tThe prompt command changes the prompt of the shell to new_prompt. If no new prompt is provided, the prompt will reset to the default 'cwushell>'" << endl;
}

void cpuManual() {
    cout << "- - - - - - - CPUINFO MANUAL - - - - - - -" << endl;
    cout << "PROLOG:" << endl;
    cout << "\tThis manual page is a part of the CWUSHELL manual. Type manual for more information." << endl;
    cout << "NAME:" << endl;
    cout << "\tcpuinfo -- provides information about the processor." << endl;
    cout << "SYNOPSIS:" << endl;
    cout << "\tcpuinfo [-c|-t|-n]" << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "\tThe cpuinfo command provides information about the cpu depending on the given switch:" << endl;
    cout << "\t\t-c: Provides the clock speed of the cpu." << endl << "\t\t-t: Provides the vendor of the cpu." << endl << "\t\t-n: Provides the number of cores in the cpu." << endl;
}

void memManual() {
    cout << "- - - - - - - MEMINFO MANUAL - - - - - - -" << endl;
    cout << "PROLOG:" << endl;
    cout << "\tThis manual page is a part of the CWUSHELL manual. Type manual for more information." << endl;
    cout << "NAME:" << endl;
    cout << "\tmeminfo -- provides information about the systems memory." << endl;
    cout << "SYNOPSIS:" << endl;
    cout << "\tmeminfo [-t|-u|-c]" << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "\tThe meminfo command provides information about the systems memory depending on the given switch:" << endl;
    cout << "\t\t-t: Provides the total RAM of the system in kilobytes." << endl << "\t\t-u: Provides the used RAM in kilobytes." << endl << "\t\t-c: Provides the size of the L2 cache in kilobytes." << endl;
}

void check_path(string cmd) {
    string path = getenv("PATH");
    string* paths;

    cout << path << endl;

    int count = 0;
    for (int i = 0; i < int(path.length()); i++) {
        if (path[i] == ':')
            count++;
    }

    paths = new string[count];

    for (int i = 0; i < count; i++) {
        paths[i] = path.substr(0, path.find(':'));
        path = path.substr(path.find(':') + 1, path.length());
    }
}