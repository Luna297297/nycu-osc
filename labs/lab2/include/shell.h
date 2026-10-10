#ifndef SHELL_H
#define SHELL_H

enum shell_action {
    SHELL_ACTION_CONTINUE,
    SHELL_ACTION_LOAD,
};

void to_buffer(char buffer[], int size);
void cmd_hello(void);
void cmd_help(void);
void cmd_info(void);
enum shell_action command_parser(char buffer[],
                    const void *initrd_start,
                    const void *initrd_end);

#endif