/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 19:10:26 by flvejux           #+#    #+#             */
/*   Updated: 2026/09/17 12:20:14 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "contact.hpp"

Contact::Contact(void)
{
}

std::string Contact::getfName(void) const
{
    return (this->_fname);
}

std::string Contact::getlName(void) const
{
    return (this->_lname);
}

std::string Contact::getPhone(void) const
{
    return (this->_phone);
}

std::string Contact::getNick(void) const
{
    return (this->_nick);
}

std::string Contact::getSecret(void) const
{
    return (this->_secret);
}

void Contact::setfName(std::string fname)
    {
        this->_fname = fname;
    }

void Contact::setlName(std::string lname)
    {
        this->_lname = lname;
    }

void Contact::setPhone(std::string phone)
    {
        this->_phone = phone;
    }

void Contact::setNick(std::string nick)
    {
        this->_nick = nick;
    }
    
void Contact::setSecret(std::string secret)
    {
        this->_secret = secret;
    }

bool Contact::isEmpty()
    {
        return (_fname.empty() && _lname.empty() &&
        _nick.empty() && _phone.empty() &&
        _secret.empty());
    }

    Contact::~Contact(void)
{
}