#!/bin/bash

#########################################################################################################
#define fav packages to install
tftp=" xinetd tftphpa "
tftp_config="xinetd.d/tftp"

nfs_kernel=" nfs-kernel-server "
nfs_kernel_config="exports"

dhcpd=" dhcp3-server "
dhcpd_config="dhcp3/dhcpd.conf"

privoxy="privoxy"
privoxy_config="privoxy/config"


#########################################################################################################
#包含公用头文件， source 命令
# pinfo perror, enter work directory, $silence
. ./lazy.h.sh $1


fapt-install-desc "tftp: tftphpa installing... " $tftphpa
pinfo "$tftp install and basic config"
cat etc/$tftp_config

fapt-install-desc "NFS server... " $nfs_kernel
pinfo " $nfs_kernel server config"
cat etc/$nfs_kernel_config


fapt-install-desc "DHCPD... " $dhcpd
pinfo " $dhcpd install and basic config"
cat etc/$dhcpd_config


fapt-install-desc "http proxy from sockets proxy $privoxy ... "  $privoxy
pinfo " $privoxy install and basic config"
tail etc/$privoxy_config


exit
################################################
# vsftp baiscs: ftp
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






