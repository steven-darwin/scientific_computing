/**
 * @file GeometryTopologyCell.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-08-18
 * @date Last Modified : 2025-08-30
 *
 * @brief [Header] Neutral representation of cell
 */

#ifndef GEOMETRY_TOPOLOGY_CELL_HPP
#define GEOMETRY_TOPOLOGY_CELL_HPP

#include <array>
#include <memory>
#include <unordered_map>

#include "GeometryTopology.hpp"
#include "GeometryTopologyShell.hpp"

class GeometryTopologyCell : public GeometryTopology {
public:
    /** Constructor of GeometryTopologyCell object
     */
    GeometryTopologyCell(std::shared_ptr<GeometryTopologyShell> shell1_ptr, std::shared_ptr<GeometryTopologyShell> shell2_ptr);

    /** Destructor of GeometryTopologyCell object */
    ~GeometryTopologyCell();

    /** Method that traverse GeometryTopologyCell data structure and return set of GeometryTopology object with particular type */
    void getDescendants(std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int>& list, GeometryTopology::Type type);

    /** Method to get address of shell on the list of shellss that build this cell */
    std::shared_ptr<GeometryTopologyShell> getShell(unsigned int shell_idx);

private:
    /** Attribute to store list of shells that build this cell */
    std::array<std::shared_ptr<GeometryTopologyShell>, 2> _shells;
};

#endif