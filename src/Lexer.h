/*
 * Copyright (c) 2018, Xianguang Zhou <xianguang.zhou@outlook.com>. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LCC_LEXER_H
#define LCC_LEXER_H

#include <stdexcept>
#include <string>
#include <list>
#include <boost/smart_ptr.hpp>
#include <boost/regex.hpp>

namespace Lcc {

using std::string;
using std::list;
using boost::shared_ptr;
using boost::regex;

class Lexer {
public:
	struct Token {
		enum struct Type {
			PATTERN, // "..."
			IDENTIFIER,
			COLON, // :
			SEMICOLON, // ;
			OR, // |
			ZERO_OR_MORE, // *
			ONE_OR_MORE, // +
			ZERO_OR_ONE, // ?
			LEFT_PARE, // (
			RIGHT_PARE, // )
		};
		Type type;
		string value;
		Token(Type type, const string& value) :
				type(type), value(value) {
		}
		Token(int type, const string& value) :
				type(static_cast<Token::Type>(type)), value(value) {
		}
	};

	class LexerException: public std::logic_error {
	public:
		explicit LexerException(const string& message) :
				std::logic_error(message) {
		}
	};

	Lexer();
	shared_ptr<list<shared_ptr<Token>>> generateTokens(const string& text)
			throw (LexerException);

private:
	size_t tokenTypesSize;
	list<regex> tokenRegexes;
	int matchedToken(const string& text);
	bool isMatchedTokenRegex(int tokenRegexIndex) {
		return tokenRegexIndex >= 0;
	}
	bool isMatchedTokenType(int tokenRegexIndex) {
		return tokenRegexIndex < tokenTypesSize;
	}
};
}

#endif
