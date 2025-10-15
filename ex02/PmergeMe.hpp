#ifndef PMERGEME_HPP
# define PMERGEME_HPP

// ------------------------------------------------
// include
// ------------------------------------------------

#include <string>
#include <vector>
#include <deque>

// ------------------------------------------------
// class
// ------------------------------------------------

class PmergeMe {
	public:
		PmergeMe();
		PmergeMe(char** input, const unsigned int n);
		PmergeMe& operator=(const PmergeMe& copy);
		PmergeMe(const PmergeMe& copy);
		~PmergeMe();

		bool	validPositiveInt(const std::string& s, int& out) const;
		void	setTimeus(struct timeval start, struct timeval end, double& outTime);
		void	printResult();

		bool	executeVecter();
		std::vector<unsigned int> loadInputToVector(char** input) const;
		void	mergeInsertSort(std::vector<unsigned int>& list) const;
		void	binaryInsert(std::vector<unsigned int>& arr, unsigned int value) const;


		bool	executeDeque();
		std::deque<unsigned int> loadInputToDeque(char** input) const;
		void	mergeInsertSort(std::deque<unsigned int>& list) const;
		void	binaryInsert(std::deque<unsigned int>& arr, unsigned int value) const;

	private:
		char**						_inputList;
		int							_size;
		std::vector<unsigned int>	_vecList;
		std::deque<unsigned int>	_deqList;
		double						_vecTimeus;
		double						_deqTimeus;
};

#endif
