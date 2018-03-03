#include <catch.hpp>

#include "vischio/inlinetoken.h"
#include <string>
#include <iostream>

namespace Vischio {
namespace Tests {

using namespace std::string_literals;

TEST_CASE( "Test basic emphasis", "[markdown][inline]" )
{

    {
        auto s = "_some text_"s;
        auto em = Token::Emphasis::factory(s.begin(), s.end());
        (*em)->tokenize();
        CHECK( (*em)->getChildren().size() == 1 );
        CHECK( (*em)->getChildren().at(0)->get() == "some text"s );
    }
    {
        auto s = "abcdef _some text_ efghi"s;
        auto em = Token::Emphasis::factory(s.begin(), s.end());
        (*em)->tokenize();
        CHECK( (*em)->getChildren().at(0)->get() == "some text"s );
    }
}

}
}
