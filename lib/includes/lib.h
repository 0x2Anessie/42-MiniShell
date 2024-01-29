/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acatusse <acatusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:24:32 by acatusse          #+#    #+#             */
/*   Updated: 2024/01/22 16:24:33 by acatusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_H
# define LIB_H

# include <fcntl.h>
# include "ft_printf.h"
# include "get_next_line.h"
# include <stdlib.h>

char		**ft_split(char const *s, char c);
char		*ft_strjoin2(char *s1, char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		ft_putstr_fd(char *s1, char *s2, int fd);
int			strlen2(char *s);
char		*ft_strchr(char *str, int c);
int			length_of_number_for_string_conversion(long n);
char		*ft_is_neg(char *res, long n, int i);
char		*ft_itoa(int n);
long long	ft_atoll(const char *str);
char		*ft_strncpy(char *dest, const char *src, size_t n);
int			ft_isalnum(int c);

#endif
