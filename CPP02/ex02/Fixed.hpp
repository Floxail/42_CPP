/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 13:16:40 by flvejux           #+#    #+#             */
/*   Updated: 2026/10/18 16:29:05 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
# define FIXED_HPP

# include <ostream>

class Fixed {
	public:
	Fixed();
	Fixed(const int n);
	Fixed(const float n);
	Fixed(const Fixed &other);
	Fixed &operator=(const Fixed &other);
	
	int getRawBits() const;
	void setRawBits(int const raw);
	float toFloat() const;
	int toInt() const;
	
	bool operator>(const Fixed &other) const;
	bool operator<(const Fixed &other) const;
	bool operator>=(const Fixed &other) const;
	bool operator<=(const Fixed &other) const;
	bool operator==(const Fixed &other) const;
	bool operator!=(const Fixed &other) const;

	Fixed operator+(const Fixed &other) const;
	Fixed operator-(const Fixed &other) const;
	Fixed operator*(const Fixed &other) const;
	Fixed operator/(const Fixed &other) const;
	
	Fixed &operator++();
	Fixed	operator++(int);
	Fixed &operator--();
	Fixed	operator--(int);
	
	static Fixed &min(Fixed& a, Fixed& b);
	static const Fixed &min(const Fixed& a, const Fixed& b);
	static Fixed &max(Fixed& a, Fixed& b);
	static const Fixed &max(const Fixed& a, const Fixed& b);

	~Fixed();

	private:
	int _value;
	static const int _bits = 8;

};

std::ostream &operator<<(std::ostream &out, const Fixed &fixed);

#endif
