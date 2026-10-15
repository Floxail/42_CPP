/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 05/10/2026 07:18:27 by flvejux           #+#    #+#             */
/*   Updated: 12/10/2026 07:44:11 by flvejux          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Harl.hpp"
#include <iostream>
#include <string>

int   main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Usage: ./harl <LEVEL>" << std::endl;
        return 1;
    }
    std::string level = av[1];
    Harl harl;
    harl.complain(level);
    return 0;
}
