/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Phonebook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 15:22:50 by flvejux           #+#    #+#             */
/*   Updated: 2026/09/17 12:20:09 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHONEBOOK_HPP
# define PHONEBOOK_HPP
#include "contact.hpp"

class PhoneBook {
    
    public:
        
    void addContact(void);
    void searchContact(void);
    std::string GetInput(std::string prompt);
    
    PhoneBook(void);
    ~PhoneBook(void);
    private:
    
    Contact _contacts[8];
    int _count;
    int _index;
    std::string truncate(std::string str);
};

#endif
