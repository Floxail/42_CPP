/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 12:54:22 by flvejux           #+#    #+#             */
/*   Updated: 2026/09/06 13:39:41 by flvejux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int main(int ac, char **av)
{
    int i = 1;
    int y = 0;
    if(ac == 1)
        std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *";
    else
    {   
        while(i < ac)
        {
            while(av[i][y] != '\0')
            {
                std::cout << (char)toupper(av[i][y]);
                y++;
            }
            y = 0;
            i++;
        }
    }
    std::cout << std::endl;
}