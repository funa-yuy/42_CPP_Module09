#include"BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <cctype>
#include <cstdlib>
#include <iomanip>

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
	int y = std::atoi(date.substr(0, 4).c_str());
	int m = std::atoi(date.substr(5, 2).c_str());
	int d = std::atoi(date.substr(8, 2).c_str());
	if (m < 1 || m > 12)
		return (false);
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)))
		daysInMonth[1] = 29;
	if (d < 1 || d > daysInMonth[m - 1])
		return (false);
	if (d < 1 || d > 31)
		return (false);
	return (true);
}

// レートの取得（変換のみ行う。空/非数/末尾ゴミで false）
static bool parseRate(const std::string& valueStr, double& out)
{
	if (valueStr.size() == 0)
		return (false);
	std::stringstream ss(valueStr);
	double v;
	if (!(ss >> v))
		return (false);
	ss >> std::ws;
	if (!ss.eof())
		return (false);
	out = v;
	return (true);
}

static void splitFirstComma(const std::string& line, std::string& date, std::string& valueStr)
{
	std::string::size_type comma = line.find(',');
	if (comma == std::string::npos) {
		date = line;
		valueStr.clear();
		return;
	}
	date = line.substr(0, comma);
	valueStr = line.substr(comma + 1);
}

bool	BitcoinExchange::loadDatabase(const char* databaseFile) {
	std::ifstream ifs(databaseFile);
	if (!ifs.is_open()) {
		std::cout << "Error: could not open data file." << std::endl;
		return (false);
	}

	std::string line;
	if (!std::getline(ifs, line))
		return (false);

	while (std::getline(ifs, line)) {
		line = trimIsspace(line);
		if (line.empty())
			continue;

		std::string date;
		std::string valueStr;
		splitFirstComma(line, date, valueStr);

		date = trimIsspace(date);
		valueStr = trimIsspace(valueStr);
		if (date.empty() || valueStr.empty())
			return (false);

		if (!isValidDate(date))
			continue;

		double rate;
		if (!parseRate(valueStr, rate) || rate < 0.0)
			continue;

		_database[date] = rate;
	}
	return (true);
}

static bool isValidSyntax(const std::string& line, std::string& outDate, double& outValue)
{
	std::string::size_type bar = line.find('|');
	if (bar == std::string::npos)
		return (false);

	std::string left = trimIsspace(line.substr(0, bar));
	std::string right = trimIsspace(line.substr(bar + 1));
	if (left.size() == 0 || right.size() == 0)
		return (false);

	if (!isValidDate(left))
		return (false);

	double v;
	if (!parseRate(right, v))
		return (false);

	outDate = left;
	outValue = v;
	return (true);
}

// 日付が妥当範囲内かを検証（存在チェック/下限存在チェック）。
static bool isValidDateRange(const std::map<std::string, double>& db, const std::string& requestedDate)
{
	if (db.empty())
		return (false);

	std::map<std::string, double>::const_iterator it = db.lower_bound(requestedDate);
	if (it != db.end() && it->first == requestedDate)
		return (true);
	if (it == db.begin())
		return (false);
	return (true);
}

bool	BitcoinExchange::validInputLine(const std::string& line, std::string& outDate, double& outValue) {
	if (!isValidSyntax(line, outDate, outValue)) {
		std::cout << "Error: bad input => " <<  line << std::endl;
		return(false);
	}
	if (!isValidDateRange(_database, outDate)) {
		std::cout << "Error: Date is out of database range." << std::endl;
		return(false);
	}
	if (outValue < 0.0) {
		std::cout << "Error: not a positive number." << std::endl;
		return(false);
	}
	if (outValue > 1000) {
		std::cout << "Error: too large a number." << std::endl;
		return(false);
	}
	return (true);
}

double BitcoinExchange::getExchangeRate(const std::string& date, double value) const {
	if (_database.empty())
		return (0.0);
	std::map<std::string,double>::const_iterator it = _database.lower_bound(date);
	if (it == _database.end()) {
		--it;
	} else if (it->first != date) {
		if (it == _database.begin())
			return (0.0);
		--it;
	}
	return (value * it->second);
}

bool	BitcoinExchange::execute(char* input) {
	std::ifstream ifs(input);
	if (!ifs.is_open()) {
		std::cout << "Error: could not open file." << std::endl;
		return (false);
	}

	std::string line;
	if (!std::getline(ifs, line))
		return (false);

	while (std::getline(ifs, line)) {
		std::string str = trimIsspace(line);
		if (str.empty())
			continue ;

		std::string outDate;
		double outValue;
		if (!validInputLine(str, outDate, outValue))
			continue ;

		double result = getExchangeRate(outDate, outValue);
		std::cout << outDate << " => " << outValue << " = "
				  << std::fixed << std::setprecision(2) << result << std::endl;
	}
	return (true);
}
