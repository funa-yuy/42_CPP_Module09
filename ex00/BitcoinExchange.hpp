#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

// ------------------------------------------------
// include
// ------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ------------------------------------------------
// class
// ------------------------------------------------

class BitcoinExchange {
	public:
		BitcoinExchange();
		BitcoinExchange& operator=(const BitcoinExchange& copy);
		BitcoinExchange(const BitcoinExchange& copy);
		~BitcoinExchange();

		bool	loadDatabase(char* databaseFile);
		bool	execute(char* input);
	private:
		std::vector<std::string, double>	_database;
};

// ------------------------------------------------
// function
// ------------------------------------------------

#endif
