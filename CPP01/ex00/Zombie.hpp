/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:19:56 by flvejux           #+#    #+#             */
/*   Updated: 2026/09/21 13:03:46 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP
#include <string>

class Zombie
{
	public:
		Zombie(std::string name);
		~Zombie();
		void announce();
		
	private:
		std::string name;
};

Zombie* newZombie(std::string name);
void randomChump(std::string name);

#endif