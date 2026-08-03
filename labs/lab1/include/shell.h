#ifndef SHELL_H
#define SHELL_H

void to_buffer(char buffer[], int size);
void cmd_hello();
void cmd_help();
void cmd_info();
void command_parser(char buffer[]);

#endif