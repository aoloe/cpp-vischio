#ifndef VISCHIO_HTMLRENDERER_H
#define VISCHIO_HTMLRENDERER_H

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <memory>

#include "blocktoken.h"
#include "inlinetoken.h"
#include "string.h"

namespace Vischio {
namespace Renderer {
    class Html
    {
        public:
            Html()
            {
                // blockRenderers["Heading"] = [=](const auto& block){return this->heading(block);};
                // blockRenderers["Paragraph"] = [=](const auto& block){return this->paragraph(block);};
                blockRenderers["Heading"] = [=](std::shared_ptr<Token::Block> block) -> std::string {
                    return this->heading(std::dynamic_pointer_cast<Token::Heading>(block));
                };
                blockRenderers["Paragraph"] = [=](std::shared_ptr<Token::Block> block) -> std::string {
                    return this->paragraph(std::dynamic_pointer_cast<Token::Paragraph>(block));
                };

                inlineRenderers["RawText"] = [=](std::shared_ptr<Token::Inline> inlineElement) -> std::string {
                    return this->rawText(std::dynamic_pointer_cast<Token::RawText>(inlineElement));
                };
                inlineRenderers["Emphasis"] = [=](std::shared_ptr<Token::Inline> inlineElement) -> std::string {
                    return this->emphasis(std::dynamic_pointer_cast<Token::Emphasis>(inlineElement));
                };

            }
            std::string render(Document document)
            {
                return render(document.getChildren());
            }

        protected:
            std::map<std::string, std::function<std::string(std::shared_ptr<Token::Block>)>> blockRenderers;
            std::map<std::string, std::function<std::string(std::shared_ptr<Token::Inline>)>> inlineRenderers;

            std::string render(Token::Blocks blocks)
            {
                std::vector<std::string> lines{};
                for (auto const& block: blocks) {
                    lines.push_back(blockRenderers.at(block->getType())(block));
                }
                return ::Vischio::String::join(lines.begin(), lines.end(), "\n");
            }

            std::string render(Token::Inlines items)
            {
                std::vector<std::string> block;
                for (auto const& item: items) {
                    block.push_back(inlineRenderers.at(item->getType())(item));
                }
                return ::Vischio::String::join(block.begin(), block.end(), "");
            }

            std::string heading(std::shared_ptr<Token::Heading> block)
            {
                auto level = block->getLevel();
                return "<h" + std::to_string(level) + ">" + render(block->getChildren()) + "</h" + std::to_string(level) + ">";
            }

            std::string paragraph(std::shared_ptr<Token::Paragraph> block)
            {

                return "<p>" + render(block->getChildren()) + "</p>";
            }

            std::string emphasis(std::shared_ptr<Token::Emphasis> block)
            {
                return "<em>" + render(block->getChildren()) + "</em>";
            }

            std::string rawText(std::shared_ptr<Token::RawText> block)
            {

                return block->get();
            }
    };
}
}

#endif // VISCHIO_HTMLRENDERER_H
