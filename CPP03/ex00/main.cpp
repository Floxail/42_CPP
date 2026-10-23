/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 21:23:39 by flvejux           #+#    #+#             */
/*   Updated: 2026/10/20 21:24:27 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

int main() {
	std::cout << "=== Creating ClapTraps ===" << std::endl;
	ClapTrap a("Randy");
	ClapTrap b("Stan");
	ClapTrap c(a);
	ClapTrap d;
	d = b;

	std::cout << "\n=== Testing attack ===" << std::endl;
	a.attack("Cartman");
	b.attack("DR. CHAOS");

	std::cout << "\n=== Testing takeDamage ===" << std::endl;
	a.takeDamage(3);
	a.takeDamage(5);

	std::cout << "\n=== Testing beRepaired ===" << std::endl;
	a.beRepaired(2);

	std::cout << "\n=== Testing energy depletion ===" << std::endl;
	for (int i = 0; i < 12; i++) {
		b.attack("Target");
	}

	std::cout << "\n=== Testing death ===" << std::endl;
	ClapTrap dead("Doomed");
	dead.takeDamage(100);
	dead.attack("Nobody");
	dead.beRepaired(10);

	std::cout << "\n=== Destruction ===" << std::endl;
	return 0;
}