#include <iostream>
#include <fstream>

#include "cmdargs.h"

#include "document.h"
#include "blocktoken.h"
#include "inlinetoken.h"
#include "htmlrenderer.h"

int main(int argc, char **argv)
{

    using namespace std::string_literals;

    Vischio::CmdArgs::Options opts = {
        {"-o", {"input.md"s, "filename", "output file"}}
    };
    Vischio::CmdArgs::CmdArgs cmd{argc, argv};
    cmd.usage("[OPTION]... [FILE]");
    cmd.description = "Convert markdown files to html";

    if (!cmd.parse(opts)) {
        return -1;
    }

    if (cmd.is("-h")) {
        cmd.help();
        return 0;
    }

    auto output = cmd.is("-o") ? cmd.get("-o") : "";

    std::string filename;
    try {
        filename = cmd.arguments.at(0);
    } catch (const std::out_of_range& e) {
        std::cout << cmd.usage() << std::endl;
        return -1;
    }

    std::ifstream markdown(filename);
    if (markdown.is_open()) {
        Vischio::Renderer::Html renderer;
        std::string rendered = renderer.render(Vischio::Document(markdown));
        // TODO: add the output in a file
        std::cout << rendered << std::endl;
    } else {
        std::cout << "File not found (" + filename + ")"  << std::endl;
    }

}
