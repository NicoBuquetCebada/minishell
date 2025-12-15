/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/06 23:14:29 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(int argc, char *argv[], char *envp[])
{
    char *line;
    t_exec_ctx ctx;
    int i;

    ctx.envp = envp;
    ctx.last_status = 0;
    ctx.interactive = 1;
    i = 0;
    while (i < 5) // tengo que ver cómo gestionar esto
    {
        line = readline("minishell-1.0 $ ");
        add_history(line);       // se recupera solo el historial
        handle_input(line, ctx); // funcion handdle input -> lexer -> tokens -> parser -> llamo funcion de nico
        free(line);
        i++;
    }
    clear_history(); // rl_clear_history en Linux
    return 0;
}