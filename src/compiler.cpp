#include <iostream>
#include <vector>
#include <string>

struct token {
	std::string type;
	std::string value;
};

std::vector<token> tokeniser(std::string input) {
	std::vector<token> tokens;

	for (int cursor = 0; input[cursor] != EOF; ++cursor) {
		char cur_char = input[cursor];
		switch(cur_char) {
			case '(':
				token new_token = {"parenthesis", "("};
				tokens.push_back(new_token);
				break;
		}
	}
	return tokens;
}

int main(void) {
	std::vector<token> tokens = tokeniser("(((");
	
	for (token i: tokens) {
		std::cout << '"' << i.value << "\" ";
	}
}
