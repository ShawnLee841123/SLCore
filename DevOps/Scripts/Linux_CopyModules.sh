echo "====================Begin Copy .SO Files================="

#chmod -R 775 ~/GitWork/SLCore/SLCore/Build
#chmod -R 775 ~/GitWork/SLCore/SLCore/DevOps/Modules
curpath=$(pwd)
echo "Current path: $curpath"
cp -r $curpath/../../LinuxBuild/*.so ~/GitWork/SLCore/SLCore/DevOps/Modules


echo "====================Finish Copy .SO Files================="

