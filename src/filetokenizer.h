#ifndef VISCHIO_FILETOKENIZER_H
#define VISCHIO_FILETOKENIZER_H

#include <iostream>

#include <string>
#include <vector>
#include <fstream> // ifstream;
#include <iterator> // istream_iterator;
#include <algorithm> // copy, replace
#include <regex>

namespace Vischio {
namespace Tokenizer {
namespace File {
    std::vector<std::string> getLines(std::ifstream& markdownFile)
    {
        std::vector<std::string> lines;
        for (std::string line; std::getline(markdownFile, line); /**/ )
            lines.push_back(line);
        /*
        // TODO: why is this tokenizing the file instead of giving it line by line?
        std::copy(std::istream_iterator<std::string>(markdownFile),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(lines));
        */
        return lines;
    }

    std::vector<std::string> getNormalized(std::vector<std::string> lines)
    {
        std::vector<std::string> result{};
        std::string codeFence = "";
        for (auto line: lines) {
            if ((line.find("````") == 0) || (line.find("~~~~") == 0)) {
                if (codeFence == "") {
                    result.push_back("\n");
                    codeFence = line.substr(0, 3);
                } else if (line.find(codeFence) == 0) {
                    codeFence = "";
                }
            }
            line = std::regex_replace(line, std::regex(R"(\t)"), "    ");
            result.push_back(line);
        }
        return result;
    }
}
}
}

#endif // VISCHIO_FILETOKENIZER_H

