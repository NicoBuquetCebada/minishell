#include "../include/minishell.h"

static t_exec_info *init_exec_info(void)
{
    t_exec_info *info;

    info = malloc(sizeof(t_exec_info));
    if (!info)
        return NULL;
    info->cmd_count = 0;
    info->cmds = NULL;
    return info;
}

static void count_commands(t_exec_info *info, t_token *tokens)
{
    t_token *current = tokens;
    size_t count = 1;
    while (current)
    {
        if (current->parts && current->parts->type == PIPE)
            count++;
        current = current->next;
    }
    info->cmd_count = count;
}

static int allocate_cmd_info(t_exec_info *info)
{
    info->cmds = malloc(sizeof(t_cmd_info) * info->cmd_count);
    if (!info->cmds)
        return 0;
    return 1;
}

/*static int allocate_cmd_info(t_exec_info *info)
{
    info->cmds = malloc(sizeof(t_cmd_info) * info->cmd_count);
    if (!info->cmds)
        return 0;
    size_t i = 0;
    while (i < info->cmd_count)
    {
        info->cmds[i].argc = 0;
        info->cmds[i].redir_count = 0;
        i++;
    }
    return 1;
}*/

/*static void fill_cmd_info(t_exec_info *info, t_token *tokens)
{
    t_token *current;
    t_tokenpart *part;
    size_t cmd_index;

    cmd_index = 0;
    current = tokens;
    while (cmd_index < info->cmd_count)
    {
        info->cmds[cmd_index].argc = 0;
        info->cmds[cmd_index].redir_count = 0;
        part = current->parts;
        while (part)
        {
            if (part->type == WORD)
                info->cmds[cmd_index].argc++;
            else if (part->type == REDIR_IN || part->type == REDIR_OUT ||
                     part->type == APPEND || part->type == HEREDOC)
                info->cmds[cmd_index].redir_count++;
            part = part->next;
        }
        if (current->parts && current->parts->type == PIPE)
            cmd_index++;
        current = current->next;
    }
}*/

static void fill_cmd_info(t_exec_info *info, t_token *tokens)
{
    t_token *current = tokens;
    size_t cmd_index = 0;

    while (current)
    {
        if (!current->parts)
        {
            current = current->next;
            continue;
        }

        t_tokenpart *part = current->parts;
        while (part)
        {
            if (part->type == WORD)
                info->cmds[cmd_index].argc++;
            else if (part->type == REDIR_IN || part->type == REDIR_OUT ||
                     part->type == APPEND || part->type == HEREDOC)
                info->cmds[cmd_index].redir_count++;
            part = part->next;
        }

        if (current->parts->type == PIPE)
            cmd_index++;
        current = current->next;
    }
}

static t_exec_info *preprocess_tokens(t_token *tokens)
{
    t_exec_info *info;

    info = init_exec_info();
    if (!info)
        return NULL;
    count_commands(info, tokens);
    if (!allocate_cmd_info(info))
    {
        free(info);
        return NULL;
    }
    fill_cmd_info(info, tokens);
    return info;
}

static void free_exec_info(t_exec_info *info)
{
    if (!info)
        return;
    free(info->cmds);
    free(info);
}

static t_exec *allocate_exec(t_exec_info *info)
{
    t_exec *exec;
    size_t i;

    exec = malloc(sizeof(t_exec));
    if (!exec)
        return NULL;

    exec->cmds = malloc(sizeof(t_command) * info->cmd_count);
    if (!exec->cmds)
    {
        free(exec);
        return NULL;
    }

    exec->cmd_c = info->cmd_count;
    i = 0;
    while (i < info->cmd_count)
    {
        exec->cmds[i].argv = malloc(sizeof(char *) * (info->cmds[i].argc + 1));
        exec->cmds[i].ios = malloc(sizeof(t_iospec) * info->cmds[i].redir_count);
        exec->cmds[i].io_c = 0;
        exec->cmds[i].role = HEAD;
        exec->cmds[i].resolved_path = NULL;
        i++;
    }

    return exec;
}


static void process_command_parts(t_command *cmd, t_tokenpart *part, size_t *arg_index, size_t *redir_index)
{
    while (part)
    {
        if (part->type == WORD)
        {
            cmd->argv[*arg_index] = strdup(part->value);
            (*arg_index)++;
        }
        else if (part->type == REDIR_IN || part->type == REDIR_OUT ||
                 part->type == APPEND || part->type == HEREDOC)
        {
            cmd->ios[*redir_index].type = IO_FILE_IN;
            if (part->type == REDIR_OUT)
                cmd->ios[*redir_index].type = IO_FILE_TRUNC;
            else if (part->type == APPEND)
                cmd->ios[*redir_index].type = IO_FILE_APPEND;
            else if (part->type == HEREDOC)
                cmd->ios[*redir_index].type = IO_FILE_HEREDOC;

            cmd->ios[*redir_index].arg = strdup(part->next->value);
            cmd->ios[*redir_index].expand = 1;
            if (part->type == HEREDOC)
                cmd->ios[*redir_index].expand = 0;

            (*redir_index)++;
            part = part->next;
        }
        part = part->next;
    }
}


static void process_tokens_into_exec(t_exec *exec, t_exec_info *info, t_token *tokens)
{
    t_token *current;
    size_t cmd_index;
    size_t arg_index;
    size_t redir_index;

    cmd_index = 0;
    arg_index = 0;
    redir_index = 0;
    current = tokens;

    while (current)
    {
        process_command_parts(&exec->cmds[cmd_index], current->parts, &arg_index, &redir_index);
        exec->cmds[cmd_index].argv[arg_index] = NULL;

        if (current->parts && current->parts->type == PIPE)
        {
            cmd_index++;
            arg_index = 0;
            redir_index = 0;
        }
        current = current->next;
    }
}

static void assign_roles(t_exec *exec)
{
    size_t i;

    i = 0;
    while (i < exec->cmd_c)
    {
        if (i == 0)
            exec->cmds[i].role = HEAD;
        else if (i == exec->cmd_c - 1)
            exec->cmds[i].role = TAIL;
        else
            exec->cmds[i].role = MIDDLE;
        i++;
    }
}

t_exec *fill_exec(t_token *tokens)
{
    t_exec_info *info;
    t_exec *exec;

    info = preprocess_tokens(tokens);
    if (!info)
        return NULL;

    exec = allocate_exec(info);
    if (!exec)
    {
        free_exec_info(info);
        return NULL;
    }

    process_tokens_into_exec(exec, info, tokens);
    assign_roles(exec);
    free_exec_info(info);

    return exec;
}


