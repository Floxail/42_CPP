/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flvejux <flvejux@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 07:24:56 by flvejux           #+#    #+#             */
/*   Updated: 2026/10/05 07:25:27 by flvejux          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cctype>

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