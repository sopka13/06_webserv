/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 23:34:53 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/19 15:41:57 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

char		*ft_strcpy(char *dest, char *src)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(src))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i++] = '\0';
	while (i < ft_strlen(dest))
		dest[i++] = 0;
	return (dest);
}
