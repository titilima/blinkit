@REM This file SHOULD BE executed in x64 mode!
msbuild BlinKit.sln /t:Clean /p:Configuration=Release;Platform=x86
msbuild BkRequest\BkRequest.vcxproj /t:Clean /p:Configuration=Release;Platform=x86
msbuild blink_crawler\blink_crawler.vcxproj /t:Clean /p:Configuration=Release;Platform=x86
msbuild js_crawler\js_crawler.vcxproj /t:Clean /p:Configuration=Release;Platform=x86
msbuild BkCrawler\BkCrawler.vcxproj /t:Clean /p:Configuration=Release;Platform=x86

msbuild BlinKit\BlinKit.vcxproj /t:Build /p:Configuration=Release;Platform=x86
msbuild BkRequest\BkRequest.vcxproj /t:Build /p:Configuration=Release;Platform=x86
msbuild BkCrawler\BkCrawler.vcxproj /t:Build /p:Configuration=Release;Platform=x86
