/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wexitstatus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyunkim <kyunkim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 19:55:57 by kyunkim           #+#    #+#             */
/*   Updated: 2021/11/01 19:55:57 by kyunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wexitstatus(int status)
{
	return ((int)status >> 8 & (0x000000ff));
}
