#include"RPN.hpp"


//プログラムの実行中にエラーが発生した場合は、エラーメッセージを標準エラーに表示しなければならない。

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "Error: Invalid argument." << std::endl;
		return (EXIT_FAILURE);
	}



	RPN::calc_RPN(argc, argv[]);

	//todo: 引数の数値が数字かつ10以下か確認
	return (EXIT_SUCCESS);
}
