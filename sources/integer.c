/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtora <elehtora@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 21:39:27 by elehtora          #+#    #+#             */
/*   Updated: 2022/09/21 22:00:06 by elehtora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Adds a sign for a '+' flag signed conversion.
void	prepend_sign(t_fstring *fs)
{
	char		*sign_prepended_str;

	if (!fs->sign)
	{
		sign_prepended_str = ft_strnew(ft_strlen(fs->string) + 1);
		if (fs->format & F_FORCE_SIGN)
			sign_prepended_str[0] = '+';
		else if (fs->format & F_SPACE_SIGN)
			sign_prepended_str[0] = ' ';
		else if (fs->format & (F_ALT_FORM + C_UOCT))
			sign_prepended_str[0] = '0'; // Octal
		ft_strcpy(sign_prepended_str + 1, fs->string);
		free(fs->string);
		fs->string = sign_prepended_str;
		fs->len += 1;
		/*ft_putnbr(fs->len);*/
	}
}

// Applies a precision greater than the number, i.e. left pads the number with 0's.
// Forced signs come only after this, so we only account for negative sign.
static void	pad_integer_precision(t_fstring *fs)
{
	const size_t	initial_len = ft_strlen(fs->string);
	size_t			corrected_precision;
	char			*str_expanded;
	uint8_t			sign;

	str_expanded = NULL;
	sign = 0;
	if (fs->sign)
		sign = 1;
	corrected_precision = fs->precision + sign;
	if (fs->precision > initial_len - sign)
	{
		if (sign)
			fs->string[0] = '0'; // Swap out minus (precision pad always 0)
		fs->format ^= EXPL_EXPAND_PRECISION;
		str_expanded = ft_strnew(corrected_precision);
		ft_memset(str_expanded, '0', corrected_precision);
		ft_memmove(str_expanded + (corrected_precision - initial_len),\
				fs->string, initial_len);
		free(fs->string);
		fs->string = str_expanded;
		if (sign)
			fs->string[0] = '-'; // Prepend minus
		if (fs->len < corrected_precision)
			fs->len = corrected_precision;
		/*ft_putnbr(fs->len);*/
	}
}

static void	set_explicit_zero(t_fstring *fs)
{
	if (fs->format & EXPL_PRECISION && fs->precision == 0)
		fs->string = ft_strdup("");
	else
	{
		fs->string = ft_strdup("0");
		fs->len = 1;
	}
}

// TODO
int	convert_unsigned_int(t_fstring *fs, unsigned long long int arg)
{
	if (arg == 0)
		set_explicit_zero(fs);
	else if (fs->format & CMASK_HEX)
		fs->string = format_hex(arg, fs);
	else if (fs->format & C_UOCT)
		fs->string = format_oct(arg, fs); // Sets fs->string inside
	else
		fs->string = ft_ltoa_unsigned(arg);
	if (!fs->string)
		return (0);
	fs->len = ft_strlen(fs->string);
	pad_integer_precision(fs);
	if (fs->format & MASK_HEX_PREFIX && !(fs->format & C_UOCT) && arg != 0)
		add_hex_prefix(fs);
	if (fs->field_width > ft_strlen(fs->string))
		expand_to_field_width(fs);
	return (2);
}

int	convert_signed_int(t_fstring *fs, long long int arg)
{
	if (arg == 0)
		set_explicit_zero(fs);
	else
		fs->string = ft_ltoa(arg);
	fs->len = ft_strlen(fs->string);
	/*ft_putnbr(fs->len);*/
	if (arg < 0)
		fs->sign = fs->string; // Pointer to the sign char
	pad_integer_precision(fs);
	if (fs->format & (F_FORCE_SIGN + F_SPACE_SIGN))
		prepend_sign(fs);
	if (fs->field_width > ft_strlen(fs->string))
		expand_to_field_width(fs);
	return (1);
}
