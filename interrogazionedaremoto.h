#ifndef INTERROGAZIONEDAREMOTO
#define INTERROGAZIONEDAREMOTO

void new_connection (int);
void *get_in_addr(struct sockaddr *);
int make_accept_sock (const char* );
void accept_loop (const char*);

#endif
