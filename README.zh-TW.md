# MeloTTS.cpp
<p>
   <b>< <a href='./README.md'>English</a></b> | <a href='./README.zh-CN.md'>简体中文</a> | <b>繁體中文</b> >
</p>


**MeloTTS.cpp** 是[meloTTS](https://github.com/myshell-ai/MeloTTS) 的C++ 實現，**meloTTS**這是由MyShell.ai 發布的一個高品質、多語言的文字轉語音(TTS) 庫，支援英語、中文以及其他多種語言。本倉庫基於**OpenVINO**，支援在 CPU、GPU 和 NPU 邊緣設備上的部署。目前，本倉庫僅支援中文(夾雜英文)。對[MeloTTS 英文模型](https://huggingface.co/myshell-ai/MeloTTS-English)的支援仍處於開發階段。

## Pipeline Design




MeloTTS.cpp的設計與[原始PyTorch 版本](https://github.com/myshell-ai/MeloTTS) 基本一致，由三個模型組成（BERT、TTS 和DeepFilterNet），其中DeepFilterNet 是額外新增的模型。


<img src="images/melotts_design.png" alt="Pipeline Design" title="Pipeline Design" width="800" style="display: block">


#### 圖例
1. tokenizer and BERT: tokenizer和 BERT 模型為中文使用 `bert-base-multilingual-uncased`，英文使用 `bert-base-uncased` 
2. g2p: Grapheme-to-Phoneme 
3. phones and tones: 中文表示為拼音和四聲，英文表示為音標和重音
4. tone_sandi: 修正分詞和音素的類別（僅用於中文）
5. DeepFilterNet: 用於降噪（由 int8 量化引入的背景噪音）


### Model-Device Compatibility Table
下表概述了每個模型支援的XPU：
| Model Name       | CPU Support | GPU Support | NPU Support |
|------------------|-------------|-------------|-------------|
| BERT (Preprocessing) | ✅           | ✅           | ✅           |
| TTS (Inference)      | ✅           | ✅           | ❌           |
| DeepFilterNet (Post-processing) | ✅           | ✅           | ✅           |

## Setup and Execution Guide

### 1. 下載 OpenVINO C++ Package

若要下載適用於Windows 的OpenVINO C++ 套件，請參考以下連結：[Install OpenVINO for Windows](https://docs.openvino.ai/2024/get-started/install-openvino/install-openvino-archive-windows.html)。
對於 OpenVINO 2024.4 在 Windows 上的安裝，您可以在cmd中執行命令列。

```
curl -O https://storage.openvinotoolkit.org/repositories/openvino/packages/2024.4/windows/w_openvino_toolkit_windows_2024.4.0.16579.c3152d32c9c_x86_64.zip --ssl-no-revoke
tar -xvf w_openvino_toolkit_windows_2024.4.0.16579.c3152d32c9c_x86_64.zip
```
對於 Linux，您可以從以下連結下載 C++ 套件：[Install OpenVINO for Linux](https://docs.openvino.ai/2024/get-started/install-openvino/install-openvino-archive-linux.html)。對於 OpenVINO 2024.4 在 Linux 上的安裝，只需從 https://storage.openvinotoolkit.org/repositories/openvino/packages/2024.4/linux 下載並解壓縮該套件。

有關其他版本和更多OpenVINO 信息，請訪問OpenVINO 官方工具包頁面：[OpenVINO Toolkit Overview](https://www.intel.com/content/www/us/en/developer/tools/openvino-toolkit/overview.html)

### 2. Clone倉庫
```
git lfs install
git clone https://github.com/apinge/MeloTTS.cpp.git
```

### 3. 編譯與執行
#### 3.1 Windows 編譯與執行
```
<OpenVINO_DIR>\setupvars.bat
cd MeloTTS.cpp
cmake -S . -B build && cmake --build build --config Release
.\build\Release\meloTTS_ov.exe --model_dir ov_models --input_file inputs.txt  --output_file audio.wav
```
#### 3.2 Linux 編譯與執行
```
source <OpenVINO_DIR>/setupvars.sh
cd MeloTTS.cpp 
cmake -S . -B build && cmake --build build --config Release
./build/meloTTS_ov --model_dir ov_models --input_file inputs.txt --output_file audio.wav
```
#### 3.3 在cmake裡啟用和停用 DeepFilterNet
DeepFilterNet 功能目前僅在 Windows 上支持，用於過濾 int8 量化模型中的雜訊。預設情況下，該功能是啟用的，但您可以在 CMake 階段使用 `-DUSE_DEEPFILTERNET` 選項來啟用或停用它。

例如，若要停用此功能，您可以在 CMake 產生過程中使用下列命令：
```
cmake -S . -B build -DUSE_DEEPFILTERNET=OFF
```
有關更多信息，請參閱 [DeepFilterNet.cpp](https://github.com/apinge/MeloTTS.cpp/blob/develop/src/deepfilternet/README.md).

### 4. 參數說明

可以使用 `run_tts.bat` 或 `run_tts.sh` 作為範例腳本來執行模型。以下為參數說明：


- `--model_dir`: 指定包含模型檔案、字典檔案和第三方資源檔案的資料夾，該資料夾為倉庫中的 `ov_models` 資料夾。您可能需要根據目前工作目錄調整相對路徑。
- `--tts_device`: 指定用於 TTS 模型的 OpenVINO 裝置。支援的設備包括 CPU 和 GPU（預設：CPU）。
- `--bert_device`: 指定用於 BERT 模型的 OpenVINO 裝置。支援的設備包括 CPU、GPU 和 NPU（預設：CPU）。
- `--nf_device`: 指定用於 DeepfilterNet 模型的 OpenVINO 裝置。支援的設備包括 CPU、GPU 和 NPU（預設：CPU）。
- `--input_file`: 指定要處理的輸入文字檔。確保文字是 **UTF-8** 格式。
- `--output_file`: 指定要產生的輸出 *.wav 音訊檔案。
- `--speed`: 指定輸出音訊的速度。預設值為 1.0。
- `--quantize`: 指示是否使用 int8 量化模型。預設值為 `false`，表示預設使用 fp16 模型。
- `--disable_bert`: 指示是否停用 BERT 模型推理。預設值為 false。
- `--disable_nf`: 指示是否停用 DeepfilterNet 模型推理（預設：`false`）。
- `--language`: 指定 TTS 的語言。預設語言為中文（`ZH`）。

## NPU設備支援
BERT 和 DeepFilterNet 模型支援將 NPU 作為推理設備，利用 Meteor Lake 和 Lunar Lake 中整合的 NPU。


以下是啟用的方法:
<details>
  <summary>Click here to expand/collapse content</summary>
  <ul>
   <li><strong>How to Build</strong></li>
   To enable the BERT model on NPU, an additional CMake option <code>-DUSE_BERT_NPU=ON</code> is required during the CMake generation. For example:
    <pre><code>cmake -DUSE_BERT_NPU=ON -B build -S .</code></pre>
   To enable DeepFilterNet on NPU, no additional compilation steps are required.
   <li><strong>How to Set Arguments</strong></li>
        To set arguments for models on NPU, use <code>--bert_device</code> npu for the BERT model and  <code>--nf_device</code> npu for the DeepFilterNet model respectively. For example:
        <pre><code>build\Release\meloTTS_ov.exe --bert_device NPU --nf_device NPU --model_dir ov_models --input_file inputs.txt  --output_file audio.wav</code></pre>
    
</ul>
</details>

## 版本支援
- **Operating System**: Windows, Linux
- **CPU Architecture**: Metor Lake, Lunar Lake, 大多數Intel CPUs
- **GPU Architecture**: Intel® Arc™ Graphics (Intel Xe, 包括iGPU)
- **NPU  Architecture**: NPU 4, Meteor Lake, Lunar Lake中整合的NPU
- **OpenVINO Version**: >=2024.4
- **C++ Version**: >=C++20

如果您使用的是 Windows 的 AI PC 筆記本，GPU 和 NPU 驅動通常是預先安裝的。然而，Linux 使用者或希望更新到最新驅動的 Windows 使用者應按照以下指南操作:

- **安裝/更新GPU驅動**: 請參考 [Configurations for Intel® Processor Graphics (GPU) with OpenVINO™](https://docs.openvino.ai/2024/get-started/configurations/configurations-intel-gpu.html) 

- **安裝/更新GPU驅動**: 請參考 [NPU Device](https://docs.openvino.ai/2024/openvino-workflow/running-inference/inference-devices-and-modes/npu-device.html) 

请注意，Windows 和 Linux 的所有驱动程序有所不同，因此请确保按照您特定操作系统的说明进行操作。

## 開發計劃

1. **實現MeloTTS英文版本**: 
   
2. **提高量化品質**:
   - 目前的INT8量化模型表現出輕微的背景雜訊。我們整合了 DeepFilterNet 進行後處理。未來的目標是透過量化技術解決噪音問題。

## Python版本
Python 版本（整合 OpenVINO 的 MeloTTS）可在 [MeloTTS-OV](https://github.com/zhaohb/MeloTTS-OV/tree/speech-enhancement-and-npu) 中找到。 Python 版本包括將模型轉換為 OpenVINO IR 的方法。

## 第三方庫

參考了以下倉庫:

- [cppjieba](https://github.com/yanyiwu/cppjieba)
    - C++中文分詞庫
- [cppinyin](https://github.com/pkufool/cppinyin)
    - C++漢語拼音庫，我們剝離了其python部分整合在程式碼中
- [libtorch](https://github.com/pytorch/pytorch/blob/main/docs/libtorch.rst)
   - 用於DeepFilterNet

