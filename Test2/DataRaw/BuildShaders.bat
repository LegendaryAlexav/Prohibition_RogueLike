@setlocal EnableDelayedExpansion

@echo off

set shadercompiler=..\Tools\shaderc

@mkdir ..\Data\Shaders
@mkdir ..\Data\Shaders\dx11
rem @mkdir ..\Data\Shaders\pssl
rem @mkdir ..\Data\Shaders\metal
rem @mkdir ..\Data\Shaders\nvn
@mkdir ..\Data\Shaders\glsl
rem @mkdir ..\Data\Shaders\essl
@mkdir ..\Data\Shaders\spirv

@call; rem clear the errorlevel

@FOR %%G in ("Shaders\*.vert") DO (
    %shadercompiler% -f %%G -o ../Data/Shaders/dx11/%%~nxG.bin  --type v --platform windows -i Shaders\bgfx -p s_5_0
    IF !ERRORLEVEL! NEQ 0 (echo Errors in %%G && goto error) ELSE (echo Built dx11 %%G)
    %shadercompiler% -f %%G -o ../Data/Shaders/glsl/%%~nxG.bin  --type v --platform windows -i Shaders\bgfx -p 440
    IF !ERRORLEVEL! NEQ 0 (echo Errors in %%G && goto error) ELSE (echo Built glsl %%G)
    %shadercompiler% -f %%G -o ../Data/Shaders/spirv/%%~nxG.bin --type v --platform windows -i Shaders\bgfx -p spirv
    IF !ERRORLEVEL! NEQ 0 (echo Errors in %%G && goto error) ELSE (echo Built spirv %%G)
)

IF !ERRORLEVEL! NEQ 0 (goto error)

@FOR %%G in ("Shaders\*.frag") DO (
    %shadercompiler% -f %%G -o ../Data/Shaders/dx11/%%~nxG.bin  --type f --platform windows -i Shaders\bgfx -p s_5_0
    IF !ERRORLEVEL! NEQ 0 (echo Errors in %%G && goto error) ELSE (echo Built dx11 %%G)
    %shadercompiler% -f %%G -o ../Data/Shaders/glsl/%%~nxG.bin  --type f --platform windows -i Shaders\bgfx -p 440
    IF !ERRORLEVEL! NEQ 0 (echo Errors in %%G && goto error) ELSE (echo Built glsl %%G)
    %shadercompiler% -f %%G -o ../Data/Shaders/spirv/%%~nxG.bin --type f --platform windows -i Shaders\bgfx -p spirv
    IF !ERRORLEVEL! NEQ 0 (echo Errors in %%G && goto error) ELSE (echo Built spirv %%G)
)

goto :end

:error
echo.
pause
exit /B 1

:end
