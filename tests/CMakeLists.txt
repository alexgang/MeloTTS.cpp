add_executable(test_cmudict ${CMAKE_CURRENT_SOURCE_DIR}/test_cmudict.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/language_modules/cmudict.cpp)
target_include_directories(test_cmudict PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/language_modules/cmudict.h)


add_executable(test_openvoice_tts ${CMAKE_CURRENT_SOURCE_DIR}/test_openvoice_tts.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvino_model_base.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvoice_tts.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.cpp )
target_link_libraries(test_openvoice_tts PRIVATE openvino::runtime) 
target_include_directories(test_openvoice_tts PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvoice_tts.h 
                                                      ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvino_model_base.h
                                                      ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.h 
                                                      ${CMAKE_CURRENT_SOURCE_DIR}/../src/info_data.h )

add_executable(test_openvoice_tts_en ${CMAKE_CURRENT_SOURCE_DIR}/test_openvoice_tts_en.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvino_model_base.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvoice_tts.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp)
target_link_libraries(test_openvoice_tts_en PRIVATE openvino::runtime) 
target_include_directories(test_openvoice_tts PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvoice_tts.h 
                                                      ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvino_model_base.h
                                                      ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.h 
                                                      ${CMAKE_CURRENT_SOURCE_DIR}/../src/info_data.h )

add_executable(test_chinese_mix ${CMAKE_CURRENT_SOURCE_DIR}/test_chinese_mix.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/language_modules/chinese_mix.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp 
                ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/cppinyin.cc 
                ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/cppinyin_csrc_utils.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/language_modules/chinese_mix.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.h)
target_include_directories(test_chinese_mix PRIVATE 
                    ${CMAKE_SOURCE_DIR}/thirdParty/cppjieba 
                    ${CMAKE_SOURCE_DIR}/thirdParty/cppjieba/include
                    ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/)


add_executable(test_cppjieba ${CMAKE_CURRENT_SOURCE_DIR}/test_cppjieba.cpp)
target_include_directories(test_cppjieba PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/cppjieba 
                                                 ${CMAKE_SOURCE_DIR}/thirdParty/cppjieba/include)

add_executable(test_cppinyin ${CMAKE_CURRENT_SOURCE_DIR}/test_cppinyin.cpp  
                            ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/cppinyin.cc
                            ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/cppinyin_csrc_utils.cc)                                
target_include_directories(test_cppinyin PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/)

add_executable(test_darts ${CMAKE_CURRENT_SOURCE_DIR}/test_darts.cpp ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/darts.h)
target_include_directories(test_darts  PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/)

add_executable(test_split_sentence ${CMAKE_CURRENT_SOURCE_DIR}/test_split_sentence.cpp ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/darts.h)
target_include_directories(test_split_sentence  PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc/)

add_executable(test_wordbreak ${CMAKE_CURRENT_SOURCE_DIR}/test_wordbreak.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/language_modules/cmudict.cpp)
target_include_directories(test_wordbreak PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/language_modules ${CMAKE_SOURCE_DIR}/thirdParty/cppinyin/csrc)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_bert
               test_bert.cpp
               ../src/tokenizer.cpp 
               ../src/openvino_model_base.cpp 
               ../src/bert.cpp  
               ../src/utils.cpp)
target_link_libraries(test_bert PRIVATE gtest_main openvino::runtime)

add_executable(test_tokenizer ${CMAKE_CURRENT_SOURCE_DIR}/test_tokenizer.cpp ../src/tokenizer.cpp ../src/utils.cpp)
target_link_libraries(test_tokenizer  PRIVATE gtest_main openvino::runtime)

add_executable(test_openvino_tokenizer test_openvino_tokenizer.cpp ../src/utils.cpp ../experimental/openvino_tokenizer.cpp)
target_link_libraries(test_openvino_tokenizer PRIVATE gtest_main openvino::runtime)

include(GoogleTest)
gtest_discover_tests(test_bert)
gtest_discover_tests(test_tokenizer)
gtest_discover_tests(test_openvino_tokenizer)
