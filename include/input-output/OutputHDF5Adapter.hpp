/**
 * @file OutputHDF5Adapter.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-08-30
 *
 * @brief [Header] Output adapter to HDF5 library
 */

#ifndef IO_OUTPUT_HDF5_ADAPTER_HPP
#define IO_OUTPUT_HDF5_ADAPTER_HPP

#include <memory>

#include "hdf5.h"

#include "geometry-topology/GeometryTopology.hpp"
#include "OutputAdapter.hpp"

class OutputHDF5Adapter : public OutputAdapter {
public:
    /** Constructor of OutputHDF5Adapter object
     */
    OutputHDF5Adapter();
    OutputHDF5Adapter(const char* hdf5_file_path);

    /** Destructor of OutputHDF5Adapter object */
    ~OutputHDF5Adapter();

    /** Method to serialize internal neutral data structure to HDF5 file's data */
    void serialize(std::shared_ptr<GeometryTopology> neutral_geometry_topology);

private:
    /** Attribute to store the HDF5 file path */
    const char* _hdf5FilePath;

    /** Attribute to store the vertex descendant of particular GeometryTopology object */
    std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> _vertexList;

    /** Attribute to store the wire descendant of particular GeometryTopology object */
    std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> _wireList;

    /** Attribute to store the shell descendant of particular GeometryTopology object */
    std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> _shellList;

    /** Method to write geometry dataset */
    void writeGeometryDataset(std::shared_ptr<GeometryTopology> neutral_geometry_topology, hid_t parent_group);

    /** Method to write topology - by wires dataset */
    void writeWireTopologyDataset(std::shared_ptr<GeometryTopology> neutral_geometry_topology, hid_t parent_group);

    /** Method to write topology - by shells dataset */
    void writeShellTopologyDataset(std::shared_ptr<GeometryTopology> neutral_geometry_topology, hid_t parent_group);
};

#endif