/**
 * @file GeometryTopologyComposite.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2026-03-22
 * @date Last Modified : 2026-03-22
 *
 * @brief [Header] Neutral representation of multiple cells
 */

#ifndef GEOMETRY_TOPOLOGY_COMPOSITE_HPP
#define GEOMETRY_TOPOLOGY_COMPOSITE_HPP

#include <vector>
#include <memory>
#include <unordered_map>

#include "GeometryTopology.hpp"
#include "GeometryTopologyCell.hpp"

class GeometryTopologyComposite : public GeometryTopology {
public:
    /** Constructor of GeometryTopologyComposite object
     */
    GeometryTopologyComposite();

    /** Destructor of GeometryTopologyComposite object */
    ~GeometryTopologyComposite();

    /** Method that traverse GeometryTopologyCell data structure and return set of GeometryTopology object with particular type */
    void getDescendants(std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int>& list, GeometryTopology::Type type);

    /** Method to get address of shell on the list of cells that build this cell */
    void addCell(std::shared_ptr<GeometryTopologyCell> cell_ptr);

private:
    /** Attribute to store list of shells that build this cell */
    std::vector<std::shared_ptr<GeometryTopologyCell>> _cells;
};

#endif