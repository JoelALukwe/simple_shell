#include "custom_shell.h"

void read_command(char *command, size_t size) {
    if (fgets(command, size, stdin) == NULL) {
        if (feof(stdin)) {
            joelprint("\n");
            exit(EXIT_SUCCESS);
        } else {
            joelprint("Error valhalla will have to wait for now\n");
            exit(EXIT_FAILURE);
        }
    }
    command[strcspn(command, "\n")] = '\0';
}
