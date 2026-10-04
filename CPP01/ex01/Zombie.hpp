/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 14:06:14 by flvejux           #+#    #+#             */
/*   Updated: 2026/09/22 14:06:14 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

#include <string>

class Zombie
{
	public:
		Zombie();
		Zombie(std::string name);
		~Zombie();

		void announce();
		void setName(std::string name);

	private:
		std::string name;
};

Zombie* zombieHorde( int N, std::string name );

#endif