#ifndef VISCHIO_BLOCKTOKENIZER_H
#define VISCHIO_BLOCKTOKENIZER_H

#include <string>
#include <vector>
#include <algorithm>
#include <memory> // make_shared
#include <utility> // move
#include <optional>
#include <functional>
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
                factories.push_back(block);
            }
            // if we want to be able to remove blocks, we have to use a map instead of the vector.
            void remove(std::string blockName)
            {
            }
            */

            Block() :
                factories{
                    // For block factories, the order is relevant
                    &Token::Heading::factory,
                    &Token::Paragraph::factory
                }
            {}

            Token::Blocks getTokens(Lines lines)
            {
                Token::Blocks result{};
                for(auto it = lines.begin(); it != lines.end(); ++it) {
                    for (auto factory: factories) {
                        if (auto token = factory(it)) {
                            (*token)->tokenize(inlineTokenizer);
                            result.push_back(*token);
                            break;
                        }
                    }
                }
                return result;
            }

            std::vector<std::function<Token::OptionalBlock(std::vector<std::string>::iterator)>> factories{};
            Inline inlineTokenizer{};
    };
}
}

#endif // VISCHIO_BLOCKTOKENIZER_H
