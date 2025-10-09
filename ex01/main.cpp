#include"RPN.hpp"

//プログラムの実行中にエラーが発生した場合は、エラーメッセージを標準エラーに表示しなければならない。
int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "Error: Invalid argument." << std::endl;
		return (EXIT_FAILURE);
	}

	RPN	rpn;
	if (!rpn.execute(argv[1])) {
		exit (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
