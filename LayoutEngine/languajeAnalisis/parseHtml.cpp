#include "parseHtml.hpp"
#include <iostream>

#include <Lexer/lexer.hpp>
#include <Parser/earleyParser.hpp>
#include <Parser/treeParser.hpp>

#include "../utils.hpp"

using namespace std;
using namespace LipsLexer;
using namespace LipsParser;

HtmlNode* parseHtmlTree(PNode *node)
{

    NodeFun fun = [](PNode *nd, vector<NodeData> childs) -> NodeData
    {
        if (ruleEquals(nd, "Start -> ElementStart")) {
            return childs[0];
        }
        if (ruleEquals(nd, "ElementStart -> SPACE Element")) {
            return childs[1];
        }
        if (ruleEquals(nd, "ElementStart -> SPACE Element SPACE")) {
            return childs[1];
        }
        if (ruleEquals(nd, "ElementStart -> Element SPACE")) {
            return childs[0];
        }
        if (ruleEquals(nd, "Element -> LT ID Attributes GT ElementList LT SLASH ID SPACE GT")) {
            
            string id = childs[0].getData<string>();
            vector<HtmlAttribute> attrs = childs[2].getData<vector<HtmlAttribute>>();

            vector<HtmlNode*> children = childs[4].getData<vector<HtmlNode*>>();

            return new HtmlNode{id, attrs, children, ""};
        }
        if (ruleEquals(nd, "Element -> LT ID Attributes GT ElementList LT SLASH ID GT")) {
            string id = childs[1].getData<string>();
            vector<HtmlAttribute> attrs = childs[2].getData<vector<HtmlAttribute>>();
            vector<HtmlNode*> children = childs[4].getData<vector<HtmlNode*>>();

            return new HtmlNode{id, attrs, children, ""};
        }
        if (ruleEquals(nd, "Element -> LT ID Attributes SLASH GT"))
        {
            string id = childs[1].getData<string>();
            vector<HtmlAttribute> attrs = childs[2].getData<vector<HtmlAttribute>>();
            return new HtmlNode{id, attrs, {}, ""};
        }
        if (ruleEquals(nd, "Element -> Text")) {
            string text = childs.at(0).getData<string>();
            return new HtmlNode{"text", {}, {}, text};
        }
        if (ruleEquals(nd, "Text -> ID Text") ||
            ruleEquals(nd, "Text -> SYMBOL Text") ||
            ruleEquals(nd, "Text -> QUOTE Text") ||
            ruleEquals(nd, "Text -> SPACE Text") ||
            ruleEquals(nd, "Text -> SLASH Text")) {
            string data1 = childs[0].getData<string>();
            string data2 = childs[1].getData<string>();
            return string(data1 + data2);
        }
        if (ruleEquals(nd, "Text -> ?")) {
            return string("");
        }

        if (ruleEquals(nd, "Attributes -> SPACE ID EQUALS QUOTE ID QUOTE Attributes")) {
            string id = childs[1].getData<string>();
            string value = childs[4].getData<string>();
            vector<HtmlAttribute> attrs = childs[6].getData<vector<HtmlAttribute>>();
            
            attrs.push_back({id, value});
            return attrs;
        }
        if (ruleEquals(nd, "Attributes -> SPACE ID SPACE EQUALS QUOTE ID QUOTE Attributes")) {
            string id = childs[1].getData<string>();
            string value = childs[5].getData<string>();
            vector<HtmlAttribute> attrs = childs[7].getData<vector<HtmlAttribute>>();
            attrs.push_back({id, value});

            return attrs;
        }
        if (ruleEquals(nd, "Attributes -> SPACE ID SPACE EQUALS SPACE QUOTE ID QUOTE Attributes")) {
            string id = childs[1].getData<string>();
            string value = childs[6].getData<string>();
            vector<HtmlAttribute> attrs = childs[8].getData<vector<HtmlAttribute>>();
            attrs.push_back({id, value});

            return attrs;
        }
        if (ruleEquals(nd, "Attributes -> SPACE") ||
            ruleEquals(nd, "Attributes -> ?"))
            return vector<HtmlAttribute>();

        if (ruleEquals(nd, "ElementList -> Element ElementList")) {
            vector<HtmlNode*> nodes = childs[1].getData<vector<HtmlNode*>>();
            nodes.insert(nodes.begin(), childs[0].getData<HtmlNode*>());
            return nodes;
        }
        if (ruleEquals(nd, "ElementList -> SPACE ElementList")) {
            return childs[1];
        }
        if (ruleEquals(nd, "ElementList -> ?"))
            return vector<HtmlNode*>();

        cerr << "no fun found" << endl;
        throw runtime_error("no fun found");
    };

    NodeData data = parseSynthezied(node, fun);

    HtmlNode* html = data.getData<HtmlNode*>();
    return html;
}

HtmlNode* parseHtml(std::string code)
{
    
    string dir = __FILE__;
    // ../../LayoutEngine/LayoutEngine/languajeAnalisis/parse.cpp

    dir = dir.substr(0, dir.find_last_of("/\\"));
    // ../../LayoutEngine/LayoutEngine/languajeAnalisis

    dir = dir.substr(0, dir.find_last_of("/\\"));
    // ../../LayoutEngine/LayoutEngine

    dir = dir.substr(0, dir.find_last_of("/\\"));
    // ../../LayoutEngine

    string lexerGDir = dir + "/grammar/lexerHtml.txt";
    string parserGDir = dir + "/grammar/parserHtml.txt";

    string lexerGrammar = readFile(lexerGDir);
    string parserGrammar = readFile(parserGDir);

    if (lexerGrammar == "" || parserGrammar == "") {
        cout << "failed to read grammar" << endl;
        return nullptr;
    }

    Lexer lexer = createLexer(lexerGrammar);
    Grammar pGrammar = createParser(parserGrammar);
    EarleyParser parser(pGrammar);

    bool correct;
    vector<Token> tokens = lexer.lex(code, correct);

    if(!correct) {
        cout << "failed to lex" << endl;
        return nullptr;
    }

    int errorIdx = 0;
    PNode* root = parser.parse(tokens, correct, errorIdx);

    if(!correct) {
        cout << "failed to parse" << endl;
        return nullptr;
    }    
    // string str = treeToString(root);
    // cout << str;

    return parseHtmlTree(root);
}