#ifndef PMERGEME_HPP
# define PMERGEME_HPP

// ------------------------------------------------
// include
// ------------------------------------------------

#include <cctype>
#include <cstdlib>
#include <climits>
#include <string>
#include <vector>

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

		bool	validPositiveInt(const std::string& s, int& out);
		bool	execute_1(char**	input);
		std::vector<unsigned int> mergeInsertSort(std::vector<unsigned int> a) const;

		template <typename Container>
		Container	loadInputToContainer(char** input) const;


	private:
		unsigned int _size;
};

// ------------------------------------------------
// function
// ------------------------------------------------

template <typename Container>
Container PmergeMe::loadInputToContainer(char** input) const
{
	Container out;
	for (unsigned int i = 0; i < _size; ++i)
	{
		int num;
		if (!validPositiveInt(std::string(input[i]), num))
			return Container();
		out.push_back(static_cast<typename Container::value_type>(num));
	}
	return out;
}

#endif
