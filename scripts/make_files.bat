@REM Please install PHP and add it to PATH.

cd %~dp0
php html_names.h.php > ..\src\chromium\third_party\blink\renderer\core\html_names.h
php html_names.cpp.php > ..\src\chromium\third_party\blink\renderer\core\html_names.cpp
