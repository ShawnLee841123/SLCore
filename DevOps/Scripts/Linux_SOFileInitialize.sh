
echo "123456"|sudo sh -c "echo 'pwd'../Modules >> /etc/ld.so.conf.d/SLCore.conf"

echo "123456"|sudo ldconfig

echo "123456"|sudo sh -c "echo /home/shawnlee/GitWork/SLCore/SLCore/DevOps/DumpPath/core.%e.%p.%t > /proc/sys/kernel/core_pattern"
