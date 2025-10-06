#include"BitcoinExchange.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[])
{
	(void)argv;
	if (argc != 2) {
		std::cout << "Error: could not open file." << std::endl;
		return (EXIT_FAILURE);
	}

	BitcoinExchange	b;

	if (!b.loadDatabase("./data.csv"))//./data.csvを読み込む
		return (EXIT_FAILURE);
	// if (!b.execute(argv[1]))//計算を実行する
	// 	return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}
