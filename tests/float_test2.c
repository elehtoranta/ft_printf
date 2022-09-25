/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_test2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtora <elehtora@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 22:28:45 by elehtora          #+#    #+#             */
/*   Updated: 2022/09/25 01:05:49 by elehtora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>
#define FORMAT "{%.3f}{%.3lf}{%.3Lf}\n"

int	main()
{
	ft_printf(FORMAT, 1.42, 1.42, 1.42l);
	printf(FORMAT, 1.42, 1.42, 1.42l);
}