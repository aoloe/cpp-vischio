#ifndef VISCHIO_BLOCKTOKENIZER_H
#define VISCHIO_BLOCKTOKENIZER_H

#include <string>
#include <vector>
#include <algorithm>
#include <memory> // make_shared
#include <utility> // move
#include <optional> // move
#include "blocktoken.h"

namespace Vischio {
namespace Tokenizer {
    using Lines = std::vector<std::string>;

    class Block
    {
        public:
            /*
            void add(std::shared_ptr<Token::Block> block)
            {
                tokenizers.push_back(block);
            }
            // if we want to be able to remove blocks, we have to use a map instead of the vector.
            void remove(std::string blockName)
            {
            }
            */

            Block()
            {
                tokenizers.emplace_back(std::make_shared<Token::Heading>());
                tokenizers.emplace_back(std::make_shared<Token::Paragraph>());
            }

            Token::Blocks getTokens(Lines lines)
            {
                Token::Blocks result{};
                for(auto it = lines.begin(); it != lines.end(); ++it) {
                    for (const auto& tokenizer: tokenizers) {
                        if (auto token = tokenizer->factory(it)) {
                            (*token)->tokenize(inlineTokenizer);
                            result.push_back(*token);
                            break;
                        }
                    }
                }
                return result;
            }

            Token::Blocks tokenizers{};
            Inline inlineTokenizer{};
    };
}
}

#endif // VISCHIO_BLOCKTOKENIZER_H
