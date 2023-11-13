#include "custom_shell.h"

void joelprint(const char *enter_valhalla) {
    write(STDOUT_FILENO, enter_valhalla, strlen(enter_valhalla));
}

