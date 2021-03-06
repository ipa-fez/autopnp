#
# Copyright (c) 2011-2014, fortiss GmbH.
# Licensed under the Apache License, Version 2.0.
# 
# Use, modification and distribution are subject to the terms specified
# in the accompanying license file LICENSE.txt located at the root directory
# of this software distribution. A copy is available at
# http://chromosome.fortiss.org/.
#
# This file is part of CHROMOSOME.
#
# $Id$
#         This file has been generated by the CHROMOSOME Modeling Tool (XMT)
#         (fortiss GmbH).

# This is the node-specific CMakeLists.txt for the demonstrator node of the
# AutoPnP project. Use it for building the node in a build
# system of its own. This is meaningful if the node uses a different compiler
# toolchain than other nodes of the same project. If all nodes are compiled
# using the same toolchain for the same platform, you may use the top-level
# CMakeLists.txt file located in ../../../ instead to generate a single
# build system for all nodes.

cmake_minimum_required (VERSION 2.6)

set (NODE_NAME "demonstrator")
project (${NODE_NAME})

# Main project settings
include ("../../../../CMakeCommon.txt")

# Add project specific include directories
xme_include_directory ("../..")

# PROTECTED REGION ID(APPLICATION_DEMONSTRATOR_CMAKELISTS_TXT_INCLUDES) ENABLED START
add_definitions(-DXME_MULTITHREAD)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_BINARY_DIR}
)
# PROTECTED REGION END

# Needed to ensure PRIuXX macros are defined by <inttypes.h>. See Issue #3837.
add_definitions (-D__STDC_FORMAT_MACROS)

# Node settings
add_definitions (-DXME_EXECUTION_MODEL_BEST_EFFORT)

# Add component and application definitions
include ("CMakeApplication.txt")
