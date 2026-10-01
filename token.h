#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
    char **items;
    int count;
} token_list_t;

void token_free(token_list_t *tokens);
void token_print(token_list_t *tokens);

#endif
