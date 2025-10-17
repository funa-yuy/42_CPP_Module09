#include"PmergeMe.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Error: 引数が不足しています" << std::endl;
		return (EXIT_FAILURE);
	}
	PmergeMe p(&argv[1], argc - 1);
	if (!p.executeVecter())
		return (EXIT_FAILURE);
	if (!p.executeDeque())
		return (EXIT_FAILURE);
	p.printResult();
	return (EXIT_SUCCESS);
}
