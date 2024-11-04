@echo off
setlocal

REM Change to the directory where the batch file is located
cd %~dp0
set "curPath=%CD%"

set "str_copy=copy"
if not "%1" == "%str_copy%" (
    if not exist "%curPath%\build\Release\meloTTS_ov.exe" (
        echo Error !!! meloTTS_ov.exe does not exist. Plase back to build stage and make sure build is completed.
        goto :SKIP_RUN
    ) else (
        goto :SKIP_COPY
    )
)

REM Copy all DLLs into the execution path
set "dll_search_folder=%curPath%\thirdParty\libtorch"
set "dll_extension=.dll"
set "destination_folder=%curPath%\build\Release"

for /r "%dll_search_folder%" %%a in (*%dll_extension%) do (
    xcopy "%%a" "%destination_folder%" /Y
)

:SKIP_COPY

echo ".\build\Release\meloTTS_ov.exe --model_dir ov_models --input_file inputs.txt  --output_file audio.wav --speed 0.95 --tts_device CPU --bert_device CPU --quantize false --disable_bert false --disable_nf false"
.\build\Release\meloTTS_ov.exe --model_dir ov_models --input_file inputs.txt  --output_file audio.wav --speed 0.95 --tts_device CPU --bert_device CPU --quantize false --disable_bert false --disable_nf false

:SKIP_RUN

endlocal