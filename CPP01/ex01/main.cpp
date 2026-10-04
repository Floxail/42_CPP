/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 14:29:52 by flvejux           #+#    #+#             */
/*   Updated: 2026/09/22 15:29:41 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"
#include <iostream>

int main() {
	std::cout << "¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯" << std::endl;
	std::cout << "-------HORDE TESTING---------" << std::endl;
	std::cout << "_____________________________" << std::endl;

	int n;

	std::cout << "How many Zombies in the horde ? ";
	std::cin >> n;
	std::string name = "Randy";
	
	std::cout << "Creating horde of " << n << " zombies named '" << name << "'" << std::endl;
	Zombie* horde = zombieHorde(n, name);

	if (!horde) {
		std::cout << "No zombies were created." << std::endl;
		return 0;
	}

	for (int i = 0; i < n; i++)
	{
		std::cout << "Zombie " << i + 1 << ": ";
		horde[i].announce();
	}
		
	std::cout << "\nDeleting the horde..." << std::endl;
	delete[] horde;

	std::cout << "\nend of test…" << std::endl;
	return 0;
}
