/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:21:31 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/09 12:27:27 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>

class	Location
{
	public:
		Location();
		Location(const Location &other);
		Location	&operator=(const Location &rhs);
		~Location();
	private:
		std::string		_root;
};

#endif
