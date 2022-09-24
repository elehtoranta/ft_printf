/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtora <elehtora@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 11:10:19 by elehtora          #+#    #+#             */
/*   Updated: 2022/09/24 16:39:42 by elehtora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "dispatch.h"

static void	init_fstring(t_fstring *fs)
{
	fs->format = 0x0;
	fs->field_width = 0;
	fs->precision = 0;
	fs->string = NULL;
	fs->sign = NULL;
	fs->len = 0;
}

// Gets the formatting information for the next argument, read from the format
// string.
// Most of the format setting functions return a pointer to the format string
// position where that specific modification has been cleared.
static void	get_next_format(t_fstring *fs, const char *init)
{
	const char	*type = ft_strpbrk(init + 1, SPEC_TYPES);
	const char	*delimiter = init + ft_strspn(init, ALL_FCHARS);

	if (!type || type > delimiter) //fallback;
	{
		fs->type = (char *)delimiter;
		return ;
	}
	if (ft_memchr(init, '*', type - init)) // Ain't handing that
	{
		fs->format |= FORMAT_ERROR;
		return ;
	}
	fs->format |= set_type(type);
	fs->type = (char *)type;
	delimiter = set_length_modifier(init, type, fs);
	delimiter = set_precision(init, delimiter, fs);
	delimiter = set_field_width(init, delimiter, fs);
	set_flags(init, delimiter, fs);
}

static void	teardown(t_fstring *fs)
{
	if (fs->string)
		ft_strdel(&fs->string);
}

// Applies the previously gathered format information to the parameter.
static int			convert_fstring(t_fstring *fs, va_list *ap)
{
	return (g_convert[(fs->format & CMASK) - 1](fs, ap)); // TODO requires header convspec formatting
}

int					ft_printf(const char *format, ...)
{
	va_list		ap;
	char		*initializer;
	size_t		printed;
	t_fstring	fs;

	va_start(ap, format);
	printed = 0;
	while (1)
	{
		init_fstring(&fs);
		initializer = ft_strchr(format, '%');
		if (!initializer)
		{
			printed += write(1, format, ft_strlen(format));
			break ;
		}
		else if (*(initializer + 1) == '\0')
			break ;
		printed += write(1, format, initializer - format);
		get_next_format(&fs, initializer);
		if (fs.format == FORMAT_ERROR)
		{
			format = initializer + 1;
			continue ;
		}
		if (!convert_fstring(&fs, &ap)) // TODO Collapse convert_fstring straight as convert[]()
			return (-1);
		printed += write(1, fs.string, fs.len);
		format = fs.type + 1;
	}
	teardown(&fs);
	va_end(ap);
	return ((int)printed);
}
