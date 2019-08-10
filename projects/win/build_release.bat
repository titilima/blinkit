@REM This file SHOULD BE executed in x64 mode!
msbuild BlinKit.sln /t:Clean /p:Configuration=Release;Platform=x86

REM msbuild BlinKit\BlinKit.vcxproj /t:Build /p:Configuration=Release;Platform=x86
msbuild BkRequest\BkRequest.vcxproj /t:Build /p:Configuration=Release;Platform=x86
msbuild BkCrawler\BkCrawler.vcxproj /t:Build /p:Configuration=Release;Platform=x86
