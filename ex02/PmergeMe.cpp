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
PmergeMe::PmergeMe() : _inputList(NULL), _size(0), _vecTimeus(0.0), _deqTimeus(0.0), _vecCompareCount(0), _deqCompareCount(0) {}


/*
 * コンストラクタ
 */
PmergeMe::PmergeMe(char** input, const unsigned int n)
 	: _inputList(input), _size(n), _vecTimeus(0.0), _deqTimeus(0.0), _vecCompareCount(0), _deqCompareCount(0) {}

/*
 * コピーコンストラクタ
 */
PmergeMe::PmergeMe(const PmergeMe& copy) {
	_size = copy._size;
	_vecList = copy._vecList;
	_deqList = copy._deqList;
	_vecTimeus = copy._vecTimeus;
	_deqTimeus = copy._deqTimeus;
	_vecCompareCount = copy._vecCompareCount;
	_deqCompareCount = copy._deqCompareCount;
}

/*
 * コピー代入演算子 (A copy assignment operator overload.)
 */
PmergeMe &PmergeMe::operator=(const PmergeMe& copy) {
	if (this != &copy) {
		_size = copy._size;
		_vecList = copy._vecList;
		_deqList = copy._deqList;
		_vecTimeus = copy._vecTimeus;
		_deqTimeus = copy._deqTimeus;
		_vecCompareCount = copy._vecCompareCount;
		_deqCompareCount = copy._deqCompareCount;
	}
	return (*this);
}

/*
 * デストラクタ
 */
PmergeMe::~PmergeMe() {}


// ↑↑↑ Orthodox Canonical Form --------------------------------------

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
	if (n <= 0)
		return (0);
	if (n == 1)
		return (1);
	if (n == 2)
		return (3);

	int prev2 = 1;
	int prev1 = 3;
	int curr;

	for (int i = 3; i <= n; ++i) {
		curr = prev1 + 2 * prev2;
		prev2 = prev1;
		prev1 = curr;
	}
	return (curr);
}

void	PmergeMe::setTimeus(struct timeval start, struct timeval end, double& outTime) {
	long seconds = end.tv_sec - start.tv_sec;
	long microseconds = end.tv_usec - start.tv_usec;
	double elapsed = seconds * 1000000.0 + microseconds;

	outTime = elapsed;
}

void	PmergeMe::printResult() {
	std::cout << "Before:";
	for (int i = 0; i < _size; ++i) {
		std::cout << " " << _inputList[i];
	}
	std::cout << std::endl << std::endl;

	std::cout << "After :";
	for (size_t i = 0; i < _vecList.size(); i++) {
		std::cout << " " << _vecList[i];
	}
	std::cout << std::endl << std::endl;

	std::cout << "要素数: " << _vecList.size()
			<< ", 使用コンテナ: std::vector"
			<< ", 処理にかかった時間: " << _vecTimeus << " us"
			<< ", 比較回数: " << _vecCompareCount << " 回"
			<< std::endl << std::endl;

	std::cout << "要素数: " << _deqList.size()
			<< ", 使用コンテナ: std::deque"
			<< ", 処理にかかった時間: " << _deqTimeus << " us"
			<< ", 比較回数: " << _deqCompareCount << " 回"
			<< std::endl << std::endl;
}

// ------------------------------------------------------------------------------------------------
// vecter
// ------------------------------------------------------------------------------------------------

std::vector<unsigned int> PmergeMe::loadInputToVector(char** input) const
{
	std::vector<unsigned int> out;
	for (int i = 0; i < _size; ++i)
	{
		int num;
		if (!validPositiveInt(std::string(input[i]), num)) {
			std::cerr << "Error: 不正な値が含まれています。" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		out.push_back(num);
	}
	return (out);
}

// 手動二分探索（比較回数カウント付き）
static std::vector<unsigned int>::iterator binarySearchWithCount(
	std::vector<unsigned int>::iterator begin,
	std::vector<unsigned int>::iterator end,
	unsigned int value,
	int& compareCount)
{
	std::vector<unsigned int>::iterator left = begin;
	std::vector<unsigned int>::iterator right = end;

	while (left < right) {
		std::vector<unsigned int>::iterator mid = left + (right - left) / 2;
		compareCount++;  // 比較回数カウント
		if (*mid < value) {
			left = mid + 1;
		} else {
			right = mid;
		}
	}
	return (left);
}

// 昇順ベクタに対する二分挿入（比較回数カウント付き）
void PmergeMe::binaryInsert(std::vector<unsigned int>& arr, unsigned int value, int& compareCount) const
{
	std::vector<unsigned int>::iterator it = binarySearchWithCount(arr.begin(), arr.end(), value, compareCount);
	arr.insert(it, value);
}

// Jacobsthal 数列の順序で losers を chain に挿入
static void insertLosers(
	std::vector<unsigned int>& chain,
	const std::vector<std::pair<unsigned int, unsigned int> >& losers,
	int& compareCount)
{
	int start = 1;
	int k = 1;
	while (start < (int)losers.size()) {
		int end = jacobsthal(k);
		if (end > (int)losers.size())
			end = (int)losers.size();

		// [start, end) の範囲を降順で挿入
		for (int i = end - 1; i >= start; --i) {
			unsigned int winner = losers[i].first;
			unsigned int loser = losers[i].second;

			std::vector<unsigned int>::iterator winnerIt = std::lower_bound(chain.begin(), chain.end(), winner);


			// [begin, winnerIt) の範囲で loser を二分挿入（比較カウントあり：本質的な比較）
			std::vector<unsigned int>::iterator ins =
				binarySearchWithCount(chain.begin(), winnerIt, loser, compareCount);
			chain.insert(ins, loser);
		}
		start = end;
		++k;
	}
}

void PmergeMe::mergeInsertSort(std::vector<unsigned int>& list, int& compareCount) const
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

		compareCount++;  // 比較回数カウント
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

	mergeInsertSort(chain, compareCount);

	// 再帰後：chain の順序に合わせて losers を再構築
	std::vector<std::pair<unsigned int, unsigned int> > losers;
	for (size_t i = 0; i < chain.size(); ++i) {
		unsigned int winner = chain[i];

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
	// 奇数で余ったextraをlosersの最後に追加
	if (hasExtra)
		losers.push_back(std::make_pair(UINT_MAX, extra));

	// chain[0] のペアを先頭に挿入
	if (!losers.empty()) {
		unsigned int firstLoser = losers[0].second;
		chain.insert(chain.begin(), firstLoser);
	}
	insertLosers(chain, losers, compareCount);

	list.swap(chain);
}

bool	PmergeMe::executeVecter() {
	struct timeval start, end;

	gettimeofday(&start, NULL);
	std::vector<unsigned int> tokens = loadInputToVector(_inputList);
	_vecCompareCount = 0;
	mergeInsertSort(tokens, _vecCompareCount);
	_vecList = tokens;
	gettimeofday(&end, NULL);

	setTimeus(start, end, _vecTimeus);
	return(true);
}


// ------------------------------------------------------------------------------------------------
// deque
// ------------------------------------------------------------------------------------------------

// std::deque<unsigned int> PmergeMe::loadInputToDeque(char** input) const
// {
// 	std::deque<unsigned int> out;
// 	for (int i = 0; i < _size; ++i)
// 	{
// 		int num;
// 		if (!validPositiveInt(std::string(input[i]), num)) {
// 			std::cerr << "Error: 不正な値が含まれています。" << std::endl;
// 			std::exit(EXIT_FAILURE);
// 		}
// 		out.push_back(num);
// 	}
// 	return (out);
// }

// // 手動二分探索（比較回数カウント付き・deque版）
// static std::deque<unsigned int>::iterator binarySearchWithCount(
// 	std::deque<unsigned int>::iterator begin,
// 	std::deque<unsigned int>::iterator end,
// 	unsigned int value,
// 	int& compareCount)
// {
// 	std::deque<unsigned int>::iterator left = begin;
// 	std::deque<unsigned int>::iterator right = end;

// 	while (left < right) {
// 		std::deque<unsigned int>::iterator mid = left + (right - left) / 2;
// 		compareCount++;  // 比較回数カウント
// 		if (*mid < value) {
// 			left = mid + 1;
// 		} else {
// 			right = mid;
// 		}
// 	}
// 	return left;
// }

// // 昇順ベクタに対する二分挿入（比較回数カウント付き）
// void PmergeMe::binaryInsert(std::deque<unsigned int>& arr, unsigned int value, int& compareCount) const
// {
// 	std::deque<unsigned int>::iterator it = binarySearchWithCount(arr.begin(), arr.end(), value, compareCount);
// 	arr.insert(it, value);
// }

// // Jacobsthal 数列の順序で losers を chain に挿入
// static void insertLosers(
// 	std::deque<unsigned int>& chain,
// 	const std::deque<std::pair<unsigned int, unsigned int> >& losers,
// 	int& compareCount)
// {
// 	int start = 0;
// 	int k = 1;
// 	while (start < (int)losers.size()) {
// 		int end = jacobsthal(k);
// 		if (end > (int)losers.size())
// 			end = (int)losers.size();

// 		// [start, end) の範囲を降順で挿入
// 		for (int i = end - 1; i >= start; --i) {
// 			unsigned int winner = losers[i].first;
// 			unsigned int loser = losers[i].second;

// 			// winner の位置を探す（比較カウントなし：実装上の補助処理）
// 			std::deque<unsigned int>::iterator winnerIt =
// 				std::lower_bound(chain.begin(), chain.end(), winner);

// 			// [begin, winnerIt) の範囲で loser を二分挿入（比較カウントあり：本質的な比較）
// 			std::deque<unsigned int>::iterator ins =
// 				binarySearchWithCount(chain.begin(), winnerIt, loser, compareCount);
// 			chain.insert(ins, loser);
// 		}
// 		start = end;
// 		++k;
// 	}
// }

// void PmergeMe::mergeInsertSort(std::deque<unsigned int>& list, int& compareCount) const
// {
// 	// winner の値と、元のインデックスと loser をセットで管理
// 	std::deque<std::pair<unsigned int, size_t> > chainWithIndex;  // (winner値, 元index)
// 	std::deque<unsigned int> losersByIndex;  // インデックスで loser を管理

// 	size_t n = list.size();
// 	if (n <= 1)
// 		return ;

// 	// 奇数処理
// 	unsigned int extra = 0;
// 	bool hasExtra = false;
// 	if (n % 2 == 1) {
// 		extra = list.back();
// 		list.pop_back();
// 		hasExtra = true;
// 		n--;
// 	}

// 	// 全ペアを処理
// 	for (size_t i = 0; i + 1 < n; i += 2) {
// 		unsigned int a = list[i];
// 		unsigned int b = list[i + 1];
// 		size_t pairIndex = i / 2;  // ペアのインデックス

// 		compareCount++;  // 比較回数カウント
// 		if (a < b) {
// 			chainWithIndex.push_back(std::make_pair(b, pairIndex));
// 			losersByIndex.push_back(a);
// 		} else {
// 			chainWithIndex.push_back(std::make_pair(a, pairIndex));
// 			losersByIndex.push_back(b);
// 		}
// 	}
// 	// winner の値だけを取り出して再帰ソート
// 	std::deque<unsigned int> chain;
// 	for (size_t i = 0; i < chainWithIndex.size(); ++i) {
// 		chain.push_back(chainWithIndex[i].first);
// 	}
// 	mergeInsertSort(chain, compareCount);

// 	// 再帰後：chain の順序に合わせて losers を再構築
// 	std::deque<std::pair<unsigned int, unsigned int> > losers;
// 	for (size_t i = 0; i < chain.size(); ++i) {
// 		unsigned int winner = chain[i];

// 		// この winner の元のインデックスを探す
// 		for (size_t j = 0; j < chainWithIndex.size(); ++j) {
// 			// compareCount++;  // 比較回数カウント
// 			if (chainWithIndex[j].first == winner) {
// 				size_t originalIndex = chainWithIndex[j].second;
// 				unsigned int loser = losersByIndex[originalIndex];
// 				losers.push_back(std::make_pair(winner, loser));
// 				// 同じ値が複数ある場合の対策（使用済みをマーク）
// 				chainWithIndex[j].first = UINT_MAX;
// 				break ;
// 			}
// 		}
// 	}
// 	// chain[0] のペアを先頭に挿入
// 	if (!losers.empty()) {
// 		unsigned int firstLoser = losers[0].second;
// 		chain.insert(chain.begin(), firstLoser);
// 		losers.erase(losers.begin());
// 	}
// 	if (hasExtra)
// 		binaryInsert(chain, extra, compareCount);
// 	insertLosers(chain, losers, compareCount);

// 	list.swap(chain);
// }

// bool	PmergeMe::executeDeque() {
// 	struct timeval start, end;

// 	gettimeofday(&start, NULL);
// 	std::deque<unsigned int> tokens = loadInputToDeque(_inputList);
// 	_deqCompareCount = 0;
// 	mergeInsertSort(tokens, _deqCompareCount);
// 	_deqList = tokens;
// 	gettimeofday(&end, NULL);

// 	setTimeus(start, end, _deqTimeus);
// 	return(true);
// }

