@REM This file SHOULD BE executed in x64 mode!
msbuild BlinKit.sln /t:Clean /p:Configuration=Release;Platform=x86

msbuild BkBase\BkBase.vcxproj /t:Build /p:Configuration=Release;Platform=x86
msbuild BkCrawler\BkCrawler.vcxproj /t:Build /p:Configuration=Release;Platform=x86
msbuild BkHelper\BkHelper.vcxproj /t:Build /p:Configuration=Release;Platform=x86

