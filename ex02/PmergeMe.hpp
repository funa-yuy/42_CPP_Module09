#ifndef PMERGEME_HPP
# define PMERGEME_HPP

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

class PmergeMe {
	public:
		PmergeMe();
		PmergeMe(const unsigned int n);
		PmergeMe& operator=(const PmergeMe& copy);
		PmergeMe(const PmergeMe& copy);
		~PmergeMe();

		void addNumber(int value);
		void addNumber(int pos, const std::vector<int>& values);

		int	shortestSpan();
		int	longestSpan();

	private:
		std::vector<int>	_vec;
		unsigned int _maxSize;
};

// ------------------------------------------------
// function
// ------------------------------------------------

#endif
