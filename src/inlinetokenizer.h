#ifndef VISCHIO_INLINETOKENIZER_H
#define VISCHIO_INLINETOKENIZER_H

#include <string>
#include <vector>
#include <memory>

#include "inlinetoken.h"

namespace Vischio {
namespace Tokenizer {
    class Inline
    {
        public:
            Inline()
            {
                tokenizers.emplace_back(std::make_shared<Token::Emphasis>());
            }

            Token::Inlines getTokens(const std::vector<std::string> lines) const
            {
                Token::Inlines result{};
                for(auto line: lines) {
                    for (auto it = line.begin(); it < line.end();) {
                        auto minIt = line.end();
                        std::shared_ptr<Token::Inline> minToken = nullptr;
                        for (const auto& tokenizer: tokenizers) {
                            if (auto token = tokenizer->factory(it, line.end())) {
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

            Token::Inlines tokenizers{};
    };


}
}

#endif // VISCHIO_INLINETOKENIZER_H
