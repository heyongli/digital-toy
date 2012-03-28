#!/bin/bash

#########################################################################################################
#define fav packages to install
tftp=" xinetd tftphpa "
nfs_kernel=" nfs-kernel-server "
dhcpd=" dhcp3-server "
#########################################################################################################
#包含公用头文件， source 命令
# pinfo perror, enter work directory, $silence
. ./lazy.h.sh $1


fapt-install-desc "tftp: tftphpa installing... " $tftphpa
pinfo "$tftp install and basic config"
cat etc/tftp

fapt-install-desc "NFS server... " $nfs_kernel
pinfo " $nfs_kernel server config"
cat etc/exports


fapt-install-desc "DHCPD... " $dhcpd
pinfo " $dhcpd install and basic config"
cat etc/dhcpd.conf


exit
################################################
# vsftp baiscs
pstart "Installing vsftpd"
srun sudo apt-get install -y vsftpd  || _check
srun sudo  /etc/init.d/vsftpd  start  
sudo mkdir -p ~/ftp  || _check
sudo chmod a+x ~/ftp || _check 
sudo chmod a-w ~/ftp || _check
srun sudo addgroup ftp  
srun sudo adduser ftp ftp --home  ~/ftp    
sudo  /etc/init.d/vsftpd  restart || _check
pstatus






