#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>

void print_prompt();
bool checkExit();

int process_cmd(std::string);
int execute_cmd(std::string[]);

void get_clocksp();
void get_vendor();
void get_cores();

void get_memory();
void get_memory_used();
void get_cache();

void manual();
void exitManual();
void promptManual();
void cpuManual();
void memManual();

void check_path(std::string);

#endif