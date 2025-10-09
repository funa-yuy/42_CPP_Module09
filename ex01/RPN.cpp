#include"RPN.hpp"
#include <stack>
#include <sstream>
#include <vector>

static std::vector<std::string> splitBySpace(const std::string& s)
{
	std::vector<std::string> tokens;
	std::istringstream iss(s);
	std::string token;

	while (iss >> token)
		tokens.push_back(token);
	return tokens;
}

bool	RPN::execute(const char*	data) {
	std::stack<int> st;
	std::vector<std::string> tokens = splitBySpace(std::string(data));

	for (size_t i = 0; i < tokens.size(); i++) {
		std::cout << "[DEBUG]" << tokens[i] << std::endl;
		if (tokens[i].size() != 1) {
			std::cerr << "Error" << std::endl;
			return (false);
		}

		// 1文字の数字（0-9）なら push
		if (tokens[i][0] >= '0' && tokens[i][0] <= '9') {
			st.push(tokens[i][0] - '0');
			continue ;
		}

		// 演算子なら pop 2回して計算
		if (tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "/" || tokens[i] == "*") {
			if (st.size() < 2) {
				std::cerr << "Error" << std::endl;
				return (false);
			}
			int b = st.top();
			st.pop();
			int a = st.top();
			st.pop();

			if (tokens[i] == "+")
				st.push(a + b);
			else if (tokens[i] == "-")
				st.push(a - b);
			else if (tokens[i] == "/") {
				if (b == 0) {
					std::cerr << "Error" << std::endl;
					return (false);
				}
				st.push(a / b);
			}
			else if (tokens[i] == "*")
				st.push(a * b);
		}
		else {
			std::cerr << "Error" << std::endl;
			return (false);
		}
	}
	std::cout << st.top() << std::endl;
	return(true);
}
