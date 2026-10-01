#include <stdio.h>
#include <readline/history.h>
#include "history.h"

void print_history(void)
{
    HIST_ENTRY **list = history_list();

    if (list == NULL)
        return;

    for (int i = 0; list[i] != NULL; i++)
        printf("%d  %s\n", i + 1, list[i]->line);
}
