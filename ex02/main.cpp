#include"PmergeMe.hpp"
#include <iostream>
#include <cstdlib>

//プログラムの実行中にエラーが発生した場合は、エラーメッセージを標準エラーに表示しなければならない。

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Error: 引数が不足しています" << std::endl;
		return (EXIT_FAILURE);
	}

	std::cout << "Before:";
	for (int i = 1; i < argc; ++i) {
		std::cout << " " << argv[i];
	}
	std::cout << "\n";

	PmergeMe p(argc - 1);
	//1個目のコンテナでのソート
	if (p.execute_1(&argv[1])) {
		return (EXIT_FAILURE);
	}

	//2個目のコンテナでのソート
		//todo: 引数が全て整数か確認
	return (EXIT_SUCCESS);
}
