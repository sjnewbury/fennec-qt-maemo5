/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express oqr
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is the JavaScript 2 Prototype.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 *
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU Public License (the "GPL"), in which case the
 * provisions of the GPL are applicable instead of those above.
 * If you wish to allow use of your version of this file only
 * under the terms of the GPL and not to allow others to use your
 * version of this file under the NPL, indicate your decision by
 * deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL.  If you do not delete
 * the provisions above, a recipient may use your version of this
 * file under either the NPL or the GPL.
 */

#ifndef lexer_h___
#define lexer_h___

#include "utilities.h"
#include "formatter.h"
#include "reader.h"
#include "token.h"
#include "world.h"

namespace JavaScript 
{

    class Lexer {
        enum {tokenLookahead = 2};  // Number of tokens that can be
                                    // simultaneously live
#ifdef DEBUG
        enum {tokenGuard = 10};     // Number of invalid tokens added to
                                    // circular token buffer to catch
                                    // references to old tokens
#else
        enum {tokenGuard = 0};      // Number of invalid tokens added to
                                    // circular token buffer to catch
                                    // references to old tokens
#endif
        // Token lookahead buffer size
        enum {tokenBufferSize = tokenLookahead + tokenGuard};

        Token tokens[tokenBufferSize];  // Circular buffer of recently read or
                                        // lookahead tokens
        Token *nextToken;               // Address of next Token in the
                                        // circular buffer to be returned by
                                        // get()
        int nTokensFwd;                 // Net number of Tokens on which
                                        // unget() has been called; these
                                        // Tokens are ahead of nextToken
#ifdef DEBUG
        int nTokensBack;                // Number of Tokens on which unget()
                                        // can be called; these Tokens are
                                        // beind nextToken
        bool savedPreferRegExp[tokenBufferSize]; // Circular buffer of saved
                                                 // values of preferRegExp to
                                                 // get() calls
#endif
        bool lexingUnit;                // True if lexing a unit identifier
                                        // immediately following a number
      public:
        World &world;
        Reader reader;

        Lexer(World &world, const String &source, const String &sourceLocation,
              uint32 initialLineNum = 1);
        
        const Token &get(bool preferRegExp);
        const Token *eat(bool preferRegExp, Token::Kind kind);
        const Token &peek(bool preferRegExp);
        void redesignate(bool preferRegExp);
        void unget();
        uint32 getPos() const;
        
      private:
        void syntaxError(const char *message, uint backUp = 1);
        char16 getChar();
        char16 internalGetChar(char16 ch);
        char16 peekChar();
        char16 internalPeekChar(char16 ch);
        bool testChar(char16 ch);
        
        char16 lexEscape(bool unicodeOnly);
        bool lexIdentifier(String &s, bool allowLeadingDigit);
        bool lexNumeral();
        void lexString(String &s, char16 separator);
        void lexRegExp();
        void lexToken(bool preferRegExp);
    };

#ifndef DEBUG
    inline void Lexer::redesignate(bool) {}  // See description for the DEBUG
                                             // version inside parser.cpp
#endif

    // Return the position of the first character of the next token,
    // which must have been peeked.
    inline uint32
    Lexer::getPos() const
    {
        ASSERT(nTokensFwd);
        return nextToken->getPos();
    }
    
}

#endif /* lexer_h___ */
