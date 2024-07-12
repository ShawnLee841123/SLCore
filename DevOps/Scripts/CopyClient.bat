@set CoreRoot=%CD%

cd ../../

copy Build\ClientTest\Debug\*.exe DevOps\ClientTest /y
copy Build\ClientTest\Debug\*.pdb DevOps\ClientTest /y

pause
