/**
 * @file InputHDF5Adapter.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-08-30
 *
 * @brief [Header] Input adapter to HDF5 library
 */

#ifndef IO_INPUT_HDF5_ADAPTER_HPP
#define IO_INPUT_HDF5_ADAPTER_HPP

#include <memory>

#include "hdf5.h"

#include "geometry-topology/GeometryTopology.hpp"
#include "geometry-topology/GeometryTopologyCell.hpp"
#include "geometry-topology/GeometryTopologyShell.hpp"
#include "geometry-topology/GeometryTopologyFace.hpp"
#include "geometry-topology/GeometryTopologyWire.hpp"
#include "geometry-topology/GeometryTopologyEdge.hpp"
#include "geometry-topology/GeometryTopologyVertex.hpp"
#include "InputAdapter.hpp"

class InputHDF5Adapter : public InputAdapter {
public:
    /** Constructor of InputHDF5Adapter object
     */
    InputHDF5Adapter();
    InputHDF5Adapter(const char* hdf5_file_path);

    /** Destructor of InputHDF5Adapter object */
    ~InputHDF5Adapter();

    /** Method to deserialize HDF5 file's data to internal neutral data structure */
    std::shared_ptr<GeometryTopology> deserialize();

private:
    /** Attribute to store the HDF5 file path */
    const char* _hdf5FilePath;

    /** Attribute to store list of GeometryTopologyVertex pointer that have been created */
    std::vector<std::shared_ptr<GeometryTopologyVertex>> _vertexList;

    /** Attribute to store list of GeometryTopologyWire pointer that have been created */
    std::vector<std::shared_ptr<GeometryTopologyWire>> _wireList;

    /** Attribute to store list of GeometryTopologyShell pointer that have been created */
    std::vector<std::shared_ptr<GeometryTopologyShell>> _shellList;

    /** Method to read geometry dataset */
    void readGeometryDataset(hid_t parent_group);

    /** Method to read topology - by wires dataset */
    void readWireTopologyDataset(hid_t parent_group);

    /** Method to read topology - by shells dataset */
    void readShellTopologyDataset(hid_t parent_group);

};

#endif