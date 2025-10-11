#include"PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <cctype>
#include <climits>
#include <utility>
#include <algorithm>

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

// 昇順ベクタに対する二分挿入
void PmergeMe::binaryInsert(std::vector<unsigned int>& arr, unsigned int value) const
{
	std::vector<unsigned int>::iterator it = std::lower_bound(arr.begin(), arr.end(), value);
	arr.insert(it, value);
}

// 文字列が正の整数かを確認し、変換する（負値・非数は false）
bool PmergeMe::validPositiveInt(const std::string& s, int& out) const
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

void PmergeMe::mergeInsertSort(std::vector<unsigned int>& list) const
{
	std::vector<unsigned int> winners;
	std::vector<std::pair<unsigned int, unsigned int> > losers;

	size_t n = list.size();
	if (n <= 1)
		return ;

	if (n % 2 == 1) {
		unsigned int extra = list.back();   // ① 最後の1個を取り出す
		std::cout << "\n[DEBUG extra]: " << extra << "\n";

		list.pop_back();           // ② A から削除して偶数個にする
		mergeInsertSort(list);     // ③ 残り（偶数個）を再帰的にソート
		binaryInsert(list, extra); // ④ ソート済みのAに、最後の要素を二分挿入
		return ;
	}

	unsigned int	specialLoser;
	if (list[0] < list[1]) {
		winners.push_back(list[1]);
		specialLoser = list[0];
	} else {
		winners.push_back(list[0]);
		specialLoser = list[1];
	}

	for (size_t i = 2; i + 1 < n; i += 2)
	{
		unsigned int a = list[i];
		unsigned int b = list[i + 1];
		std::cout << "[DEBUG] i = " << i << "... list[i]: " << a << ", list[i + 1]: " << b << "\n";
		if (a < b) {
			winners.push_back(b);
			losers.push_back(std::make_pair(b, a));
		} else {
			winners.push_back(a);
			losers.push_back(std::make_pair(a, b));
		}
	}

	//再起的にwinnersをソート
	mergeInsertSort(winners);


	//losersのソート
	std::cout << "\n[DEBUG specialLoser]: " << specialLoser;
	winners.insert(winners.begin(), specialLoser);

	std::cout << "\n[DEBUG winners]:";
	for (size_t i = 0; i < winners.size(); i++) {
		std::cout << " " << winners[i];
	}
	std::cout << "\n";

	std::cout << "\n[DEBUG losers]:";
	for (size_t i = 0; i < losers.size(); i++) {
		std::cout << " (threshold: " << losers[i].first << ", loser: " << losers[i].second << ")";
	}
	std::cout << "\n";

	// size_t	t = 1;
	// for (size_t k = 1; k < losers.size(); k++)
	// {
	// 	size_t old_t = t;
	// 	size_t t = 2 * k - old_t - 1;
	// 	for (size_t i = t; i > old_t; i--)
	// 	{
	// 	}

	// }

	// k=2	[2–3]	a₃, a₂
	// k=3	[4–5]	a₅, a₄
	// k=4	[6–11]	a₁₁, a₁₀, a₉, a₈, a₇, a₆
	// k=5	[12–21]	a₂₁, a₂₀, a₁₉, a₁₈, a₁₇, a₁₆, a₁₅, a₁₄, a₁₃, a₁₂

	list.swap(winners);
}

bool	PmergeMe::execute_1(char**	input) {
	std::cout << "\n[DEBUG INPUT]:";
	for (unsigned int i = 0; i < _size; i++) {
		std::cout << " " << input[i];
	}
	std::cout << "\n";

	std::vector<unsigned int> tokens = loadInputToContainer<std::vector<unsigned int> >(input);
	std::cout << "\n[DEBUG PARSE]:";
	for (unsigned int i = 0; i < tokens.size(); i++) {
		std::cout << " " << tokens[i];
	}
	std::cout << "\n";

	mergeInsertSort(tokens);

	std::cout << "\n[DEBUG SORT]:";
	for (unsigned int i = 0; i < tokens.size(); i++) {
		std::cout << " " << tokens[i];
	}
	std::cout << "\n";
	return(true);
}

