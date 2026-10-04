/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:54:05 by flvejux           #+#    #+#             */
/*   Updated: 2026/09/21 14:32:57 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"
#include <iostream>

int main() {
    std::cout << "newZombie (heap allocation)" << std::endl;
    
    // Create zombie on heap - survives outside function
    Zombie* heapZombie1 = newZombie("Stan");
    Zombie* heapZombie2 = newZombie("Eric");
    
    // faire l'announce
    heapZombie1->announce();
    heapZombie2->announce();
    
    // Must manually delete heap zombies
    delete heapZombie1;
    delete heapZombie2;
    
    std::cout << "\nrandomChump (stack allocation)" << std::endl << std::endl;
    
    // Create zombie on stack - automatically destroyed
    randomChump("Kyle");
    randomChump("Kenny");
    
    std::cout << "\nDifference" << std::endl << std::endl;
    std::cout << "Creating heap zombie..." << std::endl;
    Zombie* survivor = newZombie("Survivor");
    
    std::cout << "Creating stack zombie..." << std::endl;
    randomChump("TemporaryZombie");  // Dies immediately
    
    std::cout << "Heap zombie still alive:" << std::endl;
    survivor->announce();  // Alive !
    std::cout << "Cleaning up heap zombie..." << std::endl;
    delete survivor;
    
    std::cout << "\nProgram ending..." << std::endl;
    return 0;
}