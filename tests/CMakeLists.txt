add_executable(test_tokenizer ${CMAKE_CURRENT_SOURCE_DIR}/test_tokenizer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.cpp)
target_link_libraries(test_tokenizer PRIVATE openvino::runtime) 
target_include_directories(test_tokenizer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.h)
#set_target_properties(bert_tokenizer_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

add_executable(test_cmudict ${CMAKE_CURRENT_SOURCE_DIR}/test_cmudict.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/language_modules/cmudict.cpp)
target_include_directories(test_cmudict PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/language_modules/cmudict.h)

add_executable(test_bert ${CMAKE_CURRENT_SOURCE_DIR}/test_bert.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvino_model_base.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/bert.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp)
target_link_libraries(test_bert PRIVATE openvino::runtime) 
target_include_directories(test_bert PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvino_model_base.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/bert.h  ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.h)

add_executable(test_openvoice_tts ${CMAKE_CURRENT_SOURCE_DIR}/test_openvoice_tts.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvino_model_base.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvoice_tts.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/bert.cpp )
target_link_libraries(test_openvoice_tts PRIVATE openvino::runtime) 
target_include_directories(test_openvoice_tts PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvoice_tts.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/openvino_model_base.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/tokenizer.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/bert.h ${CMAKE_CURRENT_SOURCE_DIR}/../src/info_data.h )

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