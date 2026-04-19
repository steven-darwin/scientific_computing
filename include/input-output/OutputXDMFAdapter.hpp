/**
 * @file OutputXDMFAdapter.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-08-30
 *
 * @brief [Header] Output adapter to XDMF library
 */

#ifndef IO_OUTPUT_XDMF_ADAPTER_HPP
#define IO_OUTPUT_XDMF_ADAPTER_HPP

#include <string>
#include <memory>
#include <vector>
#include <tuple>

#include "hdf5.h"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfAttribute.hpp"

#include "geometry-topology/GeometryTopology.hpp"
#include "input-output/OutputAdapter.hpp"

class OutputXDMFAdapter : public OutputAdapter {
public:
    /** Constructor of OutputXDMFAdapter object
     */
    OutputXDMFAdapter();
    OutputXDMFAdapter(std::string xdmf_file_name, std::string hdf5_file_name = "default");

    /** Destructor of OutputXDMFAdapter object */
    ~OutputXDMFAdapter();

    /** Method to serialize internal neutral data structure to XDMF file's data */
    void serialize(std::shared_ptr<GeometryTopology> neutral_geometry_topology);

    /** Method to store custom attribute data before it is written in XDMF file */
    void addAttribute(std::string attribute_name, std::shared_ptr<std::vector<std::vector<double>>> attribute_data, unsigned int data_inside_element_count);
private:
    /** Attribute to store the XDMF file path */
    const char* _xdmfFilePath;

    /** Attribute to store the HDF5 file path */
    const char* _hdf5FilePath;

    /** Method to handle geometry data */
    shared_ptr<XdmfGeometry> handleGeometry(hid_t parent_group);

    /** Method to handle wire topology data */
    shared_ptr<XdmfTopology> handleWireTopology(hid_t parent_group);

    /** Method to handle shell topology data */
    shared_ptr<XdmfAttribute> handleShellTopology(hid_t parent_group);
    
    /** Attribute to store custom attribute data */
    std::vector<std::tuple<std::string, std::shared_ptr<std::vector<std::vector<double>>>, unsigned int>> _attributeList;

    /** Method to handle custom attribute data */
    shared_ptr<XdmfAttribute> handleCustomAttribute(hid_t parent_group, std::string attribute_name, GeometryTopology::Type center_type, std::string hdf5_group_path);
};

#endif