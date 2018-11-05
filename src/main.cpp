/*
 * Copyright (c) 2018, Xianguang Zhou <xianguang.zhou@outlook.com>. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <fstream>
#include <iostream>
#include "Lexer.h"

using std::string;
using std::list;
using std::cout;
using std::cerr;
using std::endl;
using std::istream;
using std::istreambuf_iterator;
using boost::shared_ptr;
using Lcc::Lexer;

static shared_ptr<string> stdinToString() {
	istreambuf_iterator<char> streamBegin(std::cin);
	istreambuf_iterator<char> streamEnd;
	return shared_ptr<string>(new string(streamBegin, streamEnd));
}

static shared_ptr<string> fileToString(const char *fileName) {
	std::ifstream fileStream(fileName);
	if (!fileStream.is_open()) {
		cerr << "Failed to open the grammar file \"" << fileName << "\"."
				<< endl;
		exit(1);
	}
	istreambuf_iterator<char> streamBegin(fileStream);
	istreambuf_iterator<char> streamEnd;
	return shared_ptr<string>(new string(streamBegin, streamEnd));
}

static void printTokens(shared_ptr<list<shared_ptr<Lexer::Token>>> tokensPtr) {
	for (list<shared_ptr<Lexer::Token>>::const_iterator it = tokensPtr->begin();
			it != tokensPtr->end(); it++) {
		shared_ptr<Lexer::Token> tokenPtr = *it;
		cout << static_cast<int>(tokenPtr->type) << "\t" << tokenPtr->value
				<< endl;
	}
}

int main(int argc, char **argv) {
	shared_ptr<string> grammarTextPtr =
			argc > 1 ? fileToString(argv[1]) : stdinToString();

	Lexer lexer;
	shared_ptr<list<shared_ptr<Lexer::Token>>> tokensPtr;
	try {
		tokensPtr = lexer.generateTokens(*grammarTextPtr);
	} catch (const Lexer::LexerException& ex) {
		cerr << ex.what() << endl;
		return 1;
	}

	printTokens(tokensPtr);

	return 0;
}
