#include"PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <cctype>
#include <climits>
#include <utility>
#include <algorithm>
#include <sys/time.h>

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

static int jacobsthal(int n) {
	if (n == 0)
		return (0);
	if (n == 1)
		return (1);

	int prev2 = 0;
	int prev1 = 1;
	int curr;

	for (int i = 2; i <= n; ++i) {
		curr = prev1 + 2 * prev2;
		prev2 = prev1;
		prev1 = curr;
	}
	return (curr);
}

// Jacobsthal 数列の順序で losers を chain に挿入
static void insertLosers(
	std::vector<unsigned int>& chain,
	const std::vector<std::pair<unsigned int, unsigned int> >& losers)
{
	int start = 0;
	int k = 1;
	while (start < (int)losers.size()) {
		int end = jacobsthal(k);
		if (end > (int)losers.size())
			end = (int)losers.size();

		// [start, end) の範囲を降順で挿入
		for (int i = end - 1; i >= start; --i) {
			unsigned int winner = losers[i].first;
			unsigned int loser = losers[i].second;

			std::cout << "[DEBUG INSERT] " << k << ": end=" << end
						<< ", target=" << i << ", (loser=" << loser
						<< " (winner=" << winner << ")\n";

			// winner の位置を探す
			std::vector<unsigned int>::iterator winnerIt =
				std::lower_bound(chain.begin(), chain.end(), winner);

			// [begin, winnerIt) の範囲で loser を二分挿入
			std::vector<unsigned int>::iterator ins =
				std::lower_bound(chain.begin(), winnerIt, loser);
			chain.insert(ins, loser);
		}
		start = end;
		++k;
	}
}

// void PmergeMe::mergeInsertSort(std::vector<unsigned int>& list, bool isTopLevel) const
// {
// 	std::vector<unsigned int> chain;
// 	std::vector<std::pair<unsigned int, unsigned int> > losers;

// 	size_t n = list.size();
// 	if (n <= 1)
// 		return ;

// 	if (n % 2 == 1) {
// 		unsigned int extra = list.back();
// 		std::cout << "\n[DEBUG extra]: " << extra << "\n";

// 		list.pop_back();
// 		mergeInsertSort(list, false);
// 		binaryInsert(list, extra);
// 		return ;
// 	}

// 	for (size_t i = 0; i + 1 < n; i += 2)
// 	{
// 		unsigned int a = list[i];
// 		unsigned int b = list[i + 1];
// 		std::cout << "[DEBUG] i = " << i << "... list[i]: " << a << ", list[i + 1]: " << b << "\n";
// 		if (a < b) {
// 			chain.push_back(b);
// 			losers.push_back(std::make_pair(b, a));//(winner, losers)
// 		} else {
// 			chain.push_back(a);
// 			losers.push_back(std::make_pair(a, b));
// 		}
// 	}

// 	//再起的にchainをソート
// 	mergeInsertSort(chain, false);


// 	// //losersのソート
// 	if (isTopLevel) {
// 		// chain[0] とペアを組んでいる loser を探す
// 		unsigned int firstWinner = chain[0];
// 		for (size_t i = 0; i < losers.size(); ++i) {
// 			if (losers[i].first == firstWinner) {
// 				// このペアの loser を先頭に挿入
// 				unsigned int firstLoser = losers[i].second;
// 				std::cout << "\n[DEBUG firstLoser]: " << firstLoser;
// 				chain.insert(chain.begin(), firstLoser);

// 				// この loser は処理済みなので losers から削除
// 				losers.erase(losers.begin() + i);
// 				break;
// 			}
// 		}
// 	}

// 	std::cout << "\n[DEBUG chain]:";
// 	for (size_t i = 0; i < chain.size(); i++) {
// 		std::cout << " " << chain[i];
// 	}
// 	std::cout << "\n";

// 	std::cout << "\n[DEBUG losers]:";
// 	for (size_t i = 0; i < losers.size(); i++) {
// 		std::cout << " (threshold: " << losers[i].first << ", loser: " << losers[i].second << ")";
// 	}
// 	std::cout << "\n";

// 	insertLosers(chain, losers);

// 	list.swap(chain);
// }

void PmergeMe::mergeInsertSort(std::vector<unsigned int>& list, bool isTopLevel) const
{
	// winner の値と、元のインデックスと loser をセットで管理
	std::vector<std::pair<unsigned int, size_t> > chainWithIndex;  // (winner値, 元index)
	std::vector<unsigned int> losersByIndex;  // インデックスで loser を管理

	size_t n = list.size();
	if (n <= 1)
		return ;

	// 奇数処理
	unsigned int extra = 0;
	bool hasExtra = false;
	if (n % 2 == 1) {
		extra = list.back();
		list.pop_back();
		hasExtra = true;
		n--;
	}

	// 全ペアを処理
	for (size_t i = 0; i + 1 < n; i += 2) {
		unsigned int a = list[i];
		unsigned int b = list[i + 1];
		size_t pairIndex = i / 2;  // ペアのインデックス

		if (a < b) {
			chainWithIndex.push_back(std::make_pair(b, pairIndex));
			losersByIndex.push_back(a);
		} else {
			chainWithIndex.push_back(std::make_pair(a, pairIndex));
			losersByIndex.push_back(b);
		}
	}

	// winner の値だけを取り出して再帰ソート
	std::vector<unsigned int> chain;
	for (size_t i = 0; i < chainWithIndex.size(); ++i) {
		chain.push_back(chainWithIndex[i].first);
	}

	mergeInsertSort(chain, false);

	// 再帰後：chain の順序に合わせて losers を再構築
	std::vector<std::pair<unsigned int, unsigned int> > losers;
	for (size_t i = 0; i < chain.size(); ++i) {
		unsigned int winner = chain[i];

		// この winner の元のインデックスを探す
		for (size_t j = 0; j < chainWithIndex.size(); ++j) {
			if (chainWithIndex[j].first == winner) {
				size_t originalIndex = chainWithIndex[j].second;
				unsigned int loser = losersByIndex[originalIndex];
				losers.push_back(std::make_pair(winner, loser));

				// 同じ値が複数ある場合の対策（使用済みをマーク）
				chainWithIndex[j].first = UINT_MAX;
				break ;
			}
		}
	}

	// isTopLevel なら chain[0] のペアを先頭に挿入
	if (isTopLevel && !losers.empty()) {
		unsigned int firstLoser = losers[0].second;
		chain.insert(chain.begin(), firstLoser);
		losers.erase(losers.begin());
	}

	std::cout << "\n[DEBUG chain]:";
	for (size_t i = 0; i < chain.size(); i++)
		std::cout << " " << chain[i];
	std::cout << "\n";

	std::cout << "\n[DEBUG losers]:";
	for (size_t i = 0; i < losers.size(); i++)
		std::cout << " (threshold: " << losers[i].first << ", loser: " << losers[i].second << ")";
	std::cout << "\n";

	insertLosers(chain, losers);

	if (hasExtra)
		binaryInsert(chain, extra);

	list.swap(chain);
}

bool	PmergeMe::execute_1(char**	input) {
	struct timeval start, end;
	gettimeofday(&start, NULL);  // 開始時刻

	std::vector<unsigned int> tokens = loadInputToContainer<std::vector<unsigned int> >(input);
	std::cout << "\n[DEBUG PARSE]:";
	for (unsigned int i = 0; i < tokens.size(); i++) {
		std::cout << " " << tokens[i];
	}
	std::cout << "\n";

	mergeInsertSort(tokens, true);

	gettimeofday(&end, NULL);  // 終了時刻

	// マイクロ秒単位で計算
	long seconds = end.tv_sec - start.tv_sec;
	long microseconds = end.tv_usec - start.tv_usec;
	double elapsed = seconds * 1000000.0 + microseconds;

	std::cout << "\nAfter:";
	for (unsigned int i = 0; i < tokens.size(); i++) {
		std::cout << " " << tokens[i];
	}

	std::cout << "\n要素数: " << tokens.size()
			<< ", 使用コンテナ: std::vector, 処理にかかった時間: "
			<< elapsed << " us(マイクロ秒)" << std::endl;

	return(true);
}

