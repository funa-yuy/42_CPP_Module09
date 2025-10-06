#include"PmergeMe.hpp"

/*
 * デフォルトコンストラクタ
 */
PmergeMe::PmergeMe() {}


/*
 * コンストラクタ
 */
PmergeMe::PmergeMe(unsigned int n) : _maxSize(n) {
	_vec.reserve(n);
}

/*
 * コピーコンストラクタ
 */
PmergeMe::PmergeMe(const PmergeMe& copy) {
	_maxSize = copy._maxSize;
	_vec = copy._vec;
}

/*
 * コピー代入演算子 (A copy assignment operator overload.)
 */
PmergeMe &PmergeMe::operator=(const PmergeMe& copy) {
	if (this != &copy)
	{
		_maxSize = copy._maxSize;
		_vec = copy._vec;
	}
	return (*this);
}

/*
 * デストラクタ
 */
PmergeMe::~PmergeMe() {}


// ↑↑↑ Orthodox Canonical Form --------------------------------------
