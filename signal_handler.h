#ifndef SIGNAL_HANDLER_H

# define SIGNAL_HANDLER_H
# include <signal.h>
# include "utilities.h"
# include "parser.h"

extern sig_atomic_t g_signal;

void    handle_signal(int signo);
void    sig_init();
void    second_hand(int signo);
void    piping_sign(int status[2], t_ast_node *pipe_node);

#endif