/* Generated by re2c 0.13.5 */
// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "lexer.h"

#include <stdio.h>

#include "eval_env.h"
#include "util.h"

bool Lexer::Error(const string& message, string* err) {
  // Compute line/column.
  int line = 1;
  const char* context = input_.str_;
  for (const char* p = input_.str_; p < last_token_; ++p) {
    if (*p == '\n') {
      ++line;
      context = p + 1;
    }
  }
  int col = last_token_ ? (int)(last_token_ - context) : 0;

  char buf[1024];
  snprintf(buf, sizeof(buf), "%s:%d: ", filename_.AsString().c_str(), line);
  *err = buf;
  *err += message + "\n";

  // Add some context to the message.
  const int kTruncateColumn = 72;
  if (col > 0 && col < kTruncateColumn) {
    int len;
    bool truncated = true;
    for (len = 0; len < kTruncateColumn; ++len) {
      if (context[len] == 0 || context[len] == '\n') {
        truncated = false;
        break;
      }
    }
    *err += string(context, len);
    if (truncated)
      *err += "...";
    *err += "\n";
    *err += string(col, ' ');
    *err += "^ near here";
  }

  return false;
}

Lexer::Lexer(const char* input) {
  Start("input", input);
}

void Lexer::Start(StringPiece filename, StringPiece input) {
  filename_ = filename;
  input_ = input;
  ofs_ = input_.str_;
  last_token_ = NULL;
}

const char* Lexer::TokenName(Token t) {
  switch (t) {
  case ERROR:    return "lexing error";
  case BUILD:    return "'build'";
  case COLON:    return "':'";
  case DEFAULT:  return "'default'";
  case EQUALS:   return "'='";
  case IDENT:    return "identifier";
  case INCLUDE:  return "'include'";
  case INDENT:   return "indent";
  case NEWLINE:  return "newline";
  case PIPE2:    return "'||'";
  case PIPE:     return "'|'";
  case POOL:     return "'pool'";
  case RULE:     return "'rule'";
  case SUBNINJA: return "'subninja'";
  case TEOF:     return "eof";
  }
  return NULL;  // not reached
}

const char* Lexer::TokenErrorHint(Token expected) {
  switch (expected) {
  case COLON:
    return " ($ also escapes ':')";
  default:
    return "";
  }
}

string Lexer::DescribeLastError() {
  if (last_token_) {
    switch (last_token_[0]) {
    case '\r':
      return "carriage returns are not allowed, use newlines";
    case '\t':
      return "tabs are not allowed, use spaces";
    }
  }
  return "lexing error";
}

void Lexer::UnreadToken() {
  ofs_ = last_token_;
}

Lexer::Token Lexer::ReadToken() {
  const char* p = ofs_;
  const char* q;
  const char* start;
  Lexer::Token token;
  for (;;) {
    start = p;
    
{
	unsigned char yych;
	unsigned int yyaccept = 0;
	static const unsigned char yybm[] = {
		  0,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,   0,  64,  64,   0,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		192,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  96,  96,  64, 
		 96,  96,  96,  96,  96,  96,  96,  96, 
		 96,  96,  64,  64,  64,  64,  64,  64, 
		 64,  96,  96,  96,  96,  96,  96,  96, 
		 96,  96,  96,  96,  96,  96,  96,  96, 
		 96,  96,  96,  96,  96,  96,  96,  96, 
		 96,  96,  96,  64,  64,  64,  64,  96, 
		 64,  96,  96,  96,  96,  96,  96,  96, 
		 96,  96,  96,  96,  96,  96,  96,  96, 
		 96,  96,  96,  96,  96,  96,  96,  96, 
		 96,  96,  96,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
		 64,  64,  64,  64,  64,  64,  64,  64, 
	};

	yych = *p;
	if (yych <= '^') {
		if (yych <= ',') {
			if (yych <= 0x1F) {
				if (yych <= 0x00) goto yy22;
				if (yych == '\n') goto yy6;
				goto yy24;
			} else {
				if (yych <= ' ') goto yy2;
				if (yych == '#') goto yy4;
				goto yy24;
			}
		} else {
			if (yych <= ':') {
				if (yych == '/') goto yy24;
				if (yych <= '9') goto yy21;
				goto yy15;
			} else {
				if (yych <= '=') {
					if (yych <= '<') goto yy24;
					goto yy13;
				} else {
					if (yych <= '@') goto yy24;
					if (yych <= 'Z') goto yy21;
					goto yy24;
				}
			}
		}
	} else {
		if (yych <= 'i') {
			if (yych <= 'b') {
				if (yych == '`') goto yy24;
				if (yych <= 'a') goto yy21;
				goto yy8;
			} else {
				if (yych == 'd') goto yy12;
				if (yych <= 'h') goto yy21;
				goto yy19;
			}
		} else {
			if (yych <= 'r') {
				if (yych == 'p') goto yy10;
				if (yych <= 'q') goto yy21;
				goto yy11;
			} else {
				if (yych <= 'z') {
					if (yych <= 's') goto yy20;
					goto yy21;
				} else {
					if (yych == '|') goto yy17;
					goto yy24;
				}
			}
		}
	}
yy2:
	yyaccept = 0;
	yych = *(q = ++p);
	goto yy70;
yy3:
	{ token = INDENT;   break; }
yy4:
	yyaccept = 1;
	yych = *(q = ++p);
	if (yych <= 0x00) goto yy5;
	if (yych != '\r') goto yy65;
yy5:
	{ token = ERROR;    break; }
yy6:
	++p;
yy7:
	{ token = NEWLINE;  break; }
yy8:
	++p;
	if ((yych = *p) == 'u') goto yy59;
	goto yy26;
yy9:
	{ token = IDENT;    break; }
yy10:
	yych = *++p;
	if (yych == 'o') goto yy55;
	goto yy26;
yy11:
	yych = *++p;
	if (yych == 'u') goto yy51;
	goto yy26;
yy12:
	yych = *++p;
	if (yych == 'e') goto yy44;
	goto yy26;
yy13:
	++p;
	{ token = EQUALS;   break; }
yy15:
	++p;
	{ token = COLON;    break; }
yy17:
	++p;
	if ((yych = *p) == '|') goto yy42;
	{ token = PIPE;     break; }
yy19:
	yych = *++p;
	if (yych == 'n') goto yy35;
	goto yy26;
yy20:
	yych = *++p;
	if (yych == 'u') goto yy27;
	goto yy26;
yy21:
	yych = *++p;
	goto yy26;
yy22:
	++p;
	{ token = TEOF;     break; }
yy24:
	yych = *++p;
	goto yy5;
yy25:
	++p;
	yych = *p;
yy26:
	if (yybm[0+yych] & 32) {
		goto yy25;
	}
	goto yy9;
yy27:
	yych = *++p;
	if (yych != 'b') goto yy26;
	yych = *++p;
	if (yych != 'n') goto yy26;
	yych = *++p;
	if (yych != 'i') goto yy26;
	yych = *++p;
	if (yych != 'n') goto yy26;
	yych = *++p;
	if (yych != 'j') goto yy26;
	yych = *++p;
	if (yych != 'a') goto yy26;
	++p;
	if (yybm[0+(yych = *p)] & 32) {
		goto yy25;
	}
	{ token = SUBNINJA; break; }
yy35:
	yych = *++p;
	if (yych != 'c') goto yy26;
	yych = *++p;
	if (yych != 'l') goto yy26;
	yych = *++p;
	if (yych != 'u') goto yy26;
	yych = *++p;
	if (yych != 'd') goto yy26;
	yych = *++p;
	if (yych != 'e') goto yy26;
	++p;
	if (yybm[0+(yych = *p)] & 32) {
		goto yy25;
	}
	{ token = INCLUDE;  break; }
yy42:
	++p;
	{ token = PIPE2;    break; }
yy44:
	yych = *++p;
	if (yych != 'f') goto yy26;
	yych = *++p;
	if (yych != 'a') goto yy26;
	yych = *++p;
	if (yych != 'u') goto yy26;
	yych = *++p;
	if (yych != 'l') goto yy26;
	yych = *++p;
	if (yych != 't') goto yy26;
	++p;
	if (yybm[0+(yych = *p)] & 32) {
		goto yy25;
	}
	{ token = DEFAULT;  break; }
yy51:
	yych = *++p;
	if (yych != 'l') goto yy26;
	yych = *++p;
	if (yych != 'e') goto yy26;
	++p;
	if (yybm[0+(yych = *p)] & 32) {
		goto yy25;
	}
	{ token = RULE;     break; }
yy55:
	yych = *++p;
	if (yych != 'o') goto yy26;
	yych = *++p;
	if (yych != 'l') goto yy26;
	++p;
	if (yybm[0+(yych = *p)] & 32) {
		goto yy25;
	}
	{ token = POOL;     break; }
yy59:
	yych = *++p;
	if (yych != 'i') goto yy26;
	yych = *++p;
	if (yych != 'l') goto yy26;
	yych = *++p;
	if (yych != 'd') goto yy26;
	++p;
	if (yybm[0+(yych = *p)] & 32) {
		goto yy25;
	}
	{ token = BUILD;    break; }
yy64:
	++p;
	yych = *p;
yy65:
	if (yybm[0+yych] & 64) {
		goto yy64;
	}
	if (yych <= 0x00) goto yy66;
	if (yych <= '\f') goto yy67;
yy66:
	p = q;
	if (yyaccept <= 0) {
		goto yy3;
	} else {
		goto yy5;
	}
yy67:
	++p;
	{ continue; }
yy69:
	yyaccept = 0;
	q = ++p;
	yych = *p;
yy70:
	if (yybm[0+yych] & 128) {
		goto yy69;
	}
	if (yych == '\n') goto yy71;
	if (yych == '#') goto yy64;
	goto yy3;
yy71:
	++p;
	yych = *p;
	goto yy7;
}

  }

  last_token_ = start;
  ofs_ = p;
  if (token != NEWLINE && token != TEOF)
    EatWhitespace();
  return token;
}

bool Lexer::PeekToken(Token token) {
  Token t = ReadToken();
  if (t == token)
    return true;
  UnreadToken();
  return false;
}

void Lexer::EatWhitespace() {
  const char* p = ofs_;
  for (;;) {
    ofs_ = p;
    
{
	unsigned char yych;
	static const unsigned char yybm[] = {
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		128,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
	};
	yych = *p;
	if (yych <= ' ') {
		if (yych <= 0x00) goto yy78;
		if (yych <= 0x1F) goto yy80;
	} else {
		if (yych == '$') goto yy76;
		goto yy80;
	}
	++p;
	yych = *p;
	goto yy84;
yy75:
	{ continue; }
yy76:
	++p;
	if ((yych = *p) == '\n') goto yy81;
yy77:
	{ break; }
yy78:
	++p;
	{ break; }
yy80:
	yych = *++p;
	goto yy77;
yy81:
	++p;
	{ continue; }
yy83:
	++p;
	yych = *p;
yy84:
	if (yybm[0+yych] & 128) {
		goto yy83;
	}
	goto yy75;
}

  }
}

bool Lexer::ReadIdent(string* out) {
  const char* p = ofs_;
  for (;;) {
    const char* start = p;
    
{
	unsigned char yych;
	static const unsigned char yybm[] = {
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0, 128, 128,   0, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128,   0,   0,   0,   0,   0,   0, 
		  0, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128,   0,   0,   0,   0, 128, 
		  0, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
	};
	yych = *p;
	if (yych <= '@') {
		if (yych <= '.') {
			if (yych <= ',') goto yy89;
		} else {
			if (yych <= '/') goto yy89;
			if (yych >= ':') goto yy89;
		}
	} else {
		if (yych <= '_') {
			if (yych <= 'Z') goto yy87;
			if (yych <= '^') goto yy89;
		} else {
			if (yych <= '`') goto yy89;
			if (yych >= '{') goto yy89;
		}
	}
yy87:
	++p;
	yych = *p;
	goto yy92;
yy88:
	{
      out->assign(start, p - start);
      break;
    }
yy89:
	++p;
	{ return false; }
yy91:
	++p;
	yych = *p;
yy92:
	if (yybm[0+yych] & 128) {
		goto yy91;
	}
	goto yy88;
}

  }
  ofs_ = p;
  EatWhitespace();
  return true;
}

bool Lexer::ReadEvalString(EvalString* eval, bool path, string* err) {
  const char* p = ofs_;
  const char* q;
  const char* start;
  for (;;) {
    start = p;
    
{
	unsigned char yych;
	static const unsigned char yybm[] = {
		  0, 128, 128, 128, 128, 128, 128, 128, 
		128, 128,   0, 128, 128,   0, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		 16, 128, 128, 128,   0, 128, 128, 128, 
		128, 128, 128, 128, 128, 224, 160, 128, 
		224, 224, 224, 224, 224, 224, 224, 224, 
		224, 224,   0, 128, 128, 128, 128, 128, 
		128, 224, 224, 224, 224, 224, 224, 224, 
		224, 224, 224, 224, 224, 224, 224, 224, 
		224, 224, 224, 224, 224, 224, 224, 224, 
		224, 224, 224, 128, 128, 128, 128, 224, 
		128, 224, 224, 224, 224, 224, 224, 224, 
		224, 224, 224, 224, 224, 224, 224, 224, 
		224, 224, 224, 224, 224, 224, 224, 224, 
		224, 224, 224, 128,   0, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
	};
	yych = *p;
	if (yych <= ' ') {
		if (yych <= '\n') {
			if (yych <= 0x00) goto yy101;
			if (yych >= '\n') goto yy97;
		} else {
			if (yych == '\r') goto yy103;
			if (yych >= ' ') goto yy97;
		}
	} else {
		if (yych <= '9') {
			if (yych == '$') goto yy99;
		} else {
			if (yych <= ':') goto yy97;
			if (yych == '|') goto yy97;
		}
	}
	++p;
	yych = *p;
	goto yy126;
yy96:
	{
      eval->AddText(StringPiece(start, p - start));
      continue;
    }
yy97:
	++p;
	{
      if (path) {
        p = start;
        break;
      } else {
        if (*start == '\n')
          break;
        eval->AddText(StringPiece(start, 1));
        continue;
      }
    }
yy99:
	++p;
	if ((yych = *p) <= '/') {
		if (yych <= ' ') {
			if (yych == '\n') goto yy115;
			if (yych <= 0x1F) goto yy104;
			goto yy106;
		} else {
			if (yych <= '$') {
				if (yych <= '#') goto yy104;
				goto yy108;
			} else {
				if (yych == '-') goto yy110;
				goto yy104;
			}
		}
	} else {
		if (yych <= '^') {
			if (yych <= ':') {
				if (yych <= '9') goto yy110;
				goto yy112;
			} else {
				if (yych <= '@') goto yy104;
				if (yych <= 'Z') goto yy110;
				goto yy104;
			}
		} else {
			if (yych <= '`') {
				if (yych <= '_') goto yy110;
				goto yy104;
			} else {
				if (yych <= 'z') goto yy110;
				if (yych <= '{') goto yy114;
				goto yy104;
			}
		}
	}
yy100:
	{
      last_token_ = start;
      return Error(DescribeLastError(), err);
    }
yy101:
	++p;
	{
      last_token_ = start;
      return Error("unexpected EOF", err);
    }
yy103:
	yych = *++p;
	goto yy100;
yy104:
	++p;
yy105:
	{
      last_token_ = start;
      return Error("bad $-escape (literal $ must be written as $$)", err);
    }
yy106:
	++p;
	{
      eval->AddText(StringPiece(" ", 1));
      continue;
    }
yy108:
	++p;
	{
      eval->AddText(StringPiece("$", 1));
      continue;
    }
yy110:
	++p;
	yych = *p;
	goto yy124;
yy111:
	{
      eval->AddSpecial(StringPiece(start + 1, p - start - 1));
      continue;
    }
yy112:
	++p;
	{
      eval->AddText(StringPiece(":", 1));
      continue;
    }
yy114:
	yych = *(q = ++p);
	if (yybm[0+yych] & 32) {
		goto yy118;
	}
	goto yy105;
yy115:
	++p;
	yych = *p;
	if (yybm[0+yych] & 16) {
		goto yy115;
	}
	{
      continue;
    }
yy118:
	++p;
	yych = *p;
	if (yybm[0+yych] & 32) {
		goto yy118;
	}
	if (yych == '}') goto yy121;
	p = q;
	goto yy105;
yy121:
	++p;
	{
      eval->AddSpecial(StringPiece(start + 2, p - start - 3));
      continue;
    }
yy123:
	++p;
	yych = *p;
yy124:
	if (yybm[0+yych] & 64) {
		goto yy123;
	}
	goto yy111;
yy125:
	++p;
	yych = *p;
yy126:
	if (yybm[0+yych] & 128) {
		goto yy125;
	}
	goto yy96;
}

  }
  last_token_ = start;
  ofs_ = p;
  if (path)
    EatWhitespace();
  // Non-path strings end in newlines, so there's no whitespace to eat.
  return true;
}
