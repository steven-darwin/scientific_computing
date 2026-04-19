/**
 * @file GeometryTopology.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-08-18
 * @date Last Modified : 2025-08-30
 *
 * @brief [Header] Neutral representation of abstract shape
 */

#ifndef GEOMETRY_TOPOLOGY_HPP
#define GEOMETRY_TOPOLOGY_HPP

#include <unordered_map>
#include <memory>

class GeometryTopology {
public:
    /** Enumeration of GeometryTopology type */
    enum Type {VERTEX, EDGE, WIRE, FACE, SHELL, CELL, COMPOSITE};

    /** Constructor of GeometryTopology object
     */
    GeometryTopology();

    /** Destructor of GeometryTopology object */
    ~GeometryTopology();
    
    /** Virtual method that traverse GeometryTopology data structure and return set of GeometryTopology object with particular type */
    virtual void getDescendants(std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> &list, GeometryTopology::Type type) = 0;

protected:
    /** Accessor of _type */
    GeometryTopology::Type getType();

    /** Attribute to store the type of GeometryTopology object that is just being constructed */
    GeometryTopology::Type _type;

private:

};

#endif