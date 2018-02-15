#ifndef VISCHIO_BLOCKTOKEN_H
#define VISCHIO_BLOCKTOKEN_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <optional>
#include <algorithm>


/**
 *
 * Base class for span-level tokens. Recursively parse inner tokens.
 * 
 * Naming conventions:
 *
 * - lines denotes a list of (possibly unparsed) input lines, and is
 *   commonly used as the argument name for constructors.
 * - self.children is a tuple with all the inner tokens (thus if a
 *   token has children attribute, it is not a leaf node; if a token
 *   calls span_token.tokenize_inner, it is the boundary between
 *   span-level tokens and block-level tokens);
 * - match is a static method used by tokenize to check if line_buffer
 *   matches the current token. Every subclass of BlockToken must
 *   define a match function (see block_tokenizer.tokenize).
*/

namespace Vischio {
namespace Token {
    class Block
    {
        public:
            Block(std::string type) : type{type} {}
            virtual std::optional<std::shared_ptr<Block>> factory(std::vector<std::string>::iterator) const
            {
                return {};
            }
            std::vector<std::string> read() {
                return {};
            }
            virtual std::string getType() const {return type;}
        protected:
            /**
             * Actual children attribute.
             *
             * If self.children is never accessed, the generator is never actually
             * run. This allows for lazy-parsing of the input, while still maintaining
             * idempotent behavior for tokens.
             */
            std::vector<std::string>::iterator it;
            std::vector<std::shared_ptr<Block>> children{};
            std::string type{"Block"}; // TODO: can a variable be virtual?
    };

    class Paragraph : public Block
    {
        public:
            Paragraph() : Block("Paragraph") {}
            Paragraph(std::vector<std::string> data) : Block("Paragraph"), data{data} {}
            std::optional<std::shared_ptr<Block>> factory(std::vector<std::string>::iterator it) const override
            {
                if (it->empty() || *it == "\n") {
                    return {};
                }
                // TODO: peek to \n
                std::vector<std::string> lines = {*it};
                // Paragraph paragraph{lines};
                return {std::make_shared<Paragraph>(lines)};
            }

            auto getLines() { return data; }

        private:
            std::vector<std::string> data;
    };

    class Heading : public Block
    {
        public:
            struct HeadingData
            {
                int level;
                std::string title;
            };
            
            Heading() : Block("Heading") {};
            Heading(int level, std::string title) : Block("Heading"), data{level, title} {}
            Heading(HeadingData data) : Block("Heading"), data{data} {}
            std::optional<std::shared_ptr<Block>> factory(std::vector<std::string>::iterator it) const override
            {
                if (auto heading = getLevelAndTitle(*it)) {
                    return {std::make_shared<Heading>((*heading).level, (*heading).title)};
                }
                return {};
            }
            int getLevel() { return data.level; }
            std::string getTitle() { return data.title; }

        private:
            HeadingData data;

            /**
             * starts with a # and the last consecutive # is followed by non-spaces
             */

            std::optional<HeadingData> getLevelAndTitle(std::string line) const
            {
                try {
                    if (line.at(0) != '#') {
                        return {};
                    }
                }
                catch (std::out_of_range const& e) {
                    return {};
                }
                // https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
                auto lastHash = std::find_if(line.begin(), line.end(), [](int ch) {
                    return ch != '#';
                });
                int level = std::distance(line.begin(), lastHash);
                // ltrim #: delete from begin to the last #
                line.erase(line.begin(), lastHash);
                // rtrim
                line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
                    return !std::isspace(ch);
                }).base(), line.end());
                if (line.empty()) {
                    return {};
                }

                // ltrim
                line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
                    return !std::isspace(ch);
                }));

                return {{level, line}};
            }
    };

}
}

#endif // VISCHIO_BLOCKTOKEN_H
