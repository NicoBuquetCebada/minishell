/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/19 20:10:52 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "executor.h"

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;
    char *line;
    t_exec_ctx ctx;

    ctx.envp = envp;
    ctx.last_status = 0;
    ctx.interactive = 1;
    while (1) // tengo que ver cómo gestionar esto
    {
        line = readline("minishell-1.0 $ ");
        add_history(line);       // se recupera solo el historial
        handle_input(line, ctx); // funcion handdle input -> lexer -> tokens -> parser -> llamo funcion de nico
        free(line);
    }
    //clear_history(); // rl_clear_history en Linux
    return 0;
}