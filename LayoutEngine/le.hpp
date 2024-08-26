#ifndef LE_HPP
#define LE_HPP

#include "attrib.hpp"
#include <vector>


struct Value {
    enum Type {
        PX,     //pixels
        DP,     //dot per inch
        PC      //percent
    };

    Type type;
    float value;
};

struct Size {
    Value width, height;
};

struct Config {
    Size size;
    Size margin;
    Size padding;
    Size realSize;

    Size position;

    Direction direction;

    Size getContainerSize(){
        Size s;
        s.width.value = size.width.value - 2.f * padding.width.value;
        s.height.value = size.height.value - 2.f * padding.height.value;
        return s;
        
    }
    
};

/*struct ResolvedConfig {
    int width, height;
    int marginW, marginH;
    int paddingW, paddingH;
};*/

struct Node {
    Config config;
    Config resConfig;
    std::vector<Node*> children;


};


void calculate(Node* node, int width, int);

#endif
