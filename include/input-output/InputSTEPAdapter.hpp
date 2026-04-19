/**
 * @file InputSTEPAdapter.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-08-30
 *
 * @brief [Header] Input adapter to OCCT STEP library
 */

#ifndef IO_INPUT_STEP_ADAPTER_HPP
#define IO_INPUT_STEP_ADAPTER_HPP

#include <string>
#include <memory>
#include <unordered_map>

#include "STEPControl_Reader.hxx"
#include "TopoDS_Vertex.hxx"
#include "TopoDS_Edge.hxx"
#include "TopoDS_Face.hxx"
#include "TopoDS_Solid.hxx"

#include "geometry-topology/GeometryTopology.hpp"
#include "geometry-topology/GeometryTopologyCell.hpp"
#include "geometry-topology/GeometryTopologyShell.hpp"
#include "geometry-topology/GeometryTopologyFace.hpp"
#include "geometry-topology/GeometryTopologyWire.hpp"
#include "geometry-topology/GeometryTopologyEdge.hpp"
#include "geometry-topology/GeometryTopologyVertex.hpp"
#include "InputAdapter.hpp"

static unsigned int counter;

class InputSTEPAdapter : public InputAdapter {
public:
    /** Constructor of InputSTEPAdapter object
     */
    InputSTEPAdapter();
    InputSTEPAdapter(std::string step_file_name);

    /** Destructor of InputSTEPAdapter object */
    ~InputSTEPAdapter();

    /** Method to deserialize STEP file's data to internal neutral data structure */
    std::shared_ptr<GeometryTopology> deserialize();

private:
    /** Attribute to store the STEP file path */
    const char* _stepFilePath;

    /** Attribute to store the map of TopoDS_Vertex pointer to GeometryTopologyVertex pointer */
    std::unordered_map<Handle(TopoDS_TShape), std::shared_ptr<GeometryTopologyVertex>> _vertexList;

    /** Attribute to store the map of TopoDS_Edge pointer to GeometryTopologyEdge pointer */
    std::unordered_map<Handle(TopoDS_TShape), std::shared_ptr<GeometryTopologyEdge>> _edgeList;

    /** Attribute to store the map of TopoDS_Face pointer to GeometryTopologyFace pointer */
    std::unordered_map<Handle(TopoDS_TShape), std::shared_ptr<GeometryTopologyFace>> _faceList;

    /** Attribute to store the map of TopoDS_Solid pointer to GeometryTopologyCell pointer */
    std::unordered_map<Handle(TopoDS_TShape), std::shared_ptr<GeometryTopologyCell>> _cellList;

    /** Method that does necessary actions for particular TopoDS_Vertex object */
    void handleVertex(const TopoDS_Vertex occt_vertex);

    /** Method that does necessary actions for particular TopoDS_Edge object */
    void handleEdge(const TopoDS_Edge occt_edge);
    
    /** Method that does necessary actions for particular TopoDS_Face object */
    void handleFace(const TopoDS_Face occt_face);
    
    /** Method that does necessary actions for particular TopoDS_Solid object */
    void handleCell(const TopoDS_Solid occt_solid);
};

#endif