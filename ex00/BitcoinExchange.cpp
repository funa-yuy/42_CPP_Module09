#include"BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>

/*
 * デフォルトコンストラクタ
 */
BitcoinExchange::BitcoinExchange() {}

/*
 * コピーコンストラクタ
 */
BitcoinExchange::BitcoinExchange(const BitcoinExchange& copy) {
	_database = copy._database;
}

/*
 * コピー代入演算子 (A copy assignment operator overload.)
 */
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange& copy) {
	if (this != &copy)
	{
		_database = copy._database;
	}
	return (*this);
}

/*
 * デストラクタ
 */
BitcoinExchange::~BitcoinExchange() {}


// ↑↑↑ Orthodox Canonical Form --------------------------------------

//左右の空白除去
static std::string trimIsspace(const std::string& s)
{
	std::string::size_type start = 0;
	while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
		++start;
	std::string::size_type end = s.size();
	while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
		--end;
	return (s.substr(start, end - start));
}

// 日付の簡易検証 (YYYY-MM-DD)
static bool isValidDate(const std::string& date)
{
	if (date.size() != 10)
		return (false);
	if (date[4] != '-' || date[7] != '-')
		return (false);
	for (std::size_t i = 0; i < date.size(); ++i) {
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return (false);
	}
	int m = std::atoi(date.substr(5, 2).c_str());
	int d = std::atoi(date.substr(8, 2).c_str());
	if (m < 1 || m > 12)
		return (false);
	if (d < 1 || d > 31)
		return (false);
	return (true);
}

// レートの取得（欠損・不正・負値は失敗）
static bool parseRate(const std::string& valueStr, double& out)
{
	if (valueStr.size() == 0)
		return (false);
	std::stringstream ss(valueStr);
	double v = 0.0;
	if (!(ss >> v))
		return (false);
	ss >> std::ws;
	if (!ss.eof())
		return(false);
	if (v < 0.0)
		return (false);
	out = v;
	return (true);
}

// 行を最初のカンマで2分割して date/valueStr を返す
static void splitFirstComma(const std::string& line, std::string& date, std::string& valueStr)
{
	std::string::size_type comma = line.find(',');
	if (comma == std::string::npos)
	{
		date = line;
		valueStr.clear();
		return;
	}
	date = line.substr(0, comma);
	valueStr = line.substr(comma + 1);
}

bool	BitcoinExchange::loadDatabase(const char* databaseFile) {
	std::ifstream ifs(databaseFile);
	if (!ifs.is_open())
		return (false);

	std::string line;
	if (!std::getline(ifs, line))//1行目は確定でスキップ
		return (false);

	while (std::getline(ifs, line))
	{
		line = trimIsspace(line);
		if (line.empty())
			continue;

		std::string date;
		std::string valueStr;
		splitFirstComma(line, date, valueStr);

		date = trimIsspace(date);
		valueStr = trimIsspace(valueStr);

		if (!isValidDate(date))
			continue;

		double rate;
		if (!parseRate(valueStr, rate))
			continue;

		_database[date] = rate;
	}
	return (true);
}

// bool	validateDate(char line) {
// 	//todo: inputファイルの内容が正しいか確認
// 	if (構文がおかしい)
// 		return ("Error: bad input => " + line);
// 	if (日付がデーターベース範囲外)
// 		return ("Error: Date is out of database range.");
// 	if (valueが負の値)
// 		return ("Error: not a positive number")
// 	if (value > INT_MAX) //valueがintの範囲外
// 		return ("Error: too large a number.");
// 	return (true);
// }

// double getExchangeRate(const std::string& line) {

// 	data = line.dateから一番値が近いdatabaseの日付行
// 	return (line.value * data.prices);
// }

// bool	BitcoinExchange::execute(char* input) {

// 	//todo: inputfileの内容を一気に格納する


// 	//todo: １行ずつ計算して、結果出力
// 	while (line = 1行)
// 	{
// 		if (!validateDate(line))
// 			return (false);
// 		double result = getExchangeRate(line);
// 		std::cout << line.date << " => " << line.value << " = " << result << std::endl;
// 	}
// 	return (true);
// }
