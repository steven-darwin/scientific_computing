/**
 * @file OutputHDF5Adapter.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-08-30
 *
 * @brief [Implementation] Output adapter to HDF5 library
 */

#include <memory>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "hdf5.h"

#include "geometry-topology/GeometryTopology.hpp"
#include "geometry-topology/GeometryTopologyVertex.hpp"
#include "input-output/OutputHDF5Adapter.hpp"

OutputHDF5Adapter::OutputHDF5Adapter(const char* hdf5_file_path) {
    _hdf5FilePath = hdf5_file_path;
}

OutputHDF5Adapter::~OutputHDF5Adapter() {
    // TBA
}

void OutputHDF5Adapter::serialize(std::shared_ptr<GeometryTopology> neutral_geometry_topology) {
    hid_t file = H5Fcreate(_hdf5FilePath, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t mesh_group = H5Gcreate(file, "mesh", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    writeGeometryDataset(neutral_geometry_topology, mesh_group);
    writeWireTopologyDataset(neutral_geometry_topology, mesh_group);
    writeShellTopologyDataset(neutral_geometry_topology, mesh_group);

    H5Gclose(mesh_group);
    H5Fclose(file);
}

void OutputHDF5Adapter::writeGeometryDataset(std::shared_ptr<GeometryTopology> neutral_geometry_topology, hid_t parent_group) {
    
    // Preparation
    neutral_geometry_topology->getDescendants(_vertexList, GeometryTopology::Type::VERTEX);

    std::vector<double> vertex_coordinate_buffer;
    for (unsigned int i = 0; i < _vertexList.size(); i++) {
        for (auto vertex_iter = _vertexList.begin(); vertex_iter != _vertexList.end(); vertex_iter++) {
            if (i == (*vertex_iter).second) {
                // (*iter) need to be downcasted from GeometryTopology to GeometryTopologyVertex so getCoordinate method can be called
                vertex_coordinate_buffer.push_back(dynamic_cast<GeometryTopologyVertex*>((*vertex_iter).first.get())->getCoordinate(GeometryTopologyVertex::Coordinate::X));
                vertex_coordinate_buffer.push_back(dynamic_cast<GeometryTopologyVertex*>((*vertex_iter).first.get())->getCoordinate(GeometryTopologyVertex::Coordinate::Y));
                vertex_coordinate_buffer.push_back(dynamic_cast<GeometryTopologyVertex*>((*vertex_iter).first.get())->getCoordinate(GeometryTopologyVertex::Coordinate::Z));
                break;
            }
        }
    }

    // Serialization
    const hsize_t memory_dataspace_dim[1] = { vertex_coordinate_buffer.size() };
    const hsize_t memory_start[1] = { 0 };
    const hsize_t memory_stride[1] = { 3 };
    const hsize_t memory_count[1] = { _vertexList.size() };
    const hsize_t memory_block[1] = { 3 };

    hid_t memory_dataspace = H5Screate_simple(1, memory_dataspace_dim, nullptr);
    H5Sselect_hyperslab(memory_dataspace, H5S_SELECT_SET, memory_start, memory_stride, memory_count, memory_block);
    hid_t memory_datatype = H5Tcopy(H5T_NATIVE_DOUBLE);

    const hsize_t geometry_dataspace_dim[2] = { _vertexList.size(), 3 };
    const hsize_t geometry_start[2] = { 0, 0 };
    const hsize_t geometry_stride[2] = { 1, 3 };
    const hsize_t geometry_count[2] = { _vertexList.size(), 1 };
    const hsize_t geometry_block[2] = { 1, 3 };

    hid_t geometry_dataspace = H5Screate_simple(2, geometry_dataspace_dim, nullptr);
    H5Sselect_hyperslab(geometry_dataspace, H5S_SELECT_SET, geometry_start, geometry_stride, geometry_count, geometry_block);
    hid_t geometry_datatype = H5Tcopy(H5T_NATIVE_DOUBLE);

    hid_t geometry_dataset = H5Dcreate(parent_group, "geometry", geometry_datatype, geometry_dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (H5Dwrite(geometry_dataset, memory_datatype, memory_dataspace, geometry_dataspace, H5P_DEFAULT, vertex_coordinate_buffer.data()) < 0) {
        std::cout << "Geometry Dataset creation is failed." << std::endl;
    }
    else {
        std::cout << "Geometry Dataset creation is success." << std::endl;
    }

    // Resource Cleaning
    H5Sclose(memory_dataspace);
    H5Tclose(memory_datatype);

    H5Sclose(geometry_dataspace);
    H5Tclose(geometry_datatype);

    H5Dclose(geometry_dataset);
}

void OutputHDF5Adapter::writeWireTopologyDataset(std::shared_ptr<GeometryTopology> neutral_geometry_topology, hid_t parent_group) {

    // Preparation
    neutral_geometry_topology->getDescendants(_wireList, GeometryTopology::Type::WIRE);

    std::vector<unsigned int> vertex_on_wire_buffer;
    for (unsigned int i = 0; i < _wireList.size(); i++) {
        for (auto wire_iter = _wireList.begin(); wire_iter != _wireList.end(); wire_iter++) {
            if (i * 2 == (*wire_iter).second) {
                std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> edge_on_wire;
                ((*wire_iter).first)->getDescendants(edge_on_wire, GeometryTopology::Type::EDGE);

                std::shared_ptr<GeometryTopology> next_vertex;
                std::unordered_set<std::shared_ptr<GeometryTopology>> traversed_edge;

                for (unsigned int j = 0; j < edge_on_wire.size(); j++) {
                    for (auto edge_iter = edge_on_wire.begin(); edge_iter != edge_on_wire.end(); edge_iter++) {
                        if (!traversed_edge.contains((*edge_iter).first)) {
                            std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> vertex_on_edge;
                            ((*edge_iter).first)->getDescendants(vertex_on_edge, GeometryTopology::Type::VERTEX);

                            if (j == 0) {
                                for (auto vertex_iter = vertex_on_edge.begin(); vertex_iter != vertex_on_edge.end(); vertex_iter++) {
                                    if (vertex_iter == vertex_on_edge.begin()) {
                                        vertex_on_wire_buffer.push_back(_vertexList.at((*vertex_iter).first));
                                    }
                                    else {
                                        next_vertex = (*vertex_iter).first;
                                    }
                                }
                                traversed_edge.insert((*edge_iter).first);
                                break;
                            }
                            else {
                                if (vertex_on_edge.contains(next_vertex)) {
                                    std::shared_ptr<GeometryTopology> temp_next_vertex;
                                    for (auto vertex_iter = vertex_on_edge.begin(); vertex_iter != vertex_on_edge.end(); vertex_iter++) {
                                        if ((*vertex_iter).first == next_vertex) {
                                            vertex_on_wire_buffer.push_back(_vertexList.at((*vertex_iter).first));
                                        }
                                        else {
                                            temp_next_vertex = (*vertex_iter).first;
                                        }
                                    }
                                    next_vertex = temp_next_vertex;
                                    traversed_edge.insert((*edge_iter).first);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Serialization
    const hsize_t memory_dataspace_dim[1] = { vertex_on_wire_buffer.size() };
    const hsize_t memory_start[1] = { 0 };
    const hsize_t memory_stride[1] = { 4 };
    const hsize_t memory_count[1] = { _wireList.size() / 2 };
    const hsize_t memory_block[1] = { 4 };

    hid_t memory_dataspace = H5Screate_simple(1, memory_dataspace_dim, nullptr);
    H5Sselect_hyperslab(memory_dataspace, H5S_SELECT_SET, memory_start, memory_stride, memory_count, memory_block);
    hid_t memory_datatype = H5Tcopy(H5T_NATIVE_INT);

    const hsize_t wire_topology_dataspace_dim[2] = { _wireList.size() / 2, 4 };
    const hsize_t wire_topology_start[2] = { 0, 0 };
    const hsize_t wire_topology_stride[2] = { 1, 4 };
    const hsize_t wire_topology_count[2] = { _wireList.size() / 2, 1 };
    const hsize_t wire_topology_block[2] = { 1, 4 };

    hid_t wire_topology_dataspace = H5Screate_simple(2, wire_topology_dataspace_dim, nullptr);
    H5Sselect_hyperslab(wire_topology_dataspace, H5S_SELECT_SET, wire_topology_start, wire_topology_stride, wire_topology_count, wire_topology_block);
    hid_t wire_topology_datatype = H5Tcopy(H5T_NATIVE_INT);

    hid_t wire_topology_dataset = H5Dcreate(parent_group, "topology_wire", wire_topology_datatype, wire_topology_dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (H5Dwrite(wire_topology_dataset, memory_datatype, memory_dataspace, wire_topology_dataspace, H5P_DEFAULT, vertex_on_wire_buffer.data()) < 0) {
        std::cout << "Wire Topology Dataset creation is failed." << std::endl;
    }
    else {
        std::cout << "Wire Topology Dataset creation is success." << std::endl;
    }

    // Resource Cleaning
    H5Sclose(wire_topology_dataspace);
    H5Tclose(wire_topology_datatype);
    H5Dclose(wire_topology_dataset);
}

void OutputHDF5Adapter::writeShellTopologyDataset(std::shared_ptr<GeometryTopology> neutral_geometry_topology, hid_t parent_group) {

    // Preparation
    neutral_geometry_topology->getDescendants(_shellList, GeometryTopology::Type::SHELL);
    
    std::vector<unsigned int> wire_on_shell_buffer;
    for (unsigned int i = 0; i < _shellList.size(); i++) {
        for (auto shell_iter = _shellList.begin(); shell_iter != _shellList.end(); shell_iter++) {
            if (i * 2 == (*shell_iter).second) {
                std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> wire_on_shell;
                ((*shell_iter).first)->getDescendants(wire_on_shell, GeometryTopology::Type::WIRE);

                for (auto wire_iter = wire_on_shell.begin(); wire_iter != wire_on_shell.end(); wire_iter++) {
                    if (_wireList.at((*wire_iter).first) % 2 == 0) {
                        wire_on_shell_buffer.push_back(_wireList.at((*wire_iter).first) / 2);
                    }
                }
            }
        }
    }

    // Serialization
    const hsize_t memory_dataspace_dim[1] = { wire_on_shell_buffer.size() };
    const hsize_t memory_start[1] = { 0 };
    const hsize_t memory_stride[1] = { 6 };
    const hsize_t memory_count[1] = { _shellList.size() / 2 };
    const hsize_t memory_block[1] = { 6 };

    hid_t memory_dataspace = H5Screate_simple(1, memory_dataspace_dim, nullptr);
    H5Sselect_hyperslab(memory_dataspace, H5S_SELECT_SET, memory_start, memory_stride, memory_count, memory_block);
    hid_t memory_datatype = H5Tcopy(H5T_NATIVE_INT);

    const hsize_t shell_topology_dataspace_dim[2] = { _shellList.size() / 2, 6 };
    const hsize_t shell_topology_start[2] = { 0, 0 };
    const hsize_t shell_topology_stride[2] = { 1, 6 };
    const hsize_t shell_topology_count[2] = { _shellList.size() / 2, 1 };
    const hsize_t shell_topology_block[2] = { 1, 6 };

    hid_t shell_topology_dataspace = H5Screate_simple(2, shell_topology_dataspace_dim, nullptr);
    H5Sselect_hyperslab(shell_topology_dataspace, H5S_SELECT_SET, shell_topology_start, shell_topology_stride, shell_topology_count, shell_topology_block);
    hid_t shell_topology_datatype = H5Tcopy(H5T_NATIVE_INT);

    hid_t shell_topology_dataset = H5Dcreate(parent_group, "topology_shell", shell_topology_datatype, shell_topology_dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (H5Dwrite(shell_topology_dataset, memory_datatype, memory_dataspace, shell_topology_dataspace, H5P_DEFAULT, wire_on_shell_buffer.data()) < 0) {
        std::cout << "Shell Topology Dataset creation is failed." << std::endl;
    }
    else {
        std::cout << "Shell Topology Dataset creation is success." << std::endl;
    }

    // Resource Cleaning
    H5Sclose(shell_topology_dataspace);
    H5Tclose(shell_topology_datatype);
    H5Dclose(shell_topology_dataset);
}