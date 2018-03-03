#ifndef VISCHIO_INLINETOKENIZER_H
#define VISCHIO_INLINETOKENIZER_H

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "inlinetoken.h"

namespace Vischio {
namespace Tokenizer {
    class Inline
    {
        public:
            Inline() :
                factories{
                    &Token::Emphasis::factory
                }
            {}

            Token::Inlines getTokens(const std::vector<std::string> lines) const
            {
                Token::Inlines result{};
                for(auto line: lines) {
                    for (auto it = line.begin(); it < line.end();) {
                        auto minIt = line.end();
                        std::shared_ptr<Token::Inline> minToken = nullptr;
                        for (auto factory: factories) {
                            if (auto token = factory(it, line.end())) {
                                if ((*token)->getIteratorStartMatch() < minIt) {
                                    minIt = (*token)->getIteratorStartMatch();
                                    minToken = *token;
                                }
                            }
                        }

                        if (minIt > it) {
                            // create a rawtext with the beginning

                            result.push_back(std::make_shared<Token::RawText>(std::string(it, minIt)));
                        }

                        if (minToken != nullptr) {
                            minToken->tokenize();
                            result.push_back(minToken);
                            it = minToken->getIteratorEndMatch();
                        } else {
                            it = minIt + 1;
                        }
                    }

                }

                return result;
            }

            std::vector<std::function<Token::OptionalInline(std::string::iterator start, std::string::iterator end)>> factories{};

    };

}
}

#endif // VISCHIO_INLINETOKENIZER_H
