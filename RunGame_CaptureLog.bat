@echo off
REM CMD 로그창에 뜨는 빨간 에러를 파일로 남기려면 이 배치로 실행하세요.
REM -stdout -FullStdOutLogOutput 로 콘솔 출력을 뿌리고, 리다이렉트로 파일에 저장합니다.

set PROJECT_DIR=%~dp0
set LOGDIR=%PROJECT_DIR%Saved\Logs
if not exist "%LOGDIR%" mkdir "%LOGDIR%"

REM 타임스탬프로 파일명 (빨간줄은 보통 크래시 직전에 찍히므로, 크래시 후 이 파일 끝부분을 보면 됨)
set LOGFILE=%LOGDIR%\CmdCapture.log

echo Log file: %LOGFILE%
echo Running with -stdout -FullStdOutLogOutput ...
echo.

REM UE 설치 경로 (필요하면 수정)
set UE_EDITOR=C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe
if not exist "%UE_EDITOR%" (
    echo ERROR: UnrealEditor not found at %UE_EDITOR%
    echo Edit RunGame_CaptureLog.bat and set UE_EDITOR to your install path.
    pause
    exit /b 1
)

REM 리다이렉트: CMD에 나오는 출력이 CmdCapture.log 에 기록됨.
REM (크래시 시 버퍼가 안 flush 되면 마지막 몇 줄은 빠질 수 있음. 그때는 Saved\Crashes\... 의 로그/ CrashContext 참고)
"%UE_EDITOR%" "%PROJECT_DIR%Jujutsu.uproject" -game -log -windowed -ResX=800 -ResY=450 -stdout -FullStdOutLogOutput > "%LOGFILE%" 2>&1

echo.
echo Process ended. Check end of: %LOGFILE%
pause
