#include "parseCss.hpp"
#include <iostream>

#include <Lexer/lexer.hpp>
#include <Parser/earleyParser.hpp>
#include <Parser/treeParser.hpp>

#include "../utils.hpp"

using namespace std;
using namespace LipsLexer;
using namespace LipsParser;

CssList parseCssTree(PNode *node)
{

    NodeFun fun = [](PNode *nd, vector<NodeData> childs) -> NodeData
    {
        if (ruleEquals(nd, "Start -> SelectorList")) {
            return CssList{childs[0].getData<vector<CssSelector>>()};
        }
        if (ruleEquals(nd, "SelectorList -> Selector SelectorList")) {
            vector<CssSelector> selectors = childs[1].getData<vector<CssSelector>>();
            selectors.insert(selectors.begin(), childs[0].getData<CssSelector>());
            return selectors;
        }
        if (ruleEquals(nd, "SelectorList -> Selector")) {
            vector<CssSelector> selectors = {childs[0].getData<CssSelector>()};
            return selectors;
        }
        if (ruleEquals(nd, "Selector -> ID BRAKET_O DeclList BRAKET_C")) {
            vector<CssAttribute> attrs = childs[2].getData<vector<CssAttribute>>();
            return CssSelector{childs[0].getData<string>(), attrs};
        }
        if (ruleEquals(nd, "Selector -> MULT BRAKET_O DeclList BRAKET_C")) {
            vector<CssAttribute> attrs = childs[2].getData<vector<CssAttribute>>();
            return CssSelector{"*", attrs};
        }
        if (ruleEquals(nd, "Selector -> DOT ID BRAKET_O DeclList BRAKET_C")) {
            vector<CssAttribute> attrs = childs[3].getData<vector<CssAttribute>>();
            return CssSelector{"." + childs[1].getData<string>(), attrs};
        }
        if (ruleEquals(nd, "Selector -> NUMERAL ID BRAKET_O DeclList BRAKET_C")) {
            vector<CssAttribute> attrs = childs[3].getData<vector<CssAttribute>>();
            return CssSelector{"#" + childs[1].getData<string>(), attrs};
        }
        if (ruleEquals(nd, "Selector -> HEX BRAKET_O DeclList BRAKET_C")) {
            vector<CssAttribute> attrs = childs[2].getData<vector<CssAttribute>>();
            return CssSelector{childs[0].getData<string>(), attrs};
        }
        if (ruleEquals(nd, "DeclList -> Decl DeclList")) {
            CssAttribute attr = childs[0].getData<CssAttribute>();
            vector<CssAttribute> attrs = childs[1].getData<vector<CssAttribute>>();
            attrs.push_back(attr);
            return attrs;
        }
        if (ruleEquals(nd, "DeclList -> Decl")) {
            CssAttribute attr = childs[0].getData<CssAttribute>();
            return vector<CssAttribute>{attr};
        }
        if (ruleEquals(nd, "Decl -> ID COLON Value SEMICOLON")) {
            return CssAttribute{childs[0].getData<string>(), childs[2].getData<string>()};
        }
        if (ruleEquals(nd, "Value -> ID")) {
            return childs[0].getData<string>();
        }
        if (ruleEquals(nd, "Value -> NUM")) {
            return childs[0].getData<string>();
        }
        if (ruleEquals(nd, "Value -> HEX")) {
            return childs[0].getData<string>();
        }
        if (ruleEquals(nd, "Value -> NUM PERCENT")) {
            return childs[0].getData<string>() + "%";
        }
        if (ruleEquals(nd, "Value -> NUM ID")) {
            if (childs[1].getData<string>() == "px") {
                return childs[0].getData<string>() + "px";
            }
            else {
                cout << "no dim found" << endl;
                throw runtime_error("no dim found");
                return "";
            }
        }
        
        cerr << "no fun found:" << nd->ruleString << endl;
        system("pause");
        throw runtime_error("no fun found");
    };

    NodeData data = parseSynthezied(node, fun);

    CssList csslist = data.getData<CssList>();
    return csslist;
}

CssList parseCss(std::string code)
{
    
    string dir = __FILE__;
    // ../../LayoutEngine/LayoutEngine/languajeAnalisis/parse.cpp

    dir = dir.substr(0, dir.find_last_of("/\\"));
    // ../../LayoutEngine/LayoutEngine/languajeAnalisis

    dir = dir.substr(0, dir.find_last_of("/\\"));
    // ../../LayoutEngine/LayoutEngine

    dir = dir.substr(0, dir.find_last_of("/\\"));
    // ../../LayoutEngine

    string lexerGDir = dir + "/grammar/lexerCss.txt";
    string parserGDir = dir + "/grammar/parserCss.txt";

    string lexerGrammar = readFile(lexerGDir);
    string parserGrammar = readFile(parserGDir);

    if (lexerGrammar == "" || parserGrammar == "") {
        cout << "failed to read grammar" << endl;
        throw runtime_error("failed to read grammar");
        return CssList();
    }

    Lexer lexer = createLexer(lexerGrammar);
    Grammar pGrammar = createParser(parserGrammar);
    EarleyParser parser(pGrammar);

    bool correct;
    vector<Token> tokens = lexer.lex(code, correct);

    if(!correct) {
        cout << "failed to lex" << endl;
        for (Token t : tokens) {
            cout << t.name << ": " << t.value << endl;
        }
        system("pause");
        throw runtime_error("failed to lex");
        return CssList();
    }

    // erase spaces and comments
    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].name == "SPACE" || tokens[i].name == "COMMENT")
        {
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }
    


    int errorIdx = 0;
    PNode* root = parser.parse(tokens, correct, errorIdx);

    if(!correct) {
        cout << "failed to parse" << errorIdx << endl;

        for (int i = 0; i < tokens.size(); i++)
        {
            if (i == errorIdx) {
                cout << tokens[i].name << ": '" << tokens[i].value << "' <---------  " <<endl;
            }
            else 
                cout << tokens[i].name << ": '" << tokens[i].value << "\'" << endl;
        }
        
        system("pause");
        throw runtime_error("failed to parse");
        return CssList();
    }    
    // string str = treeToString(root);
    // cout << str;

    return parseCssTree(root);
}