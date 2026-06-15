/**
 * @file GeometryTopology.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-08-18
 * @date Last Modified : 2025-08-30
 *
 * @brief [Implementation] Neutral representation of abstract shape
 */

#include <string>
#include <tuple>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "geometry-topology/GeometryTopology.hpp"

GeometryTopology::GeometryTopology() {
    // TBA
}

GeometryTopology::~GeometryTopology() {
    // TBA
}

GeometryTopology::Type GeometryTopology::getType() {
    return _type;
}

void GeometryTopology::upsertAttribute(std::string name, std::vector<double> values) {
    if (_attributeList.contains(name)) {
        _attributeList.at(name) = values;
    }
    else {
        _attributeList.insert(std::make_pair(name, values));
    }

    std::cout << name << " attribute for " << this << " is upserted -> ";
    for (auto i : values) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

std::vector<double> GeometryTopology::getAttributeValue(std::string name) {
    return _attributeList.at(name);
}

std::vector<std::string> GeometryTopology::getAttributeName() {
    std::vector<std::string> name_list;
    
    for (auto attribute_item : _attributeList) {
        name_list.push_back(attribute_item.first);
    }

    return name_list;
}
