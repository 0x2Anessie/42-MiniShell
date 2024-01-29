/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_flags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:24:40 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 17:24:41 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	out_to_file_flags(void)
{
	return (O_CREAT | O_RDWR | O_TRUNC);
}

int	append_to_file_flags(void)
{
	return (O_CREAT | O_RDWR | O_APPEND);
}

int	heredoc_tmp_file_flags(void)
{
	return (O_CREAT | O_WRONLY | O_TRUNC);
}
