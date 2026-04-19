/**
 * @file InputHDF5Adapter.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-08-30
 *
 * @brief [Implementation] Input adapter to HDF5 library
 */

#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

#include "hdf5.h"

#include "geometry-topology/GeometryTopology.hpp"
#include "input-output/InputHDF5Adapter.hpp"

InputHDF5Adapter::InputHDF5Adapter(std::string hdf5_file_name) {
    std::string hdf5_file_path = hdf5_file_name;

    char* hdf5_buffer = new char[hdf5_file_path.length() + 1];
    std::memcpy(hdf5_buffer, hdf5_file_path.c_str(), hdf5_file_path.length());
    hdf5_buffer[hdf5_file_path.length()] = '\0';

    _hdf5FilePath = hdf5_buffer;
}

InputHDF5Adapter::~InputHDF5Adapter() {
    // TBA
}

std::shared_ptr<GeometryTopology> InputHDF5Adapter::deserialize() {
    std::cout << _hdf5FilePath << std::endl;

    hid_t file = H5Fopen(_hdf5FilePath, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t mesh_group = H5Gopen(file, "mesh", H5P_DEFAULT);

    readGeometryDataset(mesh_group);
    readWireTopologyDataset(mesh_group);
    readShellTopologyDataset(mesh_group);

    H5Gclose(mesh_group);
    H5Fclose(file);
    
    return _composite;
}

void InputHDF5Adapter::readGeometryDataset(hid_t parent_group) {
    std::cout << "----------------------------------------------------" << std::endl;

    // Deserialization
    std::vector<double> vertex_coordinate_buffer;
    
    hid_t geometry_dataset = H5Dopen(parent_group, "geometry", H5P_DEFAULT);
    hid_t geometry_dataspace = H5Dget_space(geometry_dataset);
    hid_t geometry_datatype = H5Dget_type(geometry_dataset);

    hsize_t geometry_dataspace_dim[2];
    H5Sget_simple_extent_dims(geometry_dataspace, geometry_dataspace_dim, nullptr);

    vertex_coordinate_buffer.resize(geometry_dataspace_dim[0] * geometry_dataspace_dim[1]);

    const hsize_t geometry_start[2] = { 0, 0 };
    const hsize_t geometry_stride[2] = { 1, geometry_dataspace_dim[1] };
    const hsize_t geometry_count[2] = { geometry_dataspace_dim[0], 1};
    const hsize_t geometry_block[2] = { 1, geometry_dataspace_dim[1] };

    H5Sselect_hyperslab(geometry_dataspace, H5S_SELECT_SET, geometry_start, geometry_stride, geometry_count, geometry_block);

    const hsize_t memory_dataspace_dim[1] = { vertex_coordinate_buffer.size() };
    const hsize_t memory_start[1] = { 0 };
    const hsize_t memory_stride[1] = { geometry_dataspace_dim[1] };
    const hsize_t memory_count[1] = { geometry_dataspace_dim[0] };
    const hsize_t memory_block[1] = { geometry_dataspace_dim[1] };

    hid_t memory_dataspace = H5Screate_simple(1, memory_dataspace_dim, nullptr);
    H5Sselect_hyperslab(memory_dataspace, H5S_SELECT_SET, memory_start, memory_stride, memory_count, memory_block);
    hid_t memory_datatype = H5Tcopy(H5T_NATIVE_DOUBLE);

    if (H5Dread(geometry_dataset, memory_datatype, memory_dataspace, geometry_dataspace, H5P_DEFAULT, vertex_coordinate_buffer.data()) < 0) {
        std::cout << "Geometry Dataset fetching is failed." << std::endl;
    }
    else {
        std::cout << "Geometry Dataset fetching is success." << std::endl;
    }

    // GeometryTopologyVertex Creation


    for (unsigned int i = 0; i < vertex_coordinate_buffer.size(); i++) {
        if (i % geometry_dataspace_dim[1] == 0) {
            std::cout << "Vertex " << _vertexList.size() << std::endl;
            _vertexList.push_back(std::make_shared<GeometryTopologyVertex>(vertex_coordinate_buffer[i], vertex_coordinate_buffer[i + 1], vertex_coordinate_buffer[i + 2]));
            std::cout << "Address: " << _vertexList[_vertexList.size() - 1] << std::endl;
        }
    }

    // Resource Cleaning
    H5Sclose(memory_dataspace);
    H5Tclose(memory_datatype);

    H5Sclose(geometry_dataspace);
    H5Tclose(geometry_datatype);
    
    H5Dclose(geometry_dataset);
}

void InputHDF5Adapter::readWireTopologyDataset(hid_t parent_group) {
    std::cout << "----------------------------------------------------" << std::endl;

    // Deserialization
    std::vector<unsigned int> vertex_on_wire_buffer;

    hid_t wire_topology_dataset = H5Dopen(parent_group, "topology_wire", H5P_DEFAULT);
    hid_t wire_topology_dataspace = H5Dget_space(wire_topology_dataset);
    hid_t wire_topology_datatype = H5Dget_type(wire_topology_dataset);

    hsize_t wire_topology_dataspace_dim[2];
    H5Sget_simple_extent_dims(wire_topology_dataspace, wire_topology_dataspace_dim, nullptr);

    vertex_on_wire_buffer.resize(wire_topology_dataspace_dim[0] * wire_topology_dataspace_dim[1]);

    const hsize_t wire_topology_start[2] = { 0, 0 };
    const hsize_t wire_topology_stride[2] = { 1, wire_topology_dataspace_dim[1] };
    const hsize_t wire_topology_count[2] = { wire_topology_dataspace_dim[0], 1 };
    const hsize_t wire_topology_block[2] = { 1, wire_topology_dataspace_dim[1] };

    H5Sselect_hyperslab(wire_topology_dataspace, H5S_SELECT_SET, wire_topology_start, wire_topology_stride, wire_topology_count, wire_topology_block);

    const hsize_t memory_dataspace_dim[1] = { vertex_on_wire_buffer.size() };
    const hsize_t memory_start[1] = { 0 };
    const hsize_t memory_stride[1] = { wire_topology_dataspace_dim[1] };
    const hsize_t memory_count[1] = { wire_topology_dataspace_dim[0] };
    const hsize_t memory_block[1] = { wire_topology_dataspace_dim[1] };

    hid_t memory_dataspace = H5Screate_simple(1, memory_dataspace_dim, nullptr);
    H5Sselect_hyperslab(memory_dataspace, H5S_SELECT_SET, memory_start, memory_stride, memory_count, memory_block);
    hid_t memory_datatype = H5Tcopy(H5T_NATIVE_INT);

    if (H5Dread(wire_topology_dataset, memory_datatype, memory_dataspace, wire_topology_dataspace, H5P_DEFAULT, vertex_on_wire_buffer.data()) < 0) {
        std::cout << "Wire Topology Dataset fetching is failed." << std::endl;
    }
    else {
        std::cout << "Wire Topology Dataset fetching is success." << std::endl;
    }

    // GeometryTopologyEdge and GeometryTopologyWire Creation
    std::unordered_map<std::string, std::shared_ptr<GeometryTopologyEdge>> edge_list;
    for (unsigned int i = 0; i < vertex_on_wire_buffer.size(); i++) {
        if (i % wire_topology_dataspace_dim[1] == 0) {
            std::shared_ptr<GeometryTopologyWire> wire = std::make_shared<GeometryTopologyWire>();
            unsigned int first_vertex_on_wire;

            for (unsigned int j = 0; j < wire_topology_dataspace_dim[1]; j++) {
                if (j == 0) {
                    first_vertex_on_wire = vertex_on_wire_buffer[i];
                }

                unsigned int first_vertex_on_edge, second_vertex_on_edge;
                if (j != wire_topology_dataspace_dim[1] - 1) {
                    first_vertex_on_edge = vertex_on_wire_buffer[i + j];
                    second_vertex_on_edge = vertex_on_wire_buffer[i + j + 1];
                }
                else {
                    first_vertex_on_edge = vertex_on_wire_buffer[i + j];
                    second_vertex_on_edge = first_vertex_on_wire;
                }

                std::string vertex_on_edge_forward_indices, vertex_on_edge_reverse_indices;
                vertex_on_edge_forward_indices = std::to_string(first_vertex_on_edge) + " " + std::to_string(second_vertex_on_edge);
                vertex_on_edge_reverse_indices = std::to_string(second_vertex_on_edge) + " " + std::to_string(first_vertex_on_edge);

                if (!edge_list.contains(vertex_on_edge_forward_indices) && !edge_list.contains(vertex_on_edge_reverse_indices)) {
                    std::cout << "Edge " << edge_list.size() << std::endl;
                    std::shared_ptr<GeometryTopologyEdge> edge = std::make_shared<GeometryTopologyEdge>(_vertexList[first_vertex_on_edge], _vertexList[second_vertex_on_edge]);
                    std::cout << "Address: " << edge << std::endl;

                    edge_list.insert(std::make_pair(vertex_on_edge_forward_indices, edge));

                    wire->addEdge(edge, 0);
                }
                else if (edge_list.contains(vertex_on_edge_forward_indices)) {
                    wire->addEdge(edge_list.at(vertex_on_edge_forward_indices), 0);
                }
                else if (edge_list.contains(vertex_on_edge_reverse_indices)) {
                    wire->addEdge(edge_list.at(vertex_on_edge_reverse_indices), 1);
                }
                else {
                    // do nothing
                }
            }

            _wireList.push_back(wire);
        }
    }

    // Resource Cleaning
    H5Sclose(memory_dataspace);
    H5Tclose(memory_datatype);

    H5Sclose(wire_topology_dataspace);
    H5Tclose(wire_topology_datatype);

    H5Dclose(wire_topology_dataset);
}

void InputHDF5Adapter::readShellTopologyDataset(hid_t parent_group) {
    std::cout << "----------------------------------------------------" << std::endl;

    // Deserialization
    std::vector<unsigned int> wire_on_shell_buffer;

    hid_t shell_topology_dataset = H5Dopen(parent_group, "topology_shell", H5P_DEFAULT);
    hid_t shell_topology_dataspace = H5Dget_space(shell_topology_dataset);
    hid_t shell_topology_datatype = H5Dget_type(shell_topology_dataset);

    hsize_t shell_topology_dataspace_dim[2];
    H5Sget_simple_extent_dims(shell_topology_dataspace, shell_topology_dataspace_dim, nullptr);

    wire_on_shell_buffer.resize(shell_topology_dataspace_dim[0] * shell_topology_dataspace_dim[1]);

    const hsize_t shell_topology_start[2] = { 0, 0 };
    const hsize_t shell_topology_stride[2] = { 1, shell_topology_dataspace_dim[1] };
    const hsize_t shell_topology_count[2] = { shell_topology_dataspace_dim[0], 1 };
    const hsize_t shell_topology_block[2] = { 1, shell_topology_dataspace_dim[1] };

    H5Sselect_hyperslab(shell_topology_dataspace, H5S_SELECT_SET, shell_topology_start, shell_topology_stride, shell_topology_count, shell_topology_block);

    const hsize_t memory_dataspace_dim[1] = { wire_on_shell_buffer.size() };
    const hsize_t memory_start[1] = { 0 };
    const hsize_t memory_stride[1] = { shell_topology_dataspace_dim[1] };
    const hsize_t memory_count[1] = { shell_topology_dataspace_dim[0] };
    const hsize_t memory_block[1] = { shell_topology_dataspace_dim[1] };

    hid_t memory_dataspace = H5Screate_simple(1, memory_dataspace_dim, nullptr);
    H5Sselect_hyperslab(memory_dataspace, H5S_SELECT_SET, memory_start, memory_stride, memory_count, memory_block);
    hid_t memory_datatype = H5Tcopy(H5T_NATIVE_INT);

    if (H5Dread(shell_topology_dataset, memory_datatype, memory_dataspace, shell_topology_dataspace, H5P_DEFAULT, wire_on_shell_buffer.data()) < 0) {
        std::cout << "Shell Topology Dataset fetching is failed." << std::endl;
    }
    else {
        std::cout << "Shell Topology Dataset fetching is success." << std::endl;
    }

    // GeometryTopologyFace, GeometryTopologyShell, GeometryTopologyCell, and GeometryTopologyComposite Creation
    std::vector<std::shared_ptr<GeometryTopologyFace>> face_list;
    for (auto wire : _wireList) {
        std::cout << "Face " << face_list.size() << std::endl;
        face_list.push_back(std::make_shared<GeometryTopologyFace>(wire, wire->reverseCopy()));
        std::cout << "Address: " << face_list[face_list.size() - 1] << std::endl;
    }

    for (unsigned int i = 0; i < wire_on_shell_buffer.size(); i++) {
        if (i % shell_topology_dataspace_dim[1] == 0) {
            std::shared_ptr<GeometryTopologyShell> shell = std::make_shared<GeometryTopologyShell>();

            for (unsigned int j = 0; j < shell_topology_dataspace_dim[1]; j++) {
                shell->addFace(face_list[wire_on_shell_buffer[i + j]], 0);
            }

            _shellList.push_back(shell);
        }
    }

    _composite = std::make_shared<GeometryTopologyComposite>();
    for (auto shell : _shellList) {
        _composite->addCell(std::make_shared<GeometryTopologyCell>(shell, shell->reverseCopy()));
    }

    // Resource Cleaning
    H5Sclose(memory_dataspace);
    H5Tclose(memory_datatype);

    H5Sclose(shell_topology_dataspace);
    H5Tclose(shell_topology_datatype);

    H5Dclose(shell_topology_dataset);
}