#include <iostream>
#include <vector>
#include <string>
#include <cctype>

struct token {
	std::string type;
	std::string value;
};

struct node {
	std::string type;
	std::string value;
	std::vector<node> params;
};

struct ast {
	std::string type = "program";
	std::vector<node> body;
};

node walk(int &cursor, std::vector<token> &tokens) {
	token cur_token = tokens[cursor];
	if (cur_token.type == "number") {
		++cursor;
		return {"numberLiteral", cur_token.value};
	}

	if (cur_token.type == "string") {
		++cursor;
		return {"stringLiteral", cur_token.value};
	}

	if (cur_token.type == "parenthesis" &&
		cur_token.value == "("
	) {
		++cursor;
		cur_token = tokens[cursor];
		node callExpression = {"callExpression", cur_token.value};
		++cursor;
		cur_token = tokens[cursor];

		while (cur_token.type != "parenthesis" || cur_token.type == "parenthesis" && cur_token.value != ")") {
			callExpression.params.push_back(walk(cursor, tokens));
			cur_token = tokens[cursor];
		}

		++cursor;
		return callExpression;
	}
}

ast parser(std::vector<token> tokens) {
	ast ast;
	int cursor = 0;

	while (cursor < tokens.size()) {
		ast.body.push_back(walk(cursor, tokens));
	}

	return ast;
}

std::vector<token> tokeniser(std::string input) {
	std::vector<token> tokens;

	for (int cursor = 0; cursor < input.size(); ++cursor) {
		char cur_char = input[cursor];
		switch(cur_char) {
			case '(':{
				tokens.push_back({"parenthesis", "("});
				break;
			}
			case ')': {
				tokens.push_back({"parenthesis", ")"});
				break;
			}
			case ' ': {
				break;
			}
		}

		if (cur_char == '"') {
			std::string letters = "";
			++cursor;
			cur_char = input[cursor];
			while (isalpha(cur_char)) {
				letters.push_back(cur_char);
				++cursor;
				cur_char = input[cursor];
			}

			tokens.push_back({"string", letters});
			continue;
		}

		if (isalpha(cur_char)) {
			std::string letters = "";
			while (isalpha(cur_char)) {
				letters.push_back(cur_char);
				++cursor;
				cur_char = input[cursor];
			}
			--cursor;

			tokens.push_back({"word", letters});
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

			tokens.push_back({"number", numbers});
			continue;
		}
	}
	return tokens;
}

int main(void) {
	std::vector<token> tokens = tokeniser("\"test\" (add 2 (subtract 14 2))");
	
	for (token i: tokens) {
//		std::cout << '"' << i.value << "\" ";
//		std::cout << i.type << std::endl;
	}
}
