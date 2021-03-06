#!/bin/bash

work_dir=./

#ubuntu 10.04
eclipse=" sun-java6-jre sun-java6-plugin sun-java6-jdk sun-java6-source sun-java6-demo sun-java6-fonts  gcj-jre  "

basic=" sun-java6-bin sun-java6-jre  java-gcj-compat-headless openjdk-6-jdk "
tools=" git-core gnupg flex bison gperf build-essential zip curl zlib1g-dev "
devlibs=" gcc-multilib g++-multilib libc6-dev  x11proto-core-dev libx11-dev "
#end 10.04


#########################################################################################################
#包含公用头文件， source 命令
. ../lazy.h.sh $1


##############install dev pkgs 
fapt-install-desc "install eclipse 3.6.2 ..." $eclipse
fapt-install-desc "install basic dev pkg..." $basic
fapt-install-desc "install basic dev pkg..." $tool
fapt-install-desc "install basic dev pkg..." $devlibs


##### refrence http://www.ooommm.org/sudelwiki/How_to_install_Eclipse_on_Ubuntu_Lucid_Lynx_10.04
if [ ! -f eclipse-SDK-3.6.2-linux-gtk.tar.gz ]; then
	wget -c http://download.eclipse.org/eclipse/downloads/drops/R-3.6.2-201102101200/download.php?dropFile=eclipse-SDK-3.6.2-linux-gtk.tar.gz 

fi
sudo tar -xvzf eclipse-SDK-3.6.2-linux-gtk.tar.gz --directory=/opt
sudo chown -R root:root /opt/eclipse 
sudo cp ../bin/eclipse /usr/local/bin/eclipse


echo "Please run in work direcory....."
is_yes "Is here your Work direcory for Android develop?"


###############3install sun java6 jdk
echo "install sun-java6-jdk"
if [ "$1_NULL" == "_NULL" ] ; then
srun sudo add-apt-repository ppa:ferramroberto/java  || _check
srun sudo add-apt-repository   ppa:yogarine/eclipse   || _check
srun sudo add-apt-repository  ppa:eclipse-team/debian-package || _check
srun sudo apt-get update  
srun sudo apt-get install sun-java6-jdk || _check
fi 
#############
echo "download basic SDK "
#srun wget -c http://dl.google.com/android/android-sdk_r17-linux.tgz || _check
srun tar xvf _android-sdk_r17-linux.tgz -C $work_dir || _check

###################
is_yes "Are you going to setup android basic env?"
cd android-sdk-linux
./tools/android


##########################
echo "Please select /usr/lib/jvm/java-6-sun/jre/bin/java  "
#sudo update-alternatives –config java
sudo update-java-alternatives -s java-6-sun



############## Eclipse config

#启动 Eclipse，点击 Help -> Install New Software ... -> Add ...，输入 Name 为“Android Developer Toolkit”，Location 为“https://dl-ssl.google.com/android/eclipse”。获取到软件之后，下一步下一步的安装即可

pinfo "Eclipse Help -> Install New Software ... -> Add ...  Android Developer Toolkit ，Location 为 https://dl-ssl.google.com/android/eclipse"
pinfo  "Eclipse Window -> Preferences -> Android select Android SDK Location" 
#pinfo "On the Install New Software windows, click "Available Software Sites" and add http://download.eclipse.org/releases/indigo for eclipse 3.7."
pinfo " Install New Software windows, click Available Software Sites and add add heliose update for 3.6  http://download.jboss.org/jbosstools/updates/stable/helios/" 
#eclipse  -application "Android Developer Toolkit"  -repository https://dl-ssl.google.com/android/eclipse


#安装完成后，重启 Eclipse ，点击 Window -> Preferences -> Android，选择 Android SDK Location
#至此，已经可以开发 Android 应用程序了。


############android source code 
exit


#############download repo to ~/bin
curl http://android.git.kernel.org/repo > ~/bin/repo
chmod a+x ~/bin/repo

#setup bash rc for repo
checkpath=`echo $PATH | grep ~/bin`
if [ "$checkpath_NULL" =  "_NULL" ]
do 
	echo "export PATH=$PATH:~/bin" >~/.bashrc
	export PATH=$PATH:~/bin
done 




