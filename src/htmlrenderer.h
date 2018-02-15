#ifndef VISCHIO_HTMLRENDERER_H
#define VISCHIO_HTMLRENDERER_H

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <memory>

#include "blocktoken.h"
#include "string.h"

namespace Vischio {
namespace Renderer {
    class Html
    {
        public:
            Html()
            {
                // blocks["Heading"] = [=](const auto& block){return this->heading(block);};
                // blocks["Paragraph"] = [=](const auto& block){return this->paragraph(block);};
                blocks["Heading"] = [=](std::shared_ptr<Token::Block> block) -> std::string {return this->heading(std::dynamic_pointer_cast<Token::Heading>(block));};
                blocks["Paragraph"] = [=](std::shared_ptr<Token::Block> block) -> std::string {return this->paragraph(std::dynamic_pointer_cast<Token::Paragraph>(block));};
            }
            std::string render(Document document)
            {
                std::vector<std::string> items;
                auto& children = document.getChildren();
                for (auto const & child: children) {
                    // std::cout << "-" << child->getType() << std::endl;
                    items.push_back(blocks.at(child->getType())(child));
                }

                std::string result{};
				return ::Vischio::String::join(items.begin(), items.end(), "\n");
            }
        protected:
            std::map<std::string, std::function<std::string(std::shared_ptr<Token::Block>)>> blocks;
            std::string heading(std::shared_ptr<Token::Heading> block)
            {
               return "<h" + std::to_string(block->getLevel()) + ">" + block->getTitle() + "</h1>";
            }

            std::string paragraph(std::shared_ptr<Token::Paragraph> block)
            {

               auto lines = block->getLines();
               return "<p>" + ::Vischio::String::join(lines.begin(), lines.end(), "\n") + "</p>";
            }
    };
}
}

#endif // VISCHIO_HTMLRENDERER_H
