/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Phonebook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 07:29:27 by flvejux           #+#    #+#             */
/*   Updated: 2026/10/05 07:29:27 by flvejux          ###   ########.ch       */
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
