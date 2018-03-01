#ifndef VISCHIO_CMDARGS_H
#define VISCHIO_CMDARGS_H

#include <iostream>
#include <iomanip>

#include<vector>
#include<string>
#include<algorithm>

#include <map>
#include <variant>

/**
 * Simple command line options and arguments parser
 *
 * Features:
 *
 * - Automatically generate "-h" and the help output.
 * - supports "-h" and "-o outputfile" options.
 * - Detect "invalid" options.
 * - Does not accept option values and arguments starting with "-".
 * - All options must have a default value (which can be empty).
 * - The API:
 *   - `CmdArgs(argc, argv)`: the constructor.
 *   - `parse(options)`: a Struct with
 *      - the command line option,
 *      - the default value (false for switches or a string
 *      when a value is passed),
 *      - a description for the option.
 *   - `cmd.usage(...)`: set the options and arguments
 *     for the "usage line in the help.
 *   - `cmd.usage()`: return the usage line from the help
 *   - `cmd.description = "..."`: set the description
 *     in the help text.
 *   - `cmd.help()`: output the help text.
 *   - `cmd.is()`: check if a switch is set or an option
 *      has been set.
 *   - `cmd.get()`: get the value for an option.
 *   - `cmd.arguments.at(1)`: get the first argument.
 *
 * Usage:
 *
 * int main(int argc, char **argv){
 *     CmdArgs::Options opts = {
 *         {"-o", {"output.html", "filename", "output file"}}
 *     };
 *     CmdArgs::CmdArgs args(argc, argv);
 *     if (!cmd.parse(opts)) {
 *         return -1;
 *     }
 *
 *     if (cmd.is("-h")) {
 *         cmd.help();
 *         return 0;
 *     }
 *
 *     auto output = cmd.get("-o");
 *     auto filename = filename.arguments.at(0);
 *
 *     // ...
 * }
 *
 * Inspired by iain, https://stackoverflow.com/a/868894/5239250
 *
 * TODO:
 *
 * - add a program description in the help
 */

namespace Vischio {
namespace CmdArgs {

    struct Option {
        std::variant<bool, std::string> value;
        std::string parameter;
        std::string description;
        bool set{false};
    };
    using Options = std::map<std::string, Option>;
    class CmdArgs {
        public:
            CmdArgs (int &argc, char **argv) :
                cmdName{argv[0]}
            {
                for (int i=1; i < argc; ++i) {
                    cmd.push_back(std::string(argv[i]));
                }
            }

            /**
             * Loop through the command line options, match
             * them with the valid options, and set the options
             * values.
             */
            bool parse(Options& options) 
            {
                // Ensure the -h help option
                if (options.find("-h") == options.end()) {
                    options["-h"] = {false, "", "help"};
                }

                try {
                    for (auto it = cmd.begin(); it < cmd.end(); it++) {
                        if (auto cur = options.find(*it); cur != options.end()) {
                            if (std::holds_alternative<bool>(cur->second.value)) {
                                cur->second.value = !std::get<bool>(cur->second.value);
                            } else {
                                if (it + 1 < cmd.end()) {
                                    ++it;
                                    if ((*it).at(0) == '-') {
                                        throw std::invalid_argument(*it);
                                    }
                                    cur->second.value = std::string(*it);
                                    cur->second.set = true;
                                }
                            }
                        } else {
                            if ((*it).at(0) == '-') {
                                throw std::invalid_argument(*it);
                            }
                            for (auto jt = it; jt < cmd.end(); ++jt) {
                                arguments.push_back(*jt);
                            }
                            it = cmd.end();
                        }
                    }
                } catch (const std::invalid_argument& e) {
                    std::cout << cmdName + ": unrecognized option '" + e.what() + "'" << std::endl;
                    std::cout << "Try '" + cmdName + " -h for more information." << std::endl;
                    return false;
                }

                this->options = options;

                return true;
            }

            void usage(std::string label) { usageLabel = label; }
            std::string usage() { return "Usage: " + cmdName + " " +usageLabel; }

            void help()
            {
                std::cout << usage() << "\n";
                if (!description.empty()) {
                    std::cout << description << "\n\n";
                }
                for (const auto& opt: options) {
                    std::cout << "  " << std::left << std::setw(cmdHelpLength) << opt.first + (opt.second.parameter.empty() ? "" : " " + opt.second.parameter) << " " + opt.second.description + "\n";
                }
            }

            /**
             * For bool options return the value of the option.
             * For the string ones return true if the option was in the command line.
             */
            bool is(const std::string &option) const{
                const auto& it = options.at(option);
                if (std::holds_alternative<bool>(it.value)) {
                    return std::get<bool>(it.value);
                } else {
                    return it.set;
                }
            }

            /**
             * get the value of string options
             */
            std::string get(const std::string &option) const
            {
                try {
                    return std::get<std::string>(options.at(option).value);
                } catch (const std::out_of_range& e) {
                    std::cout << "CmdArgs::get(): Unexpected option '" + option + "'" << std::endl;
                    throw(e);
                }
                return "";
            }

            std::vector<std::string> arguments; /** list of arguments */
            int cmdHelpLength{15}; /** padding for the description */
            std::string description{};
        private:
            std::vector<std::string> cmd{};
            std::string cmdName{};
            Options options;
            std::string usageLabel{"[OPTION]... [FILE]..."};
    };
}
}
#endif // VISCHIO_CMDARGS_H
