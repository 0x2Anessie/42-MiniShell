
#include "../../include/minishell.h"

void	print_export(t_export *export_lst)
{
	t_export	*current;

	current = export_lst;
	while (current != NULL)
	{
		if (g_all.utils->node->out > 0)
		{
			ft_write_fd("declare -x ", g_all.utils->node->out);
			ft_write_fd(current->value, g_all.utils->node->out);
			ft_write_fd("\n", g_all.utils->node->out);
		}
		else if (!g_all.utils->node->out_fail)
		{
			printf("declare -x ");
			printf("%s\n", current->value);
		}
		current = current->next;
	}
	current = export_lst;
	g_all.utils->err = 0;
}

int	export_parsing_66(t_lexer *tmp)
{
	tmp = g_all.utils->head_lexer_lst;
	while (tmp)
	{
		if (check_parsing_export(tmp->word) == 1)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	process_word_and_add_export(t_lexer *tmp, t_data *data)
{
	process_word(&(g_all.utils), tmp, data);
	if (verif_var_exist_export(g_all.utils, tmp->word, data) == 0
		&& verif_equal(tmp->word, '=') == 0
		&& verif_var_exist_export_2(g_all.utils, tmp->word) == 0)
		lst_add_back_export(&(g_all.utils->export_lst), tmp->word, data);
	else if (check_case(tmp->word)
		&& verif_var_exist_export(g_all.utils, tmp->word, data) == 0)
	{
		tmp->word = case_egale(tmp->word, data);
		if (verif_var_exist_export(g_all.utils, tmp->word, data) == 0)
			lst_add_back_export(&(g_all.utils->export_lst), tmp->word, data);
	}
	else if (verif_var_exist_export(g_all.utils, tmp->word, data) == 0)
	{
		if (verif_var_exist_export(g_all.utils, tmp->word, data) == 0
			&& verif_equal(tmp->word, '=') == 1)
			lst_add_back_export(&(g_all.utils->export_lst), tmp->word, data);
		else if (verif_var_exist_export(g_all.utils, tmp->word, data) == 1)
			lst_add_back_export(&(g_all.utils->export_lst), tmp->word, data);
	}
}

void	export_remaining(t_lexer *tmp, t_data *data)
{
	if (export_parsing_66(tmp))
		return ;
	tmp = g_all.utils->head_lexer_lst;
	while (tmp)
	{
		if (tmp->token == ARG)
			process_word_and_add_export(tmp, data);
		tmp = tmp->next;
	}
}

int	export_things(t_lexer *lexer_lst, t_data *data)
{
	t_lexer	*tmp;

	sort_export_lst(&(g_all.utils->export_lst));
	tmp = lexer_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->word, "export") && !(tmp->next && tmp->next->word
				&& !(tmp->next->word[0] == '\0')))
			print_export(g_all.utils->export_lst);
		else if (!ft_strcmp(tmp->word, "export") && tmp->next
			&& tmp->next->token != ARG)
			print_export(g_all.utils->export_lst);
		else if ((ft_strcmp(tmp->word, "export") == 0) && (tmp->next
				&& tmp->next->token == ARG))
		{
			tmp = tmp->next;
			export_remaining(tmp, data);
			break ;
		}
		tmp = tmp->next;
	}
	return (0);
}
