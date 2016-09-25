del "NumberFactory.sdf"
del "NumberFactory.VC.db"

::rd /s /q "libs\"
rd /s /q "Source Files\"

rd /s /q ".vs\"
rd /s /q "ipch\"
rd /s /q "x64\"
rd /s /q "NumberFactory\x64\"

rd /s /q "ipch\"
rd /s /q "Debug\"
rd /s /q "NumberFactory\Debug\"
rd /s /q "Release\"
rd /s /q "NumberFactory\Release\"

del NumberFactory\*.txt
rd /s /q "NumberFactory\My Amplifier XE Results - NumberFactory\"
