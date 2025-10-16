#include"RPN.hpp"
#include <stack>
#include <sstream>

bool	RPN::execute(const char*	data) {
	std::stack<int> st;
	std::string str(data);

	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == ' ')
			continue;

		char target = str[i];
		if (target >= '0' && target <= '9') {
			st.push(target - '0');
			continue ;
		}

		if (target == '+' || target == '-' || target == '/' || target == '*') {
			if (st.size() < 2) {
				std::cerr << "Error: 演算に必要な数値が足りません。" << std::endl;
				return (false);
			}
			int b = st.top();
			st.pop();
			int a = st.top();
			st.pop();
			switch (target) {
				case '+':
					st.push(a + b);
					break;
				case '-':
					st.push(a - b);
					break;
				case '*':
					st.push(a * b);
					break;
				case '/':
					if (b == 0) {
						std::cerr << "Error: 0で割ることはできません。" << std::endl;
						return (false);
					}
					st.push(a / b);
					break;
			}
			continue;
		}
		std::cerr << "Error: 不正な引数です。" << std::endl;
		return (false);
	}

	if (st.size() != 1) {
		std::cerr << "Error: 式が不完全です。" << std::endl;
		return (false);
	}

	std::cout << st.top() << std::endl;
	return(true);
}
