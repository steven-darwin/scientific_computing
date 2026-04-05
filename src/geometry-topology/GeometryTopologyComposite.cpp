/**
 * @file GeometryTopologyComposite.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2026-03-22
 * @date Last Modified : 2026-03-22
 *
 * @brief [Implementation] Neutral representation of multiple cells
 */

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "geometry-topology/GeometryTopologyComposite.hpp"
#include "geometry-topology/GeometryTopologyCell.hpp"

GeometryTopologyComposite::GeometryTopologyComposite() {
    _type = GeometryTopology::Type::COMPOSITE;
}

GeometryTopologyComposite::~GeometryTopologyComposite() {
    // TBA
}

void GeometryTopologyComposite::getDescendants(std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int>& list, GeometryTopology::Type type) {
    if (type < _type) {
        for (auto cell : _cells) {
            if (type + 1 == _type) {
                if (!list.contains(cell)) {
                    list.insert(std::make_pair(cell, list.size()));
                }
            }
            else {
                cell->getDescendants(list, type);
            }
        }
    }
    else {
        return;
    }
}

void GeometryTopologyComposite::addCell(std::shared_ptr<GeometryTopologyCell> cell_ptr) {
    _cells.push_back({ cell_ptr });
    std::cout << "Cell: " << cell_ptr << std::endl;
}