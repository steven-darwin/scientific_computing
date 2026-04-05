/**
 * @file GeometryTopologyCell.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-08-18
 * @date Last Modified : 2025-08-30
 *
 * @brief [Implementation] Neutral representation of cell
 */

#include <memory>
#include <iostream>
#include <unordered_map>

#include "geometry-topology/GeometryTopologyCell.hpp"

GeometryTopologyCell::GeometryTopologyCell(std::shared_ptr<GeometryTopologyShell> shell1_ptr, std::shared_ptr<GeometryTopologyShell> shell2_ptr) {
    _type = GeometryTopology::Type::CELL;
    
    _shells[0] = shell1_ptr;
    _shells[1] = shell2_ptr;

    std::cout << "Wire 1: " << shell1_ptr << ", Wire 2: " << shell2_ptr << std::endl;
}

GeometryTopologyCell::~GeometryTopologyCell() {
    // TBA
}

void GeometryTopologyCell::getDescendants(std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int>& list, GeometryTopology::Type type) {
    if (type < _type) {
        for (auto shell : _shells) {
            if (type + 1 == _type) {
                if (!list.contains(shell)) {
                    list.insert(std::make_pair(shell, list.size()));
                }
            }
            else {
                shell->getDescendants(list, type);
            }
        }
    }
    else {
        return;
    }
}

std::shared_ptr<GeometryTopologyShell> GeometryTopologyCell::getShell(unsigned int shell_idx) {
    return _shells[shell_idx];
}