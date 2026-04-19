/**
 * @file InputSTEPAdapter.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-08-30
 *
 * @brief [Implementation] Input adapter to OCCT STEP library
 */

#include <memory>
#include <iostream>
#include <string>

#include "TopExp.hxx"
#include "TopExp_Explorer.hxx"
#include "TopTools_IndexedMapOfShape.hxx";
#include "BRep_Tool.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Vertex.hxx"

#include "geometry-topology/GeometryTopology.hpp"
#include "input-output/InputSTEPAdapter.hpp"


InputSTEPAdapter::InputSTEPAdapter(std::string step_file_name) {
	std::string step_file_path = "../../asset/STEP/" + step_file_name + ".STEP";

	char* step_buffer = new char[step_file_path.length() + 1];
	std::memcpy(step_buffer, step_file_path.c_str(), step_file_path.length());
	step_buffer[step_file_path.length()] = '\0';

	_stepFilePath = step_buffer;
}

InputSTEPAdapter::~InputSTEPAdapter() {
    // do nothing
}

std::shared_ptr<GeometryTopology> InputSTEPAdapter::deserialize() {
	STEPControl_Reader reader;
	IFSelect_ReturnStatus stat = reader.ReadFile(_stepFilePath);
	IFSelect_PrintCount mode = IFSelect_ItemsByEntity;
	reader.PrintCheckLoad(false, mode);

	Standard_Integer NbRoots = reader.NbRootsForTransfer();
	std::cout << "Number of roots in STEP file: " << NbRoots << std::endl;

	Standard_Integer NbTrans = reader.TransferRoots();
	reader.PrintCheckTransfer(false, mode);
	std::cout << "STEP roots transferred: " << NbTrans << std::endl;
	std::cout << "Number of resulting shapes is: " << reader.NbShapes() << std::endl;

	TopoDS_Shape step_root = reader.OneShape();

	TopAbs_ShapeEnum shape_list[4] = {TopAbs_VERTEX, TopAbs_EDGE, TopAbs_FACE, TopAbs_SOLID};
	for (auto shape_enum : shape_list) {
		std::cout << "----------------------------------------------------" << std::endl;
		switch (shape_enum) {
		case TopAbs_VERTEX:
			for (TopExp_Explorer vertex_exp(step_root, TopAbs_VERTEX); vertex_exp.More(); vertex_exp.Next()) {
				handleVertex(TopoDS::Vertex(vertex_exp.Current()));
			}

			break;
		case TopAbs_EDGE:
			for (TopExp_Explorer edge_exp(step_root, TopAbs_EDGE); edge_exp.More(); edge_exp.Next()) {
				handleEdge(TopoDS::Edge(edge_exp.Current()));
			}

			break;
		case TopAbs_FACE:
			for (TopExp_Explorer face_exp(step_root, TopAbs_FACE); face_exp.More(); face_exp.Next()) {
				handleFace(TopoDS::Face(face_exp.Current()));
			}

			break;
		case TopAbs_SOLID:
			for (TopExp_Explorer cell_exp(step_root, TopAbs_SOLID); cell_exp.More(); cell_exp.Next()) {
				handleCell(TopoDS::Solid(cell_exp.Current()));
			}

			break;
		}

		if (step_root.ShapeType() == shape_enum) {
			break;
		}
	}
	
	return _cellList.at(step_root.TShape());
}

void InputSTEPAdapter::handleVertex(const TopoDS_Vertex occt_vertex) {
	if (!_vertexList.contains(occt_vertex.TShape())) {
		std::cout << "Vertex " << _vertexList.size() << std::endl;

		gp_Pnt pnt = BRep_Tool::Pnt(occt_vertex);
		gp_Pnt transformed_pnt = pnt.Transformed(occt_vertex.Location().Transformation());

		_vertexList.insert(std::make_pair(occt_vertex.TShape(), new GeometryTopologyVertex(transformed_pnt.X(), transformed_pnt.Y(), transformed_pnt.Z())));

		std::cout << "Address: " << _vertexList.at(occt_vertex.TShape()) << std::endl;
	}
}

void InputSTEPAdapter::handleEdge(const TopoDS_Edge occt_edge) {
	if (!_edgeList.contains(occt_edge.TShape())) {
		std::cout << "Edge " << _edgeList.size() << std::endl;

		std::array<Handle(TopoDS_TShape), 2> occt_vertex_ptr_list = {nullptr, nullptr};
		unsigned int occt_vertex_ptr_list_idx = 0;

		for (TopExp_Explorer vertex_on_edge_exp(occt_edge, TopAbs_VERTEX); vertex_on_edge_exp.More(); vertex_on_edge_exp.Next()) {
			occt_vertex_ptr_list[occt_vertex_ptr_list_idx] = TopoDS::Vertex(vertex_on_edge_exp.Current()).TShape();
			occt_vertex_ptr_list_idx++;
		}

		_edgeList.insert(std::make_pair(occt_edge.TShape(), new GeometryTopologyEdge(_vertexList.at(occt_vertex_ptr_list[0]), _vertexList.at(occt_vertex_ptr_list[1]))));

		std::cout << "Address: " << _edgeList.at(occt_edge.TShape()) << std::endl;
	}

}

void InputSTEPAdapter::handleFace(const TopoDS_Face occt_face) {
	if (!_faceList.contains(occt_face.TShape())) {
		std::cout << "Face " << _faceList.size() << std::endl;

		for (TopExp_Explorer wire_on_face_exp(occt_face, TopAbs_WIRE); wire_on_face_exp.More(); wire_on_face_exp.Next()) {
			std::shared_ptr<GeometryTopologyWire> wire_ptr = std::make_shared<GeometryTopologyWire>();
			for (TopExp_Explorer edge_on_wire_exp(occt_face, TopAbs_EDGE); edge_on_wire_exp.More(); edge_on_wire_exp.Next()) {
				wire_ptr->addEdge(_edgeList.at(TopoDS::Edge(edge_on_wire_exp.Current()).TShape()), edge_on_wire_exp.Current().Orientation());
			}
			_faceList.insert(std::make_pair(occt_face.TShape(), new GeometryTopologyFace(wire_ptr, wire_ptr->reverseCopy())));
		}

		std::cout << "Address: " << _faceList.at(occt_face.TShape()) << std::endl;
	}
}

void InputSTEPAdapter::handleCell(const TopoDS_Solid occt_solid) {
	if (!_cellList.contains(occt_solid.TShape())) {
		std::cout << "Cell " << _cellList.size() << std::endl;

		for (TopExp_Explorer shell_on_solid_exp(occt_solid, TopAbs_SHELL); shell_on_solid_exp.More(); shell_on_solid_exp.Next()) {
			std::shared_ptr<GeometryTopologyShell> shell_ptr = std::make_shared<GeometryTopologyShell>();
			for (TopExp_Explorer face_on_shell_exp(occt_solid, TopAbs_FACE); face_on_shell_exp.More(); face_on_shell_exp.Next()) {
				shell_ptr->addFace(_faceList.at(TopoDS::Face(face_on_shell_exp.Current()).TShape()), face_on_shell_exp.Current().Orientation());
			}
			_cellList.insert(std::make_pair(occt_solid.TShape(), new GeometryTopologyCell(shell_ptr, shell_ptr->reverseCopy())));
		}

		std::cout << "Address: " << _cellList.at(occt_solid.TShape()) << std::endl;
	}
}