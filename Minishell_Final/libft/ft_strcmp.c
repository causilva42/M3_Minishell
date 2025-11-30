/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:31:01 by causilva          #+#    #+#             */
/*   Updated: 2025/11/20 15:00:25 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (1)
	{
		if (s1[i] == '\0' && s2[i] == '\0')
			return (0);
		if (((unsigned char *) s1)[i] != ((unsigned char *) s2)[i])
			return (((unsigned char *) s1)[i] - ((unsigned char *) s2)[i]);
		i++;
	}
	return (0);
}
