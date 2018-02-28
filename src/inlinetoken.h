#ifndef VISCHIO_INLINETOKEN_H
#define VISCHIO_INLINETOKEN_H

#include <vector>
#include <string>
#include "string.h"

#include <regex>
#include <iterator>

namespace Vischio {
namespace Token {

    class Inline;
    using Inlines = std::vector<std::shared_ptr<Inline>>;

    /**
     * we need something to be virtual so that we have
     * a virtual table ... which is needed, for the dynamic
     * casting of the shared pointer...)
     */
    class Inline
    {
        public:
            Inline() {}
            Inline(std::string type) : type{type} {}
            Inline(std::string type, std::string::iterator start, std::string::iterator end) : type{type}, iteratorStartMatch{start}, iteratorEndMatch{end} {}
            ~Inline() {}

            virtual std::optional<std::shared_ptr<Inline>> factory(std::string::iterator start, std::string::iterator end) const = 0;

            std::string::iterator getIteratorStartMatch() { return iteratorStartMatch; }
            std::string::iterator getIteratorEndMatch() { return iteratorEndMatch; }

            virtual void tokenize() = 0;
            virtual std::string get() = 0;

            virtual std::string getType() const {return type;}
            Inlines getChildren() { return children; }
        private:
        protected:
            std::string type{"Inline"};
            std::string::iterator iteratorStartMatch;
            std::string::iterator iteratorEndMatch;
            Inlines children;

    };

    class RawText: public Inline
    {
        public:
            RawText() : Inline("RawText") {}
            RawText(std::string content) : Inline("RawText"), content{content} {}
            RawText(std::string::iterator start, std::string::iterator end) : Inline("RawText", start, end) {}

            /**
             * RawText does not use the factory.
             * It's what does not match.
             */
            std::optional<std::shared_ptr<Inline>> factory(std::string::iterator start, std::string::iterator end) const override
            {
                return {};
            }

            void tokenize() override {}

            std::string get() override
            {
                return content;
            }

        private:
            std::string content;
    };

    /**
     * _some text_ or *some text*
     */
    class Emphasis: public Inline
    {
        public:
            Emphasis() : Inline("Emphasis") {}
            Emphasis(std::string::iterator start, std::string::iterator end) : Inline("Emphasis", start, end) {}

            std::optional<std::shared_ptr<Inline>> factory(std::string::iterator start, std::string::iterator end) const override
            {
                std::string text = std::string(start, end);
                std::smatch match;
                // TODO: how can we calculate it only once for the first Emphasis?
                std::regex pattern(R"(\b_\B(.+?)_\b|\s\*([^\s].*?[^\s])\*\s)");
                if (std::regex_search(text, match, pattern)) {
                    // std::cout << match[1] << " at position " << match.position(1) << " of length " <<  match.length(1) << std::endl;
                    // TODO: how to get from the match an iterator to the string
                    // return {std::make_shared<Emphasis>(match[0].first, match[0].second)};
                    return {std::make_shared<Emphasis>(start + match.position(), start + match.position() + match.length())};
                }

                return {};
            }

            void tokenize() override {
                children.push_back(std::make_shared<RawText>(std::string(iteratorStartMatch + 1, iteratorEndMatch - 1)));
            }

            std::string get() override
            {
                std::string result{};
                for (auto child: children) {
                    result += child->get();
                }
                return result;
            }

        private:
            std::string data;
    };

    /**
     * \s\s$
     */
    /*
    class LineBreak: public Inline
    {
        public:
            LineBreak() : Inline("LineBreak") {}
            LineBreak(std::string data) : Inline("LineBreak"), data{data} {}

            std::optional<std::shared_ptr<Inline>> factory(std::string::iterator start, std::string::iterator end) const override
            {
                return {};
            }

        private:
            std::string data;
    };
    */

}
}

#endif // VISCHIO_INLINETOKEN_H

