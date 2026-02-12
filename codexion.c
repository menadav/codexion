/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 11:52:13 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/10 14:50:28 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 9)
		return (-1);
	data = parse(argv);
	if (!data)
		return (-1);
	virtual_data(data);
	return (0);
}
