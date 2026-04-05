/**
 * @file InputXDMFAdapter.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-08-30
 *
 * @brief [Header] Input adapter to XDMF library
 */

#ifndef IO_INPUT_XDMF_ADAPTER_HPP
#define IO_INPUT_XDMF_ADAPTER_HPP

#include <memory>

#include "geometry-topology/GeometryTopology.hpp"
#include "InputAdapter.hpp"

class InputXDMFAdapter : public InputAdapter {
public:
    /** Constructor of InputXDMFAdapter object
     */
    InputXDMFAdapter();
    InputXDMFAdapter(const char* xdmf_file_path, const char* hdf5_file_path = "auto.hdf5");

    /** Destructor of InputXDMFAdapter object */
    ~InputXDMFAdapter();

    /** Method to deserialize XDMF file's data to internal neutral data structure */
    std::shared_ptr<GeometryTopology> deserialize();

private:
    /** Attribute to store the XDMF file path */
    const char* _xdmfFilePath;

    /** Attribute to store the HDF5 file path */
    const char* _hdf5FilePath;

};

#endif