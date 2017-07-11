REM @ECHO ON

REM ===================================================================================
REM 变量定义

REM ****************
REM 编译器信息
REM ****************
SET CYGDiskNo=D:
SET GYGFullPath=%CYGDiskNo%\AndroidDevelop\soft\cygwin\bin

REM ****************
REM 目标项目信息
REM ****************
SET ProjectDiskNo=D:
SET ProjectDiskNo1=D
SET ProjectFullPath=%ProjectDiskNo%\ProbeFUN4AD
SET ProjectFullPath1=%ProjectDiskNo1%/ProbeFUN4AD
SET ProjectJNIPath=jni
SET JavahCommandHeader=javah -classpath bin\classes -d %ProjectJNIPath%\source\include 

REM ===================================================================================

REM ##################################
REM 切换到指定的工程目录下
REM #####################
%ProjectDiskNo%
CD %ProjectFullPath%

REM ##################################
REM 生成头文件
REM #####################
REM %JavahCommandHeader%glovast.Framework.GFramework

REM ##################################
REM 开始编译
REM #####################
%CYGDiskNo%
chdir %GYGFullPath%
REM bash --login -c "cd /cygdrive/%ProjectFullPath1%/%ProjectJNIPath% && $NDK/ndk-build clean && $NDK/ndk-build"
bash --login -c "cd /cygdrive/%ProjectFullPath1%/%ProjectJNIPath% && $NDK/ndk-build"

pause
@ECHO ON