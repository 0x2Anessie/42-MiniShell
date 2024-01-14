#include "../../include/minishell.h"

/**
 * @brief Vérifie si un chemin direct spécifié pour une commande est invalide.
 *
 * @param cmd Le chemin de la commande à vérifier.
 * @return true Si le chemin direct est invalide (non exécutable et commence
 * par un '/'), sinon false.
 */
bool	is_invalid_direct_path(const char *cmd)
{
    return (access(cmd, X_OK) && *cmd == '/');
}

bool	is_lexer_token_cmd_arg(t_lexer *lexer_item)
{
    return (lexer_item != NULL && lexer_item->token == ARG);
}

/**
 * @brief Vérifie si l'itération dans la liste des lexèmes doit continuer.
 *
 * @param lexer_item L'élément actuel du lexer à vérifier.
 * @param current_index L'indice courant dans le processus d'itération.
 * @param total_args Nombre total d'arguments attendus.
 * @return true si l'itération doit continuer, sinon false.
 */
bool	should_continue_lexer_iteration(\
t_lexer *lexer_item, int current_index, int total_args)
{
    return (lexer_item->next != NULL && current_index < total_args);
}