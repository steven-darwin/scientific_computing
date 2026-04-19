/**
 * @file InputXDMFAdapter.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-08-30
 *
 * @brief [Implementation] Input adapter to XDMF library
 */

#include <memory>
#include <string>

#include "geometry-topology/GeometryTopology.hpp"
#include "input-output/InputXDMFAdapter.hpp"
#include "input-output/InputHDF5Adapter.hpp"

InputXDMFAdapter::InputXDMFAdapter(std::string xdmf_file_name, std::string hdf5_file_name) {
    std::string xdmf_file_path = "../staging/" + xdmf_file_name + ".xmf";

    char* xdmf_buffer = new char[xdmf_file_path.length() + 1];
    std::memcpy(xdmf_buffer, xdmf_file_path.c_str(), xdmf_file_path.length());
    xdmf_buffer[xdmf_file_path.length()] = '\0';

    _xdmfFilePath = xdmf_buffer;

    std::string hdf5_file_path = "../staging/" + hdf5_file_name + ".h5";

    char* hdf5_buffer = new char[hdf5_file_path.length() + 1];
    std::memcpy(hdf5_buffer, hdf5_file_path.c_str(), hdf5_file_path.length());
    hdf5_buffer[hdf5_file_path.length()] = '\0';

    _hdf5FilePath = hdf5_buffer;
}

InputXDMFAdapter::~InputXDMFAdapter() {
    // TBA
}

std::shared_ptr<GeometryTopology> InputXDMFAdapter::deserialize() {
    InputHDF5Adapter hdf5Adapter = InputHDF5Adapter(_hdf5FilePath);
    return hdf5Adapter.deserialize();
}