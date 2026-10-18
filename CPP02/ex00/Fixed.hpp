/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 11:09:18 by flvejux           #+#    #+#             */
/*   Updated: 2026/10/18 17:31:37 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
# define FIXED_HPP
#include <ostream>

class Fixed {
	public:
	Fixed();
	Fixed(const Fixed &other);
	Fixed &operator=(const Fixed &other);
	int getRawBits() const;
	void setRawBits(int const raw);
	~Fixed();

	Fixed(const int n);
	Fixed(const float f);

	float toFloat() const;
	int toInt() const;
	
	private:
	int _value;
	static const int _bits;
};

std::ostream &operator<<(std::ostream &out, const Fixed &f);

#endif
