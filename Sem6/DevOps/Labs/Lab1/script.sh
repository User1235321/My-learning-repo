countdown()
{
    local seconds=$1
    for ((i = seconds; i > 0; --i));
    do
        echo -ne "\rОжидание загрузки: $i секунд...   "
        sleep 1
    done
}

echo "Starting virtual machines..."
virsh -c qemu:///system start devops_course_master_0
virsh -c qemu:///system start devops_course_worker_0
virsh -c qemu:///system start devops_course_worker_1

countdown 15

gnome-terminal -- bash -c "ssh -t debian@192.168.99.100 'ping -c 4 192.168.99.101; ping -c 4 192.168.99.102; exec bash'"
echo "master0 open"
sleep 2

gnome-terminal -- bash -c "ssh debian@192.168.99.101; exec bash"
echo "worker0 open"
sleep 2

gnome-terminal -- bash -c "ssh debian@192.168.99.102; exec bash"
echo "worker1 open"
sleep 2
