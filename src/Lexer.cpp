/*
 * Copyright (c) 2018, Xianguang Zhou <xianguang.zhou@outlook.com>. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/format.hpp>
#include "Lexer.h"

namespace Lcc {

Lexer::Lexer() {
	list<string> tokenPatterns;
	tokenPatterns.push_back(
			"\"((\\\\\")|(\\\\\\\\)|[^\"(\\\\)\r\n]|\\(|\\)|(\\\\[a-zA-Z0']))+\"");
	tokenPatterns.push_back("[a-zA-Z][a-zA-Z0-9_]*");
	tokenPatterns.push_back(":");
	tokenPatterns.push_back(";");
	tokenPatterns.push_back("\\|");
	tokenPatterns.push_back("\\*");
	tokenPatterns.push_back("\\+");
	tokenPatterns.push_back("\\?");
	tokenPatterns.push_back("\\(");
	tokenPatterns.push_back("\\)");
	this->tokenTypesSize = tokenPatterns.size();
	tokenPatterns.push_back("[\\s\t\r\n]+|(//[^\r\n]*)");

	for (list<string>::const_iterator it = tokenPatterns.begin();
			it != tokenPatterns.end(); it++) {
		this->tokenRegexes.push_back(regex(*it));
	}
}

shared_ptr<list<shared_ptr<Lexer::Token>>> Lexer::generateTokens(
		const string& text) throw (LexerException) {
	shared_ptr<list<shared_ptr<Token>>> tokensPtr(
			new list<shared_ptr<Token>>());

	int line = 1, column = 1;
	int unmatchedLine, unmatchedColumn;
	string buffer;
	int lastTokenRegexIndex = this->tokenTypesSize;
	for (string::const_iterator textIt = text.begin(); textIt != text.end();
			textIt++) {
		char character = *textIt;
		buffer.append(1, character);

		int tokenRegexIndex = this->matchedToken(buffer);
		if (!isMatchedTokenRegex(tokenRegexIndex)
				&& isMatchedTokenRegex(lastTokenRegexIndex)) {
			if (isMatchedTokenType(lastTokenRegexIndex)) {
				buffer.pop_back();
				tokensPtr->push_back(
						shared_ptr<Token>(
								new Token(lastTokenRegexIndex, buffer)));
			}
			buffer = character;
			tokenRegexIndex = this->matchedToken(buffer);
			unmatchedLine = line;
			unmatchedColumn = column;
		}

		lastTokenRegexIndex = tokenRegexIndex;

		if (character == '\n') {
			++line;
			column = 1;
		} else {
			++column;
		}
	}

	if (isMatchedTokenRegex(lastTokenRegexIndex)) {
		if (isMatchedTokenType(lastTokenRegexIndex)) {
			tokensPtr->push_back(
					shared_ptr<Token>(new Token(lastTokenRegexIndex, buffer)));
		}
	} else {
		throw LexerException(
				(boost::format("Unknown token at line %1% and column %2%.")
						% unmatchedLine % unmatchedColumn).str());
	}

	return tokensPtr;
}

int Lexer::matchedToken(const string& text) {
	int tokenRegexIndex = 0;
	for (list<regex>::const_iterator regexIt = this->tokenRegexes.begin();
			regexIt != this->tokenRegexes.end(); regexIt++, tokenRegexIndex++) {
		if (boost::regex_match(text, *regexIt)) {
			return tokenRegexIndex;
		}
	}
	return -1;
}
}
