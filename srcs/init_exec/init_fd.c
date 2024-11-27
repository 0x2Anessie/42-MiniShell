#include "../../include/minishell.h"

void	setup_input_redirection(t_node *node, t_lexer *lexer_lst, t_data *data)
{
	node->input_fd = INPUT_FD_NOT_SET;
	node->is_input_redirection_failed = ZERO_INIT;
	while (is_current_token_not_pipe(lexer_lst))
	{
		if (is_input_redirection_followed_by_token_fd(lexer_lst))
		{
			if (is_input_fd_open_ready_for_read(node))
				close(node->input_fd);
			if (is_next_word_missing(lexer_lst))
				node->is_input_redirection_failed = TRUE;
			if (is_next_word_existing_and_readable(lexer_lst))
				node->input_fd = open(lexer_lst->next->cmd_segment, O_RDONLY);
			else
				handle_redirect_input_error(node, lexer_lst);
		}
		configure_here_doc_input(node, lexer_lst, data);
		lexer_lst = lexer_lst->next;
	}
}

void	append_output_redirection(\
t_node *node, t_lexer *lex_lst, int *is_output_redirection_feasible)
{
	if (is_append_out_followed_by_fd_token(lex_lst))
	{
		if (is_output_fd_open_for_closing(node))
			close (node->output_fd);
		if (is_next_lexeme_word_existing(lex_lst))
			node->output_fd = open(\
			lex_lst->next->cmd_segment, append_to_file_flags(), \
			PERM_O_RW_G_R_OT_R);
		else
		{
			ft_write_fd(ERR_AMB_REDIRECT, STDERR_FILENO);
			node->output_redirection_error_id = \
			OUTPUT_ABSENCE_OF_TARGET_ERROR_CODE;
		}
		if (is_output_append_redirection_error_detected(node, lex_lst))
			node->output_redirection_error_id = \
			OUTPUT_TARGET_ACCESS_ERROR_CODE;
		*is_output_redirection_feasible = TRUE;
	}
}

bool	is_next_command_segment_present(t_lexer *lex_lst)
{
	return (lex_lst->next && lex_lst->next->cmd_segment);
}

void	normal_output_redirection(t_node *node, t_lexer *lex_lst)
{
	if (is_redirect_out_followed_by_fd_token(lex_lst))
	{
		if (is_output_fd_open_for_closing(node))
			close (node->output_fd);
		if (is_next_command_segment_present(lex_lst))
			node->output_fd = open(\
			lex_lst->next->cmd_segment, out_to_file_flags(), \
			PERM_O_RW_G_R_OT_R);
		else
		{
			ft_write_fd(ERR_AMB_REDIRECT, STDERR_FILENO);
			node->output_redirection_error_id = \
			OUTPUT_ABSENCE_OF_TARGET_ERROR_CODE;
		}
		if (is_normal_output_redirection_error_detected(node, lex_lst))
			node->output_redirection_error_id = \
			OUTPUT_TARGET_ACCESS_ERROR_CODE;
		node->is_output_redirection_feasible = TRUE;
	}
}

void	setup_output_redirection(t_node *node, t_lexer *lex_lst)
{
	node->is_output_redirection_feasible = ZERO_INIT;
	node->output_fd = OUTPUT_FD_NOT_CONFIGURED;
	node->output_redirection_error_id = 0;
	while (is_current_token_not_pipe(lex_lst))
	{
		normal_output_redirection(node, lex_lst);
		append_output_redirection(\
		node, lex_lst, &node->is_output_redirection_feasible);
		if (is_output_redirection_error_detected(node))
		{
			node->is_output_redirection_feasible = FALSE;
			perror(OUT_FILE);
		}
		lex_lst = lex_lst->next;
	}
}
