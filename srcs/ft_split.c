/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouatt <sobouatt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 00:03:08 by sobouatt          #+#    #+#             */
/*   Updated: 2022/03/10 13:04:24 by sobouatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		wlen(char const *s, char c)
{
	int i;

	i = 0;
	while (s[i] != c && s[i])
		i++;
	return (i);
}

int		wccount(char const *s, char c)
{
	int i;
	int words;

	i = 0;
	words = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == '\0' || s[i + 1] == c))
			words++;
		i++;
	}
	return (words);
}

char	**fill(char const *s, char c, char **str, int words)
{
	int i;
	int j;
	int len;

	i = -1;
	while (++i < words)
	{
		while (*s == c)
			s++;
		len = wlen(s, c);
		if (!(str[i] = malloc(sizeof(char) * len + 1)))
			return (NULL);
		j = 0;
		while (j < len)
			str[i][j++] = *s++;
		str[i][j] = '\0';
	}
	str[i] = NULL;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		words;

	if (!s)
		return (NULL);
	words = wccount(s, c);
	if (!(str = malloc(sizeof(char *) * (words + 1))))
		return (NULL);
	str = fill(s, c, str, words);
	return (str);
}