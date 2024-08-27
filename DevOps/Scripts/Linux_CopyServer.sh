echo "=================Begin Copy Server Holder================="

curpath=$(pwd)
echo "Current path: $curpath"
#chmod -R 775 ~/GitWork/SLCore/SLCore/Build/ServerHolder

cp -r $curpath/../../LinuxBuild/ServerHolder/ServerHolder ../ServerHolder

echo "=================Finish Copy Server Holder================="
