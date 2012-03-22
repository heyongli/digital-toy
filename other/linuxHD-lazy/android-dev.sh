#!/bin/bash

work_dir=./

#ubuntu 10.04
basic=" eclipse eclipse-jdt sun-java6-bin sun-java6-jre  java-gcj-compat-headless openjdk-6-jdk "
tools=" git-core gnupg flex bison gperf build-essential zip curl zlib1g-dev "
devlibs=" gcc-multilib g++-multilib libc6-dev  x11proto-core-dev libx11-dev "
#end 10.04


#########################################################################################################
#��������ͷ�ļ��� source ����
. ../lazy.h.sh $1


##############install dev pkgs 
fapt-install-desc "install basic dev pkg..." $basic
fapt-install-desc "install basic dev pkg..." $tool
fapt-install-desc "install basic dev pkg..." $devlibs


echo "Please run in work direcory....."
is_yes "Is here your Work direcory for Android develop?"


###############3install sun java6 jdk
echo "install sun-java6-jdk"
srun sudo add-apt-repository ppa:ferramroberto/java  || _check
srun sudo add-apt-repository   ppa:yogarine/eclipse   || _check
srun sudo add-apt-repository  ppa:eclipse-team/debian-package || _check
srun sudo apt-get update  
srun sudo apt-get install sun-java6-jdk || _check
srun sudo apt-get install eclipse-adt  || _check

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
#sudo update-alternatives �Cconfig java
sudo update-java-alternatives -s java-6-sun



############## Eclipse config

#���� Eclipse����� Help -> Install New Software ... -> Add ...������ Name Ϊ��Android Developer Toolkit����Location Ϊ��https://dl-ssl.google.com/android/eclipse������ȡ�����֮����һ����һ���İ�װ����

pinfo "Eclipse Help -> Install New Software ... -> Add ...  Android Developer Toolkit ��Location Ϊ https://dl-ssl.google.com/android/eclipse"
pinfo  "Eclipse Window -> Preferences -> Android select Android SDK Location" 
pinfo "On the Install New Software windows, click "Available Software Sites" and add http://download.eclipse.org/releases/indigo as a site."
#eclipse  -application "Android Developer Toolkit"  -repository https://dl-ssl.google.com/android/eclipse


#��װ��ɺ����� Eclipse ����� Window -> Preferences -> Android��ѡ�� Android SDK Location
#���ˣ��Ѿ����Կ��� Android Ӧ�ó����ˡ�


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




