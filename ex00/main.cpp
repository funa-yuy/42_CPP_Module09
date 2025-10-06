#include"BitcoinExchange.hpp"

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cout << "Error: could not open file." << std::endl;
		return (EXIT_FAILURE);
	}

	BitcoinExchange	b;

	if (!b.loadDatabase("./data.csv"))
		return (EXIT_FAILURE);
	if (!b.execute(argv[1]))
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}
