#ifndef CLIENTS_LOADING_H
#define CLIENTS_LOADING_H


struct list;


void load_clients(const int argc, char *argv[], struct list *players, struct list *libs);

void free_clients(struct list *players, struct list *libs);

void error(const char *message);

#endif // CLIENTS_LOADING_H
