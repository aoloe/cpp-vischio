#include <iostream>
#include <fstream>

#include "vischio.h"
#include "document.h"
#include "blocktoken.h"
#include "htmlrenderer.h"


int main()
{

    // Vischio::Vischio vischio{};
    // vischio.markdown();


    std::ifstream markdown("input.md");
    if (markdown.is_open()) {
        Vischio::Renderer::Html renderer;
        std::string rendered = renderer.render(Vischio::Document(markdown));
        std::cout << rendered << std::endl;
    } else {
        std::cout << "File not found (input.md)"  << std::endl;
    }

}
