# DISTRHO Plugin Framework (DPF)
# Copyright (C) 2021 Jean Pierre Cimalando <jp-dev@inbox.ru>
# Copyright (C) 2021-2022 Filipe Coelho <falktx@falktx.com>
# SPDX-License-Identifier: ISC

cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)

set(NAME ScaleSpace)
project(${NAME})

add_subdirectory(dpf)

dpf_add_plugin(${NAME}
  TARGETS clap lv2 vst2 vst3 jack
  FILES_DSP
      plugins/ScaleSpace/ScaleSpace.cpp
  FILES_UI
      plugins/ScaleSpace/ScaleSpaceUI.cpp
      plugins/ScaleSpace/lib/DPFDearImGuiWidgets/opengl/DearImGui.cpp)

target_include_directories(${NAME} PUBLIC plugins/ScaleSpace)
target_include_directories(${NAME} PUBLIC plugins/ScaleSpace/lib/DearWidgetsApi)
target_include_directories(${NAME} PUBLIC plugins/ScaleSpace/lib/DPFDearImGuiWidgets/generic)
target_include_directories(${NAME} PUBLIC plugins/ScaleSpace/lib/DPFDearImGuiWidgets/opengl)
target_include_directories(${NAME} PUBLIC MTS-ESP/Master)
target_include_directories(${NAME} PUBLIC tuning-library/include)
