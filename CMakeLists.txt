cmake_minimum_required(VERSION 3.15)
project(MeloTTS)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# for env witout setting utf-8 for Chinese 
if(WIN32)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")
endif()

if(WIN32) #default compiler on win is msvc. I've not verifed it with clang-cl yet.
    add_definitions(-DNOMINMAX) # Otherwise, std::max() and std::min() won't work
    add_compile_options(/Zc:__cplusplus) # Add /Zc:__cplusplus flag for Visual Studio to properly set the __cplusplus macro
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Choose the configuration types" FORCE)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")# refer to https://developercommunity.visualstudio.com/t/debug-build-works-but-release-build-failsas-well-a/428160
endif()

find_package(OpenVINO REQUIRED COMPONENTS Runtime)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(SOURCE_FILES
    melo.cpp
    src/openvino_model_base.cpp
    src/tokenizer.cpp
    src/utils.cpp
    src/bert.cpp
    src/openvoice_tts.cpp
    src/tts.cpp
    src/language_modules/cmudict.cpp
    src/language_modules/chinese_mix.cpp
    src/language_modules/tone_sandhi.cpp
    src/text_normalization/text_normalization.cpp   
    src/text_normalization/char_convert.cpp     
    src/text_normalization/chronology.cpp     
    src/text_normalization/constants.cpp     
    src/text_normalization/num.cpp     
    src/text_normalization/phonecode.cpp     
    src/text_normalization/quantifier.cpp     
    ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/cppinyin.cc
    ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/cppinyin_csrc_utils.cc
)

# Header files
set(HEADER_FILES
    src/parse_args.h
    src/openvino_model_base.h
    src/info_data.h
    src/tokenizer.h
    src/utils.h
    src/bert.h
    src/openvoice_tts.h
    src/tts.h
    src/language_modules/cmudict.h
    src/language_modules/chinese_mix.h
    src/language_modules/tone_sandhi.h
    src/text_normalization/text_normalization.h 
    src/text_normalization/char_convert.h 
    src/text_normalization/chronology.h 
    src/text_normalization/constant.h 
    src/text_normalization/number.h
    src/text_normalization/phonecode.h 
    src/text_normalization/quantifier.h
    ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/cppinyin.h
    ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/cppinyin_csrc_utils.h
)
include_directories(${CMAKE_SOURCE_DIR}/thirdParty/eigen)

# Define the executable
add_executable(meloTTS_ov ${SOURCE_FILES} ${HEADER_FILES})

# Define DEBUG macro for Debug configuration
target_compile_definitions(meloTTS_ov PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")
target_include_directories(meloTTS_ov PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/eigen)
target_include_directories(meloTTS_ov PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/cppjieba)
target_include_directories(meloTTS_ov PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/cppjieba/include)
target_include_directories(meloTTS_ov PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc)
target_link_libraries(meloTTS_ov
    PRIVATE openvino::runtime  # Link OpenVINO Runtime privately
)
if (UNIX)
    target_link_libraries(meloTTS_ov PRIVATE pthread)
endif()




option(ENABLE_TESTS "Enable tests" OFF) # Disable the tests by default

if (${ENABLE_TESTS})
    add_subdirectory(tests)
endif()

