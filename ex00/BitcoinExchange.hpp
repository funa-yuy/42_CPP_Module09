#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

// ------------------------------------------------
// include
// ------------------------------------------------

#include <map>
#include <string>

// ------------------------------------------------
// class
// ------------------------------------------------

class BitcoinExchange {
	public:
		BitcoinExchange();
		BitcoinExchange& operator=(const BitcoinExchange& copy);
		BitcoinExchange(const BitcoinExchange& copy);
		~BitcoinExchange();

		bool	loadDatabase(const char* databaseFile);
		bool	execute(char* input);
		bool	validInputLine(const std::string& line);

	private:
		std::map<std::string, double>	_database;
};

// ------------------------------------------------
// function
// ------------------------------------------------

#endif
