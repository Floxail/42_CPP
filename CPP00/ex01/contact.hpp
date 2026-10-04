/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contact.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 17:18:13 by flvejux           #+#    #+#             */
/*   Updated: 2026/09/17 12:20:11 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTACT_HPP
# define CONTACT_HPP
#include <string>

class Contact
{
public:
    void setfName(std::string fname);
    void setlName(std::string lname);
    void setPhone(std::string phone);
    void setNick(std::string nick);
    void setSecret(std::string secret);

    std::string getfName(void) const;
    std::string getlName(void) const;
    std::string getPhone(void) const;
    std::string getNick(void) const;
    std::string getSecret(void) const;

    bool isEmpty();

        Contact(void);
        ~Contact(void);
private:
    std::string _fname;
    std::string _lname;
    std::string _nick;
    std::string _phone;
    std::string _secret;
};

#endif