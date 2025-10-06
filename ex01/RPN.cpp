#include"RPN.hpp"

/*
 * デフォルトコンストラクタ
 */
RPN::RPN() {}

/*
 * コピーコンストラクタ
 */
RPN::RPN(const RPN& copy) {
	_st = copy._st;
}

/*
 * コピー代入演算子 (A copy assignment operator overload.)
 */
RPN &RPN::operator=(const RPN& copy) {
	if (this != &copy)
	{
		_st = copy._st;
	}
	return (*this);
}

/*
 * デストラクタ
 */
RPN::~RPN() {}


// ↑↑↑ Orthodox Canonical Form --------------------------------------

void	RPN::calc_RPN(int size, char*	data) {
	std::stack<int> st;
	for (size_t i = 0; i < size; i++) {
		if ('0' <= data[i] && data[i] >= '9')
			st.push(data[i] - '0');
		if (data[i] == '+') {
			int	a = st.top();
			st.pop();
			int	b = st.top();
			st.pop();

			if (data[i] == '+')
				st.push(a + b);
			else if (data[i] == '-')
				st.push(a - b);
			else if (data[i] == '/')
				st.push(a / b);
			else if (data[i] == '*')
				st.push(a * b);
		}
		else {
			std::cerr << "Error" << std::endl;
			exit (EXIT_FAILURE);
		}
	}
}
