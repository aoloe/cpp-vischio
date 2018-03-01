#ifndef VISCHIO_DOCUMENT_H
#define VISCHIO_DOCUMENT_H

#include "blocktoken.h"
#include "filetokenizer.h"
#include "blocktokenizer.h"

#include <string>
#include <vector>
#include <iterator>

namespace Vischio {
    class Document
    {
        public:
            Document(std::ifstream& markdownFile) {
                auto lines = Tokenizer::File::getLines(markdownFile);
                lines = Tokenizer::File::getNormalized(lines);
                Tokenizer::Block tokenizer;
                children = tokenizer.getTokens(lines);
            }
            const auto& getChildren()
            {
                return children;
            }
        protected:
            std::vector<std::shared_ptr<Token::Block>> children{};
    };
}


#endif // VISCHIO_DOCUMENT_H
