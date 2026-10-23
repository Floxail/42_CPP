/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 15:57:15 by flvejux           #+#    #+#             */
/*   Updated: 2026/10/23 15:57:16 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"
#include "ScavTrap.hpp"
#include "FragTrap.hpp"

int main() {
	std::cout << "=== Creating ClapTrap ===" << std::endl;
	ClapTrap clap("Clappy");

	std::cout << "\n=== Creating ScavTrap ===" << std::endl;
	ScavTrap scav("Scavvy");

	std::cout << "\n=== Creating FragTrap ===" << std::endl;
	FragTrap frag("Fraggy");

	std::cout << "\n=== FragTrap stats ===" << std::endl;
	std::cout << "Name: " << frag.getName() << std::endl;
	std::cout << "HP: " << frag.getHitPoints() << std::endl;
	std::cout << "Energy: " << frag.getEnergyPoints() << std::endl;
	std::cout << "Attack Damage: " << frag.getAttackDamage() << std::endl;

	std::cout << "\n=== Testing attacks ===" << std::endl;
	clap.attack("Target");
	scav.attack("Target");
	frag.attack("Target");

	std::cout << "\n=== Testing special abilities ===" << std::endl;
	scav.guardGate();
	frag.highFivesGuys();

	std::cout << "\n=== Testing damage and repair ===" << std::endl;
	frag.takeDamage(50);
	frag.beRepaired(20);

	std::cout << "\n=== Destruction (reverse order) ===" << std::endl;
	return 0;
}
