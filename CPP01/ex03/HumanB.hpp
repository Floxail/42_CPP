/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 09:44:59 by flvejux           #+#    #+#             */
/*   Updated: 2026/10/12 09:44:59 by flvejux          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANB_HPP
#define HUMANB_HPP

#include <string>
#include "Weapon.hpp"

class HumanB {
private:
    std::string name;
    Weapon* weapon;
public:
    HumanB(const std::string& name);
    void setWeapon(Weapon& weapon);
    void dropWeapon();
    void attack() const;
};

#endif
