#include "le.hpp"

float calc(float size, Value v) {
    if(v.type == Value::Type::PC) {
        return v.value / 100.f * size;
    }
    else if (v.type == Value::Type::PX) {
        return v.value;
    }
    else {
        return v.value;
    }
}

Size calc(Size size, Size v) {
    Size s;
    s.width.value = calc(size.width.value, v.width);
    s.height.value = calc(size.height.value, v.height);

    return s;
}

void resolveConf(Node* node, Size rootSize) {
    // resolve size
    node->resConfig.size = calc(rootSize, node->config.size);
    // resolve padding
    node->resConfig.padding = calc(rootSize, node->config.padding);
    // resolve margin
    node->resConfig.margin = calc(rootSize, node->config.margin);
    // resolve realSize
    node->resConfig.realSize.width.value = node->resConfig.size.width.value + node->resConfig.padding.width.value;
    node->resConfig.realSize.height.value = node->resConfig.size.height.value + node->resConfig.padding.height.value;
}

void calculate(Node* node, int width, int height) {
    Size size;
    size.width.value = width;
    size.height.value = height;

    resolveConf(node, size);


    for (int i = 0; i < node->children.size(); i++) {
        resolveConf(node->children[i], node->config.getContainerSize());
    }
    
    if(node->config.direction == Direction::ROW){
        float tmp = 0.f;
        for (int i = 0; i < node->children.size(); i++) {
            node->children[i]->resConfig.position.width.value = tmp;
            tmp += node->children[i]->resConfig.realSize.width.value;
        }
    }
    else {
        float tmp = 0.f;
        for (int i = 0; i < node->children.size(); i++) {
            node->children[i]->resConfig.position.height.value = tmp;
            tmp += node->children[i]->resConfig.realSize.height.value;
        }
    }

    for (int i = 0; i < node->children.size(); i++) {
        Size s = node->config.getContainerSize();
        calculate(node->children[i], s.width.value, s.height.value);
    }
        
}








