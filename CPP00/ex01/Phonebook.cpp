/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Phonebook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 09:47:06 by flvejux           #+#    #+#             */
/*   Updated: 2026/09/17 12:19:59 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "contact.hpp"
#include <iostream>
#include <iterator>
#include <string>
#include <iomanip>
#include <cctype>
#include "Phonebook.hpp"

PhoneBook::PhoneBook(void)
{
    this->_count = 0;
    this->_index = 0;
}

std::string PhoneBook::GetInput(std::string prompt)
{
    std::string input;
    
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, input)) {
            return "";
        }
        
        if (!input.empty()) {
            return input;
        }
        
        std::cout << "Field cannot be empty! Please try again." << std::endl;
    }
}

void PhoneBook::addContact()
{
    Contact newContact;
    
    std::string fname = GetInput("Enter first name: ");
    std::string lname = GetInput("Enter last name: ");
    std::string nick = GetInput("Enter nickname: ");
    std::string phone = GetInput("Enter phone number: ");
    std::string secret = GetInput("Enter darkest secret: ");

    newContact.setfName(fname);
    newContact.setlName(lname);
    newContact.setNick(nick);
    newContact.setPhone(phone);
    newContact.setSecret(secret);

    _contacts[_index] = newContact;

    if(_count < 8)
        _count++;
    
    _index = (_index + 1) % 8;
    std::cout << "Contact added" << std::endl;
}

std::string PhoneBook::truncate(std::string str)
{
    if (str.length() > 10)
        return (str.substr(0, 9) + ".");
    return (str);
}

void PhoneBook::searchContact()
{
    if (_count == 0)
    {
        std::cout << "Phone book is empty" << std::endl;
        return;
    }

    std::cout << std::setw(10) << "index" << "|"
              << std::setw(10) << "first name" << "|"
              << std::setw(10) << "last name" << "|"
              << std::setw(10) << "nickname" << std::endl;

    for (int i = 0; i < _count; i++)
    {
        std::cout << std::setw(10) << i + 1 << "|"
                  << std::setw(10) << truncate(_contacts[i].getfName()) << "|"
                  << std::setw(10) << truncate(_contacts[i].getlName()) << "|"
                  << std::setw(10) << truncate(_contacts[i].getNick()) << std::endl;
    }

    std::string input = GetInput("Enter index: ");

    if (input.length() != 1 || !isdigit(input[0]) || input[0] == '0')
    {
        std::cout << "Invalid index" << std::endl;
        return;
    }

    int index = input[0] - '0';

    if (index < 1 || index > _count)
    {
        std::cout << "Invalid index" << std::endl;
        return;
    }

    Contact contact = _contacts[index - 1];

    std::cout << "First name: " << contact.getfName() << std::endl;
    std::cout << "Last name: " << contact.getlName() << std::endl;
    std::cout << "Nickname: " << contact.getNick() << std::endl;
    std::cout << "Phone number: " << contact.getPhone() << std::endl;
    std::cout << "Darkest secret: " << contact.getSecret() << std::endl;
}

PhoneBook::~PhoneBook(void)
{
}