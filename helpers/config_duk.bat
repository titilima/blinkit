REM Please install yaml first:
REM     pip install pyyaml

cd %~dp0
python ..\third_party\duktape\tools\configure.py --output-directory ..\src\js\duktape --line-directives
