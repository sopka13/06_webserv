/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 23:34:26 by eyohn             #+#    #+#             */
/*   Updated: 2021/08/24 08:41:20 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

void		*ft_memset(void *s, int c, size_t n)
{
	while (n-- > 0)
		*((unsigned char *)s + n) = (unsigned char)c;
	return (s);
}
