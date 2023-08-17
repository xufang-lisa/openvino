# Copyright (C) 2018-2023 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#

include(GNUInstallDirs)

#
# ov_archive_cpack_set_dirs()
#
# Set directories for ARCHIVE cpack
#
macro(ov_archive_cpack_set_dirs)
    # common "archive" package locations
    # TODO: move current variables to OpenVINO specific locations
    set(OV_CPACK_INCLUDEDIR runtime/include)
    set(OV_CPACK_IE_CMAKEDIR runtime/cmake)
    set(OV_CPACK_NGRAPH_CMAKEDIR runtime/cmake)
    set(OV_CPACK_OPENVINO_CMAKEDIR runtime/cmake)
    set(OV_CPACK_DOCDIR docs)
    set(OV_CPACK_LICENSESDIR licenses)
    set(OV_CPACK_SAMPLESDIR samples)
    set(OV_CPACK_WHEELSDIR tools)
    set(OV_CPACK_TOOLSDIR tools)
    set(OV_CPACK_DEVREQDIR tools)
    set(OV_CPACK_PYTHONDIR python)

    if(WIN32)
        set(OV_CPACK_LIBRARYDIR runtime/lib/${ARCH_FOLDER}/$<CONFIG>)
        set(OV_CPACK_RUNTIMEDIR runtime/bin/${ARCH_FOLDER}/$<CONFIG>)
        set(OV_CPACK_ARCHIVEDIR runtime/lib/${ARCH_FOLDER}/$<CONFIG>)
        set(OV_WHEEL_RUNTIMEDIR runtime/bin/${ARCH_FOLDER}/Release)
    elseif(APPLE)
        set(OV_CPACK_LIBRARYDIR runtime/lib/${ARCH_FOLDER}/$<CONFIG>)
        set(OV_CPACK_RUNTIMEDIR runtime/lib/${ARCH_FOLDER}/$<CONFIG>)
        set(OV_CPACK_ARCHIVEDIR runtime/lib/${ARCH_FOLDER}/$<CONFIG>)
        set(OV_WHEEL_RUNTIMEDIR runtime/lib/${ARCH_FOLDER}/Release)
    else()
        set(OV_CPACK_LIBRARYDIR runtime/lib/${ARCH_FOLDER})
        set(OV_CPACK_RUNTIMEDIR runtime/lib/${ARCH_FOLDER})
        set(OV_CPACK_ARCHIVEDIR runtime/lib/${ARCH_FOLDER})
        set(OV_WHEEL_RUNTIMEDIR ${OV_CPACK_RUNTIMEDIR})
    endif()
    set(OV_CPACK_PLUGINSDIR ${OV_CPACK_RUNTIMEDIR})

    # for BW compatibility
    set(IE_CPACK_LIBRARY_PATH ${OV_CPACK_LIBRARYDIR})
    set(IE_CPACK_RUNTIME_PATH ${OV_CPACK_RUNTIMEDIR})
    set(IE_CPACK_ARCHIVE_PATH ${OV_CPACK_ARCHIVEDIR})
endmacro()

ov_archive_cpack_set_dirs()

#
# Override include / exclude rules for components
# This is required to exclude some files from installation
# (e.g. archive packages don't require python_package component)
#

macro(ov_define_component_include_rules)
    # core components
    unset(OV_CPACK_COMP_CORE_EXCLUDE_ALL)
    unset(OV_CPACK_COMP_CORE_C_EXCLUDE_ALL)
    unset(OV_CPACK_COMP_CORE_DEV_EXCLUDE_ALL)
    unset(OV_CPACK_COMP_CORE_C_DEV_EXCLUDE_ALL)
    # licensing
    unset(OV_CPACK_COMP_LICENSING_EXCLUDE_ALL)
    # samples
    unset(OV_CPACK_COMP_CPP_SAMPLES_EXCLUDE_ALL)
    unset(OV_CPACK_COMP_C_SAMPLES_EXCLUDE_ALL)
    unset(OV_CPACK_COMP_PYTHON_SAMPLES_EXCLUDE_ALL)
    # python
    unset(OV_CPACK_COMP_PYTHON_OPENVINO_EXCLUDE_ALL)
    unset(OV_CPACK_COMP_BENCHMARK_APP_EXCLUDE_ALL)
    unset(OV_CPACK_COMP_OVC_EXCLUDE_ALL)
    set(OV_CPACK_COMP_PYTHON_OPENVINO_PACKAGE_EXCLUDE_ALL EXCLUDE_FROM_ALL)
    unset(OV_CPACK_COMP_PYTHON_WHEELS_EXCLUDE_ALL)
    # tools
    set(OV_CPACK_COMP_OPENVINO_DEV_REQ_FILES_EXCLUDE_ALL EXCLUDE_FROM_ALL)
    unset(OV_CPACK_COMP_DEPLOYMENT_MANAGER_EXCLUDE_ALL)
    # scripts
    unset(OV_CPACK_COMP_INSTALL_DEPENDENCIES_EXCLUDE_ALL)
    unset(OV_CPACK_COMP_SETUPVARS_EXCLUDE_ALL)
endmacro()

ov_define_component_include_rules()

# New in version 3.18
set(CPACK_ARCHIVE_THREADS 8)
# multiple packages are generated
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
