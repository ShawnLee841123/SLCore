@set CoreRoot=%CD%

cd ../../

::copy WinBuild\ClientTest\Debug\*.exe DevOps\ClientTest /y
::copy WinBuild\ClientTest\Debug\*.pdb DevOps\ClientTest /y
copy WinBuild\ServerHolder\Debug\*.exe DevOps\ClientTest /y
copy WinBuild\ServerHolder\Debug\*.pdb DevOps\ClientTest /y

pause
