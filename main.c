#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

Display *dpy;

/**
 * @param[out] l_code A language code of current keyboard layout.
 * @param[out] l_pos A layout position in XKB group list.
 * @param[out] t_layout Total number of layouts in XKB group list.
 * @return "true" if executed successfully, "false" if an
 * error occurred.
 */
bool get_status(char **l_code, int *l_pos, int *t_layout) {
    XkbStateRec state;
    XkbGetState(dpy, XkbUseCoreKbd, &state);
    XkbRF_VarDefsRec rules;
    if (!XkbRF_GetNamesProp(dpy, NULL, &rules)) {
        fprintf(stderr, "ERR: Cannot get XKB-rules.\n");
        return false;
    }

    char *layout = strtok(rules.layout, ",");
    char *cur_layout = NULL;
    int layout_size = 0;
    while (layout != NULL) {
        if (layout_size == state.group) {
            cur_layout = layout;
        }

        ++layout_size;

        layout = strtok(NULL, ",");
    }
    if (cur_layout == NULL) {
        fprintf(stderr, "ERR: Undefined current layout.\n");
        return false;
    }

    if (l_code != NULL) {
        *l_code = cur_layout;
    }
    if (l_pos != NULL) {
        *l_pos = (int)state.group;
    }
    if (t_layout != NULL) {
        *t_layout = layout_size;
    }

    return true;
}

/**
 * @return "true" if executed successfully, "false" if an
 * error occurred.
 */
bool switch_layout(void) {
    int l_pos = 0;
    int t_layout = 0;
    if (!get_status(NULL, &l_pos, &t_layout)) {
        return false;
    }

    int next_layout = l_pos + 1 >= t_layout ? 0 : l_pos + 1;
    if (!XkbLockGroup(dpy, XkbUseCoreKbd, next_layout)) {
        fprintf(stderr, "ERR: Cannot switch keyboard layout.\n");
        return false;
    }

    return true;
}

void show_help(void) {
    printf("xkbklu - The helper for xkb* utilities.\n");
    printf("\n");
    printf("Commands:\n");
    printf("switch  -   Switch layout of keyboard.\n");
    printf("status  -   Return layout code of keyboard.\n");
}

int main(int argc, char *argv[]) {
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        fprintf(stderr, "ERR: Cannot open display.\n");
        return EXIT_FAILURE;
    }

    int exit_code = EXIT_FAILURE;
    char *command = argc > 1 ? argv[1] : "";

    if (strcmp(command, "status") == 0) {
        char *lang;
        if (get_status(&lang, NULL, NULL)) {
            printf("%s\n", lang);
            exit_code = EXIT_SUCCESS;
        }
    } else if (strcmp(command, "switch") == 0) {
        if (switch_layout()) {
            exit_code = EXIT_SUCCESS;
        }
    } else if (
        strcmp(command, "") == 0
        || strcmp(command, "--help") == 0
        || strcmp(command, "-h") == 0
    ) {
        show_help();
        exit_code = EXIT_SUCCESS;
    } else {
        fprintf(stderr, "ERR: Command is not found.\n");
    }

    XCloseDisplay(dpy);

    return exit_code;
}
