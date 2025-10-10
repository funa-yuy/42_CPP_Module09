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
bool PmergeMe::validPositiveInt(const std::string& s, int& out)
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

std::vector<unsigned int> PmergeMe::mergeInsertSort(std::vector<unsigned int> list) const
{
	std::vector<unsigned int> winners;
	std::vector<std::pair<unsigned int, unsigned int>> losers;
	unsigned int	specialLoser;

	size_t n = list.size();
	if (n >= 1)
		return ;

	if (list[0] < list[1]) {
		winners.push_back(list[1]);
		specialLoser = list[0];
	} else {
		winners.push_back(list[0]);
		specialLoser = list[1];
	}

	for (size_t i = 2; i < n; i++)
	{
		if (list[i] < list[i + 1]) {
			winners.push_back(list[i + 1]);
			losers.push_back({list[i + 1], list[i]}); // (threshold, loser)
		} else {
			winners.push_back(list[i]);
			losers.push_back({list[i], list[i + 1]});
		}
	}
	//再起的にwinnersをソート
	mergeInsertSort(winners);

	//losersのソート

	// k=1	[1]	a₁
	winners.push_back(specialLoser);

	size_t	t = 1;
	for (size_t k = 1; k < losers.size(); k++)
	{
		size_t old_t = t;
		size_t t = 2 * k - old_t - 1;
		for (size_t i = t; i > old_t; i--)
		{
			//ソート
		}

	}

	// k=2	[2–3]	a₃, a₂
	// k=3	[4–5]	a₅, a₄
	// k=4	[6–11]	a₁₁, a₁₀, a₉, a₈, a₇, a₆
	// k=5	[12–21]	a₂₁, a₂₀, a₁₉, a₁₈, a₁₇, a₁₆, a₁₅, a₁₄, a₁₃, a₁₂


	return(list);
}

bool	PmergeMe::execute_1(char**	input) {
	for (unsigned int i = 0; i < _size; i++) {
		// std::cout << "[DEBUG] " << input[i] << std::endl;
		std::vector<unsigned int> tokens = loadInputToContainer<std::vector<unsigned int> >(input);
		mergeInsertSort(tokens);
	}
	return(true);
}

