#include "../../include/minishell.h"

/**
 * @function: reset_quote_states
 * @brief: Réinitialise les états des guillemets simples et doubles.
 *
 * @param st: Pointeur sur une structure t_quote représentant l'état actuel
 *            des guillemets.
 *
 * @description:
 * Cette fonction réinitialise les états des guillemets simples
 * (singl_quot_status) et doubles (doubl_quot_status) à ZERO_INIT, ce qui est
 * nécessaire pour traiter correctement les lexèmes dans différentes parties
 * du code.
 */
void	reset_quote_states(t_quote *st)
{
	st->singl_quot_status = ZERO_INIT;
	st->doubl_quot_status = ZERO_INIT;
}

/**
 * @function: initialize_expansion_parameters
 * @brief: Initialise les paramètres nécessaires pour l'expansion des lexèmes.
 *
 * @param exp: Pointeur sur une structure t_expand contenant les détails de
 *             l'expansion.
 * @param data: Pointeur sur une structure t_data contenant des informations
 *              diverses nécessaires pour l'opération.
 * @param expnd: Pointeur double vers le nœud de lexème à traiter pour
 *               l'expansion.
 * @param st: Pointeur sur une structure t_quote représentant l'état actuel
 *            des guillemets.
 *
 * @description:
 * Cette fonction initialise les valeurs de 'exp->length_of_expanded_var_value' 
 * et 'data->utils->g_signal_in_char_format', et alloue la mémoire nécessaire 
 * pour 'exp->value_of_expanded_var_from_env' en utilisant la fonction 
 * 'allocate_memory_for_expanded_word'.
 */
void	initialize_expansion_parameters(\
t_expand *exp, t_data *data, t_lexer **expnd, t_quote *st)
{
	exp->length_of_expanded_var_value = ZERO_INIT;
	data->utils->g_signal_in_char_format = \
	convert_int_to_string_with_tracking(data, g_globi);
	exp->value_of_expanded_var_from_env = \
	allocate_memory_for_expanded_word(expnd, st, data->\
	full_env_var_copy_gamma, data);
}
