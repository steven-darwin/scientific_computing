/**
 * @file GeometryTopologyVertex.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-08-18
 * @date Last Modified : 2025-08-30
 *
 * @brief [Header] Neutral representation of vertex
 */

#ifndef GEOMETRY_TOPOLOGY_VERTEX_HPP
#define GEOMETRY_TOPOLOGY_VERTEX_HPP

#include <unordered_map>

#include "GeometryTopology.hpp"

class GeometryTopologyVertex : public GeometryTopology {
public:
    /** Enumeration of GeometryTopologyVertex coordinate */
    enum Coordinate {
        X = 0, 
        Y = 1, 
        Z = 2
    };

    /** Constructor of GeometryTopologyVertex object
     */
    GeometryTopologyVertex(double x, double y, double z);

    /** Destructor of GeometryTopologyVertex object */
    ~GeometryTopologyVertex();

    /** Method that traverse GeometryTopologyVertex data structure and return set of GeometryTopology object with particular type */
    void getDescendants(std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int>& list, GeometryTopology::Type type);

    /** Accessor of _coordinateX, _coordinateY, _coordinate_Z */
    double getCoordinate(GeometryTopologyVertex::Coordinate axis);

private:
    /** Attribute to store vertex X coordinate */
    double _coordinateX = 0;

    /** Attribute to store vertex Y coordinate */
    double _coordinateY = 0;
    
    /** Attribute to store vertex Z coordinate */
    double _coordinateZ = 0;
};

#endif