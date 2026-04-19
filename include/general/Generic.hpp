/**
 * @file Generic.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-09-21
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] --- TBA ---
 */

#ifndef GENERIC_HPP
#define GENERIC_HPP

#include <memory>
#include <string>

#include "geometry-topology/GeometryTopology.hpp"

#include "input-output/InputOutputAdapter.hpp"

namespace scmp {
    /** Enumeration of supported File Extension */
    enum FileExtension { STEP, XDMF, HDF5 };
    
    /** Struct of Adapter metadata */
    struct AdapterInfo {
        scmp::FileExtension _fileExtension;
        std::string _filePath;
        std::shared_ptr<InputOutputAdapter> _adapterObj;
        std::shared_ptr<GeometryTopology> _neutralGeometryTopology;
    };

    namespace mesh {
        /** Enumeration of supported Mesh Strategy */
        enum MeshStrategy { TRANSFINITE_INTERPOLATION, DELAUNAY };
    }

    namespace vszr {
        /** Enumeration of supported command type */
        enum CommandType { GRAPHICS, PRESENTATION, TRANSFER, COMPUTE };
    }
}

#endif