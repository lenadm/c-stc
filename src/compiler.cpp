#include <iostream>
#include <vector>
#include <string>
#include <cctype>

struct token {
	std::string type;
	std::string value;
};

std::vector<token> tokeniser(std::string input) {
	std::vector<token> tokens;

	for (int cursor = 0; cursor < input.size(); ++cursor) {
		char cur_char = input[cursor];
		switch(cur_char) {
			case '(':{
				token new_token = {"parenthesis", "("};
				tokens.push_back(new_token);
				break;
			}
			case ')': {
				token new_token = {"parenthesis", ")"};
				tokens.push_back(new_token);
				break;
			}
			case ' ': {
				break;
			}
		}

		if (isalpha(cur_char)) {
			std::string letters = "";
			while (isalpha(cur_char)) {
				letters.push_back(cur_char);
				++cursor;
				cur_char = input[cursor];
			}
			--cursor;

			token new_token = {"word", letters};
			tokens.push_back(new_token);
			continue;
		}


		if (isdigit(cur_char)) {
			std::string numbers = "";
			while (isdigit(cur_char)) {
				numbers.push_back(cur_char);
				++cursor;
				cur_char = input[cursor];
			}
			--cursor;

			token new_token = {"number", numbers};
			tokens.push_back(new_token);
			continue;
		}
	}
	return tokens;
}

int main(void) {
	std::vector<token> tokens = tokeniser("(add 2 (subtract 14 2))");
	
	for (token i: tokens) {
		std::cout << '"' << i.value << "\" ";
	}
}
