#include <iostream>
#include <format>
#include <vector>
#include <string>
#include <cctype>
#include <stdexcept>

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

	throw std::invalid_argument(std::string("Unkown token at character ") + std::to_string(cursor));
}

ast parser(std::vector<token> tokens) {
	ast ast;
	int cursor = 0;

	while (cursor < tokens.size()) {
		try {
			ast.body.push_back(walk(cursor, tokens));
		} catch(const std::exception &exception) {
			std::cout << exception.what() << std::endl;
			++cursor;
			continue;
		}
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

void walk_print(node node, int layer) {
	for (int i = 0; i < node.params.size(); ++i) {
		for (int j = 0; j < layer; j++) {
			std::cout << "  ";
		}

		std::cout << node.params[i].type << std::endl;
		if (node.params[i].type == "callExpression") {
			walk_print(node.params[i], layer + 1);
		}
	}
}

void print(ast ast) {
	for (int i = 0; i < ast.body.size(); ++i) {
		std::cout << ast.body[i].type << std::endl;
		if (ast.body[i].type == "callExpression") {
	  		walk_print(ast.body[i], 1);
	  	}
	}
}


int main(void) {
	std::vector<token> tokens = tokeniser("(add 2 (subtract 14 2))");
	ast ast = parser(tokens);
	print(ast);
}
