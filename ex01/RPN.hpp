#ifndef RPN_HPP
# define RPN_HPP

// ------------------------------------------------
// include
// ------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ------------------------------------------------
// class
// ------------------------------------------------

class RPN {
	public:
		RPN();
		RPN(const unsigned int n);
		RPN& operator=(const RPN& copy);
		RPN(const RPN& copy);
		~RPN();

		void addNumber(int value);
		void addNumber(int pos, const std::vector<int>& values);
		static	void	calc_RPN(int size, char*	data);

	private:
		std::stack<int> _st;
};

// ------------------------------------------------
// function
// ------------------------------------------------

#endif
