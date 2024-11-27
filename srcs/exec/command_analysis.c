#include "../../include/minishell.h"

int	is_cmd_match(\
t_lexer *lexer_lst, const char *command, int command_length)
{
	return (!ft_strncmp(lexer_lst->cmd_segment, command, command_length) \
	&& strlen2(lexer_lst->cmd_segment) == command_length);
}

int	is_built_in_command(t_lexer *lexer_lst)
{
	if (lexer_lst->cmd_segment == NULL)
		return (0);
	return (is_cmd_match(\
	lexer_lst, CMD_PRINT_DIRCT, ft_strlen(CMD_PRINT_DIRCT)) \
	|| is_cmd_match(\
	lexer_lst, CMD_ECHO, ft_strlen(CMD_ECHO)) \
	|| is_cmd_match(\
	lexer_lst, CMD_ENV_VARS, ft_strlen(CMD_ENV_VARS)) \
	|| is_cmd_match(\
	lexer_lst, CMD_UNSET_VARS, ft_strlen(CMD_UNSET_VARS)) \
	|| is_cmd_match(\
	lexer_lst, CMD_CHANG_DIRCT, ft_strlen(CMD_CHANG_DIRCT)) \
	|| is_cmd_match(\
	lexer_lst, CMD_EXPORT_VARS, ft_strlen(CMD_EXPORT_VARS)) \
	|| is_cmd_match(\
	lexer_lst, CMD_EXIT_SHELL, ft_strlen(CMD_EXIT_SHELL)));
}

int	should_continue_execution(t_data *data, int *y)
{
	return (data->utils->node \
	&& data->utils->node->is_command_present == 1 \
	&& ++y[1] >= 0 \
	&& data->utils->heredoc_ctrl_c_uninterrupted);
}

int	check_redirection_validity_in_node(t_node *node)
{
	return (node->input_fd != INPUT_FD_REDIRECTION_FAIL \
	&& node->output_redirection_error_id != \
	OUTPUT_ABSENCE_OF_TARGET_ERROR_CODE);
}

t_lexer	*find_next_command_in_lexer(t_lexer *lexer_list)
{
	while (lexer_list && lexer_list->token != CMD)
		lexer_list = lexer_list->next;
	return (lexer_list);
}
