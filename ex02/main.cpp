#include"BitcoinExchange.hpp"


//プログラムの実行中にエラーが発生した場合は、エラーメッセージを標準エラーに表示しなければならない。

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "引数が不正です" << std::endl;
		return (EXIT_FAILURE);
	}

	//プログラムは、引数として正の整数列( integer sequence)を使用できる必要があります。
	//todo: 引数が全て星の整数か確認
	return (EXIT_SUCCESS);
}
