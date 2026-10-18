/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 05/10/2026 07:18:27 by flvejux           #+#    #+#             */
/*   Updated: 05/10/2026 07:18:27 by flvejux          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>

class Weapon {
private:
    std::string   type;

public:
    Weapon();
    Weapon(const std::string& type);
    const std::string& getType() const;
    void setType(const std::string& type);
};

#endif
