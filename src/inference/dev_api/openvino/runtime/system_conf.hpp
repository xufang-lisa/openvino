// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

/**
 * @brief Abstraction over platform specific implementations
 * @file openvino/runtime/system_conf.hpp
 */

#pragma once

#include <vector>

#include "openvino/runtime/common.hpp"

namespace ov {

/**
 * @brief      Checks whether OpenMP environment variables are defined
 * @ingroup    ov_dev_api_system_conf
 *
 * @param[in]  include_omp_num_threads  Indicates if the omp number threads is included
 * @return     `True` if any OpenMP environment variable is defined, `false` otherwise
 */
OPENVINO_RUNTIME_API bool check_open_mp_env_vars(bool include_omp_num_threads = true);

/**
 * @brief      Returns available CPU NUMA nodes (on Linux, and Windows [only with TBB], single node is assumed on all
 * other OSes)
 * @ingroup    ov_dev_api_system_conf
 * @return     NUMA nodes
 */
OPENVINO_RUNTIME_API std::vector<int> get_available_numa_nodes();

/**
 * @brief      Returns available CPU cores types (on Linux, and Windows) and ONLY with TBB, single core type is assumed
 * otherwise
 * @ingroup    ov_dev_api_system_conf
 * @return     Vector of core types
 */
OPENVINO_RUNTIME_API std::vector<int> get_available_cores_types();

/**
 * @brief      Returns number of CPU physical cores on Linux/Windows (which is considered to be more performance
 * friendly for servers) (on other OSes it simply relies on the original parallel API of choice, which usually uses the
 * logical cores). call function with 'false' to get #phys cores of all types call function with 'true' to get #phys
 * 'Big' cores number of 'Little' = 'all' - 'Big'
 * @ingroup    ov_dev_api_system_conf
 * @param[in]  big_cores_only Additionally limits the number of reported cores to the 'Big' cores only.
 * @return     Number of physical CPU cores.
 */
OPENVINO_RUNTIME_API int get_number_of_cpu_cores(bool big_cores_only = false);

/**
 * @brief      Returns number of CPU logical cores on Linux/Windows (on other OSes it simply relies on the original
 * parallel API of choice, which uses the 'all' logical cores). call function with 'false' to get #logical cores of
 * all types call function with 'true' to get #logical 'Big' cores number of 'Little' = 'all' - 'Big'
 * @ingroup    ov_dev_api_system_conf
 * @param[in]  big_cores_only Additionally limits the number of reported cores to the 'Big' cores only.
 * @return     Number of logical CPU cores.
 */
OPENVINO_RUNTIME_API int get_number_of_logical_cpu_cores(bool big_cores_only = false);

/**
 * @brief      Checks whether CPU supports SSE 4.2 capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is SSE 4.2 instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_sse42();

/**
 * @brief      Checks whether CPU supports AVX capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is AVX instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_avx();

/**
 * @brief      Checks whether CPU supports AVX2 capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is AVX2 instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_avx2();

/**
 * @brief      Checks whether CPU supports AVX 512 capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is AVX512F (foundation) instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_avx512f();

/**
 * @brief      Checks whether CPU supports AVX 512 capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is AVX512F, AVX512BW, AVX512DQ instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_avx512_core();

/**
 * @brief      Checks whether CPU supports AVX 512 VNNI capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is AVX512F, AVX512BW, AVX512DQ, AVX512_VNNI instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_avx512_core_vnni();

/**
 * @brief      Checks whether CPU supports BFloat16 capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is tAVX512_BF16 instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_bfloat16();

/**
 * @brief      Checks whether CPU supports AMX int8 capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is tAMX_INT8 instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_avx512_core_amx_int8();

/**
 * @brief      Checks whether CPU supports AMX bf16 capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is tAMX_BF16 instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_avx512_core_amx_bf16();

/**
 * @brief      Checks whether CPU supports AMX capability
 * @ingroup    ov_dev_api_system_conf
 * @return     `True` is tAMX_INT8 or tAMX_BF16 instructions are available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool with_cpu_x86_avx512_core_amx();

/**
 * @brief      Checks whether cpu_mapping Available
 * @ingroup    ie_dev_api_system_conf
 * @return     `True` is CPU mapping is available, `false` otherwise
 */
OPENVINO_RUNTIME_API bool is_cpu_map_available();

/**
 * @brief      Get number of numa nodes
 * @ingroup    ie_dev_api_system_conf
 * @return     Number of numa nodes
 */
OPENVINO_RUNTIME_API int get_num_numa_nodes();

/**
 * @brief      Returns a table of number of processor types on Linux/Windows
 * @ingroup    ie_dev_api_system_conf
 * @return     A table about number of CPU cores of different types defined with ColumnOfProcessorTypeTable
 * The following are two example of processor type table.
 *  1. Processor table of two socket CPUs XEON server
 *  ALL_PROC | MAIN_CORE_PROC | EFFICIENT_CORE_PROC | HYPER_THREADING_PROC
 *     96            48                 0                       48          // Total number of two sockets
 *     48            24                 0                       24          // Number of socket one
 *     48            24                 0                       24          // Number of socket two
 *
 * 2. Processor table of one socket CPU desktop
 *  ALL_PROC | MAIN_CORE_PROC | EFFICIENT_CORE_PROC | HYPER_THREADING_PROC
 *     32            8                 16                       8           // Total number of one socket
 */
OPENVINO_RUNTIME_API std::vector<std::vector<int>> get_proc_type_table();

/**
 * @enum       ColumnOfProcessorTypeTable
 * @brief      This enum contains definition of each columns in processor type table which bases on cpu core types. Will
 * extend to support other CPU core type like ARM.
 *
 * The following are two example of processor type table.
 *  1. Processor table of two socket CPUs XEON server
 *
 *  ALL_PROC | MAIN_CORE_PROC | EFFICIENT_CORE_PROC | HYPER_THREADING_PROC
 *     96            48                 0                       48          // Total number of two sockets
 *     48            24                 0                       24          // Number of socket one
 *     48            24                 0                       24          // Number of socket two
 *
 * 2. Processor table of one socket CPU desktop
 *
 *  ALL_PROC | MAIN_CORE_PROC | EFFICIENT_CORE_PROC | HYPER_THREADING_PROC
 *     32            8                 16                       8           // Total number of one socket
 */
enum ColumnOfProcessorTypeTable {
    ALL_PROC = 0,              //!< All processors, regardless of backend cpu
    MAIN_CORE_PROC = 1,        //!< Processor based on physical core of Intel Performance-cores
    EFFICIENT_CORE_PROC = 2,   //!< Processor based on Intel Efficient-cores
    HYPER_THREADING_PROC = 3,  //!< Processor based on logical core of Intel Performance-cores
    PROC_TYPE_TABLE_SIZE = 4   //!< Size of processor type table
};

/**
 * @enum       ProcessorUseStatus
 * @brief      Definition of CPU_MAP_USED_FLAG column in CPU mapping table.
 */
enum ProcessorUseStatus {
    NOT_USED = -1,           //!< Processor is not bound to thread
    CPU_USED = 1,            //!< CPU is in using
    PLUGIN_USED_START = 100  //!< Plugin other than CPU needs to use. If more GPUs use CPUs, the CPU_MAP_USED_FLAG is
                             //!< accumulated from PLUGIN_USED_START. For example: GPU.0:100, GPU.1:101
};

/**
 * @brief      Get and reserve available cpu ids
 * @ingroup    ie_dev_api_system_conf
 * @param[in]  streams_info_table streams information table.
 * @return     Array of available cpu ids.
 */
OPENVINO_RUNTIME_API std::vector<std::vector<int>> reserve_available_cpus(
    const std::vector<std::vector<int>> streams_info_table);

/**
 * @brief      Set CPU_MAP_USED_FLAG of cpu_mapping
 * @ingroup    ie_dev_api_system_conf
 * @param[in]  cpu_ids cpus in cpu_mapping.
 * @param[in]  used update CPU_MAP_USED_FLAG of cpu_mapping with this flag bit
 */
OPENVINO_RUNTIME_API void set_cpu_used(const std::vector<int>& cpu_ids, const int used);

/**
 * @enum       ColumnOfCPUMappingTable
 * @brief      This enum contains definition of each columns in CPU mapping table which use processor id as index.
 *
 * GROUP_ID is generated according to the following rules.
 *  1. If one MAIN_CORE_PROC and one HYPER_THREADING_PROC are based on same Performance-cores, they are in one group.
 *  2. If some EFFICIENT_CORE_PROC share one L2 cachle, they are in one group.
 *  3. There are no duplicate group IDs in the system
 *
 * The following is the example of CPU mapping table.
 *  1. Four processors of two Pcore
 *  2. Four processors of four Ecores shared L2 cache
 *
 *  PROCESSOR_ID | SOCKET_ID | CORE_ID | CORE_TYPE | GROUP_ID | Used
 *       0             0          0          3          0        0
 *       1             0          0          1          0        0
 *       2             0          1          3          1        0
 *       3             0          1          1          1        0
 *       4             0          2          2          2        0
 *       5             0          3          2          2        0
 *       6             0          4          2          2        0
 *       7             0          5          2          2        0
 */
enum ColumnOfCPUMappingTable {
    CPU_MAP_PROCESSOR_ID = 0,  //!< column for processor id of the processor
    CPU_MAP_SOCKET_ID = 1,     //!< column for socket id of the processor
    CPU_MAP_CORE_ID = 2,       //!< column for hardware core id of the processor
    CPU_MAP_CORE_TYPE = 3,     //!< column for CPU core type corresponding to the processor
    CPU_MAP_GROUP_ID = 4,      //!< column for group id to the processor. Processors in one group have dependency.
    CPU_MAP_USED_FLAG = 5,     //!< column for resource management of the processor
    CPU_MAP_TABLE_SIZE = 6     //!< Size of CPU mapping table
};

#define CPU_TABLE_PRINT(table_name, table, table_log)                 \
    table_log = table_name;                                           \
    table_log += " is {";                                             \
    for (int i = 0; i < static_cast<int>(table.size()); i++) {        \
        table_log += "{";                                             \
        for (int j = 0; j < static_cast<int>(table[i].size()); j++) { \
            table_log += std::to_string(table[i][j]);                 \
            table_log += ",";                                         \
        }                                                             \
        if (!table[i].empty()) {                                      \
            table_log.pop_back();                                     \
        }                                                             \
        table_log += "},";                                            \
    }                                                                 \
    if (!table.empty()) {                                             \
        table_log.pop_back();                                         \
    }                                                                 \
    table_log += "}";

}  // namespace ov
