#include"PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <cctype>
#include <climits>

/*
 * デフォルトコンストラクタ
 */
PmergeMe::PmergeMe() {}


/*
 * コンストラクタ
 */
PmergeMe::PmergeMe(const unsigned int n) : _size(n) {}

/*
 * コピーコンストラクタ
 */
PmergeMe::PmergeMe(const PmergeMe& copy) {
	_size = copy._size;
}

/*
 * コピー代入演算子 (A copy assignment operator overload.)
 */
PmergeMe &PmergeMe::operator=(const PmergeMe& copy) {
	if (this != &copy) {
		_size = copy._size;
	}
	return (*this);
}

/*
 * デストラクタ
 */
PmergeMe::~PmergeMe() {}


// ↑↑↑ Orthodox Canonical Form --------------------------------------

// 文字列が正の整数かを確認し、変換する（負値・非数は false）
static bool validPositiveInt(const std::string& s, int& out)
{
	if (s.empty())
		return (false);

	size_t start = 0;
	if (s[0] == '+')
		start = 1;
	if (start >= s.size())
		return (false);

	for (size_t i = start; i < s.size(); ++i) {
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return (false);
	}
	std::stringstream ss(s);
	long val;
	if (!(ss >> val))
		return (false);
	if (val < 0 || val > INT_MAX)
		return (false);
	out = static_cast<int>(val);
	return (true);
}

bool	PmergeMe::execute_1(char**	input) {
	for (unsigned int i = 0; i < _size; i++) {
		// std::cout << "[DEBUG] " << input[i] << std::endl;
		int num;
		//整数か確認
		if (!validPositiveInt(std::string(input[i]), num)) {
			std::cerr << "Error: 引数が不正です" << std::endl;
			return (false);
		}
	}
	return(true);
}
