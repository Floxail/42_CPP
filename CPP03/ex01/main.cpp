/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 21:23:39 by flvejux           #+#    #+#             */
/*   Updated: 2026/10/23 15:49:04 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScavTrap.hpp"

int main() {
	std::cout << "=== Creating ScavTrap ===" << std::endl;
	ScavTrap scav("Scavvy");

	std::cout << "\n=== ScavTrap stats ===" << std::endl;
	std::cout << "Name: " << scav.getName() << std::endl;
	std::cout << "HP: " << scav.getHitPoints() << std::endl;
	std::cout << "Energy: " << scav.getEnergyPoints() << std::endl;
	std::cout << "Attack Damage: " << scav.getAttackDamage() << std::endl;

	std::cout << "\n=== Testing ScavTrap attack ===" << std::endl;
	scav.attack("Enemy");

	std::cout << "\n=== Testing guardGate ===" << std::endl;
	scav.guardGate();

	std::cout << "\n=== Testing inherited functions ===" << std::endl;
	scav.takeDamage(30);
	scav.beRepaired(10);

	std::cout << "\n=== Testing copy constructor ===" << std::endl;
	ScavTrap scav2(scav);
	std::cout << "scav2 HP: " << scav2.getHitPoints() << std::endl;

	std::cout << "\n=== Testing ClapTrap vs ScavTrap ===" << std::endl;
	ClapTrap clap("Clappy");
	clap.attack("Target");
	scav.attack("Target");

	std::cout << "\n=== Destruction (reverse order) ===" << std::endl;
	return 0;
}