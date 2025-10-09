#ifndef PMERGEME_HPP
# define PMERGEME_HPP

// ------------------------------------------------
// include
// ------------------------------------------------

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

		bool	execute_1(char**	input);

	private:
		unsigned int _size;
};

// ------------------------------------------------
// function
// ------------------------------------------------

#endif
