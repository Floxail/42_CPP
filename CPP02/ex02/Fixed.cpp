/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 10:25:13 by flvejux           #+#    #+#             */
/*   Updated: 2026/10/18 16:44:44 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Fixed.hpp"


Fixed::Fixed() : _value(0) {
	std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const int n) {
	std::cout << "Int constructor called" << std::endl;
	this->_value = n << this->_bits;
}

Fixed::Fixed(const float n) {
	std::cout << "Float constructor called" << std::endl;
	this->_value = static_cast<int>(n * (1 << this->_bits) + (n < 0 ? -0.5f : 0.5f));
}

Fixed::Fixed(const Fixed &other) {
	std::cout << "Copy constructor called" << std::endl;
	*this = other;
}

Fixed& Fixed::operator=(const Fixed &other) {
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other)
        this->_value = other.getRawBits();
    return *this;
}

Fixed::~Fixed() {
	std::cout << "Destructor called" << std::endl;
}

int Fixed::getRawBits() const {
	std::cout << "getRawBits member function called" << std::endl;
	return this->_value;
}

void Fixed::setRawBits(int const raw) {
	this->_value = raw;
}

float Fixed::toFloat() const {
	return static_cast<float>(this->_value) / (1 << this->_bits);
}

int Fixed::toInt() const {
	return this->_value >> this->_bits;
}

std::ostream &operator<<(std::ostream &out, const Fixed &fixed) {
	out << fixed.toFloat();
	return out;
}

bool Fixed::operator>(const Fixed &other) const{
	return (this->_value > other._value);
}

bool Fixed::operator<(const Fixed &other) const{
	return (this->_value < other._value);	
}
	
bool Fixed::operator>=(const Fixed &other) const{
	return (this->_value >= other._value);
}
	
bool Fixed::operator<=(const Fixed &other) const{
	return (this->_value <= other._value);	
}
	
bool Fixed::operator==(const Fixed &other) const{
	return (this->_value == other._value);	
}
	
bool Fixed::operator!=(const Fixed &other) const{
	return (this->_value != other._value);	
}

Fixed Fixed::operator+(const Fixed& other) const
{
	Fixed result;
	result.setRawBits(_value + other._value);
	return result;
}

Fixed Fixed::operator-(const Fixed& other) const
{
	Fixed result;
	result.setRawBits(_value - other._value);
	return result;
}

Fixed Fixed::operator*(const Fixed& other) const
{
	Fixed result;
	result.setRawBits((static_cast<long>(_value) * other._value) >> _bits);
	return result;
}

Fixed Fixed::operator/(const Fixed& other) const
{
	Fixed result;
	result.setRawBits((_value << _bits) / other._value);
	return result;
}

Fixed& Fixed::operator++()
{
	_value++;
	return *this;
}

Fixed Fixed::operator++(int)
{
	Fixed temp(*this);
	_value++;
	return temp;
}

Fixed& Fixed::operator--()
{
	_value--;
	return *this;
}

Fixed Fixed::operator--(int)
{
	Fixed temp(*this);
	_value--;
	return temp;
}

Fixed& Fixed::min(Fixed& a, Fixed& b)
{
	return (a < b) ? a : b;
}

const Fixed& Fixed::min(const Fixed& a, const Fixed& b)
{
	return (a < b) ? a : b;
}

Fixed& Fixed::max(Fixed& a, Fixed& b)
{
	return (a > b) ? a : b;
}

const Fixed& Fixed::max(const Fixed& a, const Fixed& b)
{
	return (a > b) ? a : b;
}