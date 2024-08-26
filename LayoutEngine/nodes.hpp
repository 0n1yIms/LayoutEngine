#ifndef NODES_HPP
#define NODES_HPP

#include <string>
#include <vector>

struct HtmlAttribute {
    std::string name;
    std::string value;
};

struct HtmlNode{
    std::string tag;                        // tag: Div
    std::vector<HtmlAttribute> attributes;      // class="test" id="test"
    std::vector<HtmlNode*> children;        //<tag>CHILDREN</tag>
    std::string text;                       // <Text>TEXT</Text>
};

struct CssAttribute {
    std::string name;
    std::string value;
};

struct CssSelector {
    std::string tag;
    std::vector<CssAttribute> attributes;
};

struct CssList {
    std::vector<CssSelector> selectors;
};


#endif
