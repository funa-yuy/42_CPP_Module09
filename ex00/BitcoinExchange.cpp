#include"BitcoinExchange.hpp"

/*
 * デフォルトコンストラクタ
 */
BitcoinExchange::BitcoinExchange() {}

/*
 * コピーコンストラクタ
 */
BitcoinExchange::BitcoinExchange(const BitcoinExchange& copy) {
	_maxSize = copy._maxSize;
	_vec = copy._vec;
}

/*
 * コピー代入演算子 (A copy assignment operator overload.)
 */
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange& copy) {
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
BitcoinExchange::~BitcoinExchange() {}


// ↑↑↑ Orthodox Canonical Form --------------------------------------

bool	BitcoinExchange::loadDatabase(char* databaseFile) {

}

bool	validateDate(char line) {
	//todo: inputファイルの内容が正しいか確認
	if (構文がおかしい)
		return ("Error: bad input => " + line);
	if (日付がデーターベース範囲外)
		return ("Error: Date is out of database range.");
	if (valueが負の値)
		return ("Error: not a positive number")
	if (value > INT_MAX) //valueがintの範囲外
		return ("Error: too large a number.");
	return (true);
}

double getExchangeRate(const std::string& line) {

	data = line.dateから一番値が近いdatabaseの日付行
	return (line.value * data.prices);
}

bool	BitcoinExchange::execute(char* input) {

	//todo: inputfileの内容を一気に格納する


	//todo: １行ずつ計算して、結果出力
	while (line = 1行)
	{
		if (!validateDate(line))
			return (false);
		double result = getExchangeRate(line)
		std:cout << line.date << " => " << line.value << " = " << result << std::endl;
	}
	return (true);
}
