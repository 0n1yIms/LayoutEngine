#include <iostream>
#include <filesystem>
#include <fstream>

#include "le.hpp"
#include <Lexer/lexer.hpp>
#include <Parser/earleyParser.hpp>
#include <Lexer/stringToExpr.hpp>
#include <Parser/treeParser.hpp>
#include "nodes.hpp"

#include "languajeAnalisis/parseHtml.hpp"
#include "languajeAnalisis/parseCss.hpp"
#include "utils.hpp"

using namespace std;
using namespace LipsLexer;
using namespace LipsParser;


void printHtmlNode(HtmlNode &node, int depth)
{
    for (int i = 0; i < depth; ++i)
        cout << "  ";
    cout << "<" << node.tag;
    for (HtmlAttribute &attr : node.attributes)
        cout << " " << attr.name << "=\"" << attr.value << "\"";
    cout << ">\n";
    if (node.text != "")
    {
        for (int i = 0; i < depth + 1; ++i)
            cout << "  ";
        cout << node.text << "\n";
    }
    
    for (HtmlNode* &child : node.children)
        printHtmlNode(*child, depth + 1);
    for (int i = 0; i < depth; ++i)
        cout << "  ";
    cout << "</" << node.tag << ">\n";
}

void printCss(CssList css){
    for (CssSelector &selector : css.selectors)
    {
        cout << selector.tag << " {\n";
        for (CssAttribute &attr : selector.attributes)
            cout << "  " << attr.name << ": " << attr.value << ";\n";
        cout << "}\n";
    }
}


int main() {

    string dir = __FILE__;
    dir = dir.substr(0, dir.find_last_of("/\\"));
    dir = dir.substr(0, dir.find_last_of("/\\")) + "/";

    string htmlcodedir = dir + "test/code1.html";
    string csscodedir = dir + "test/code1.css";

    string htmlcode = readFile(htmlcodedir);
    string csscode = readFile(csscodedir);

    HtmlNode* node = parseHtml(htmlcode);
    CssList css = parseCss(csscode);

    // printHtmlNode(*node, 0);
    printCss(css);


    system("pause");
    // Node* node = new Node;
    // node->children.push_back()

    return 0;
}