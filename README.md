# MeloTTS.cpp

This repository offers a C++ implementation of [meloTTS](https://github.com/myshell-ai/MeloTTS), which is a high-quality, multilingual Text-to-Speech (TTS) library released by MyShell.ai that supports English, Chinese (mixed with English), and various other languages. This implementation is fully integrated with OpenVINO. Currently, this repository only supports Chinese mixed with English. Support for [English model](https://huggingface.co/myshell-ai/MeloTTS-English) is coming next.


## Setup and Execution Guide

### 1. Download OpenVINO C++ Package

To download the OpenVINO C++ package for Windows, please refer to the following link: [Install OpenVINO for Windows]( https://docs.openvino.ai/2024/get-started/install-openvino/install-openvino-archive-windows.html).
For **OpenVINO 2024.4** on Windows, you can run the command line in the command prompt (cmd).
```
curl -O https://storage.openvinotoolkit.org/repositories/openvino/packages/2024.4/windows/w_openvino_toolkit_windows_2024.4.0.16579.c3152d32c9c_x86_64.zip --ssl-no-revoke
tar -xvf w_openvino_toolkit_windows_2024.4.0.16579.c3152d32c9c_x86_64.zip
```
For Linux, you can download the C++ package from this link: [Install OpenVINO for Linux](https://docs.openvino.ai/2024/get-started/install-openvino/install-openvino-archive-linux.html). For **OpenVINO 2024.4** on Linux, simply download it from https://storage.openvinotoolkit.org/repositories/openvino/packages/2024.4/linux and unzip the package.

For additional versions and more information about OpenVINO, visit the official OpenVINO Toolkit page: [OpenVINO Toolkit Overview](https://www.intel.com/content/www/us/en/developer/tools/openvino-toolkit/overview.html).

### 2. Clone the Repository
```
git lfs install
git clone https://github.com/apinge/MeloTTS.cpp.git
```

### 3. Build and Run
#### 3.1 Windows Build and Run
```
<OpenVINO_DIR>\setupvars.bat
cd MeloTTS.cpp 
cmake -S . -B build && cmake --build build --config Release
.\build\Release\meloTTS_ov.exe --model_dir ov_models --input_file inputs.txt  --output_file audio.wav
```
#### 3.2 Linux Build and Run
```
source <OpenVINO_DIR>/setupvars.sh
cd MeloTTS.cpp 
cmake -S . -B build && cmake --build build --config Release
./build/meloTTS_ov --model_dir ov_models --input_file inputs.txt --output_file audio.wav
```
### 4. Arguments Description
You can use `run_tts.bat` or `run_tts.sh` as sample scripts to run the models. Below are the meanings of all the arguments you can use with these scripts:

- `--model_dir`: Specifies the folder containing the model files, dictionary files, and third-party resource files, which is `ov_models` folder within the repo. You may need to adjust the relative path based on your current working directory.
- `--tts_device`: Specifies the OpenVINO device to be used for the TTS model. The default device is CPU.
- `--bert_device`: Specifies the OpenVINO device to be used for the BERT model. The default device is CPU.
- `--input_file`: Specifies the input text file to be processed. Make sure that the text is in **UTF-8** format.
- `--output_file`: Specifies the output *.wav audio file to be generated.
- `--speed`: Specifies the speed of output audio. The default is 1.0.
- `--quantize`: Indicates whether to use an int8 quantized model. The default is false, meaning an fp16 model is used by default.
- `--disable_bert`: Indicates whether to disable the BERT model inference. The default is false.
- `--language`: Specifies the language for TTS. The default language is Chinese (`ZH`).

## Supported Versions
- **Operating System**: Windows, Linux
- **CPU Architecture**: Metor Lake, Lunar Lake, and most Intel CPUs
- **GPU Architecture**: Intel® Arc™ Graphics (Intel Xe, including iGPU)
- **C++ Version**: >=C++20

If you specify GPU as the device, please refer to [Configurations for Intel® Processor Graphics (GPU) with OpenVINO™](https://docs.openvino.ai/2024/get-started/configurations/configurations-intel-gpu.html) to install the GPU driver.


## Future Development Plan
Here are some features and improvements planned for future releases:

1. **Add English language TTS support**: 
   - Enable English text-to-speech (TTS) functionality, but tokenization for English language input is not yet implemented.
   
2. **Support for NPU device**:
   - Implement NPU device compatibility for the BERT model within the pipeline.

## Python Version
The Python version of this repository (MeloTTS integrated with OpenVINO) is provided in [MeloTTS-OV](https://github.com/zhaohb/MeloTTS-OV/tree/speech-enhancement-and-npu). The Python version includes methods to convert the model into OpenVINO IR.

## Third-Party Code
This repository includes third-party code and libraries for Chinese word segmentation and pinyin processing.

- [cppjieba](https://github.com/yanyiwu/cppjieba)
    - A Chinese text segmentation library.
- [cppinyin](https://github.com/pkufool/cppinyin)
    - A C++ library supporting conversion between Chinese characters and pinyin


