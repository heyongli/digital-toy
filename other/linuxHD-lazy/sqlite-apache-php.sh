#!/bin/bash

www_home="/home/www"

work_dir=./


#ubuntu 10.04
sqlite=" apache2 sqlite php5 php5-sqlite "

apache_php_cfg="/etc/php5/apache2/php.ini"
apache_site_def="/etc/apache2/sites-enabled/000-default"
#########################################################################################################
#包含公用头文件， source 命令
. lazy.h.sh $1



##############install dev pkgs 
fapt-install-desc "setup sqlite C/S env" $sqlite


configed=`grep  "extension=php_pdo_sqlite.so" $apache_php_cfg`

if [ "$configed-NULL" == "-NULL" ] ; then
echo "Config the php extention for apache2"
sudo echo "extension=php_pdo_sqlite.so" >> $apache_php_cfg
sudo echo "extension=php_sqlite.so" >> $apache_php_cfg
fi 

######################### setup apache home 
pinfo "setup apache home"
sudo  mkdir -p $www_home
sudo chmod a+rx $www_home
cat $apache_site_def | eval "sed 's\DocumentRoot /var/www\DocumentRoot $www_home\'" > /var/apa_def_site_to_new
#head /var/apa_def_site_to_new
sudo cp /var/apa_def_site_to_new  $apache_site_def

##test php script ok??
sudo echo '<?php echo "I like php on ".PHP_OS;  ?>' > $www_home/test.php


#### setup sqlite manager 
pinfo "setup sqlite manager "
cd /usr/src

SQLiteMan_V="SQLiteManager-1.2.4"
SQLiteMan="SQliteManager-1.2.4.tar.gz"

if [ ! -f $SQLiteMan ]; then
wget http://cdnetworks-kr-2.dl.sourceforge.net/project/sqlitemanager/sqlitemanager/1.2.4/$SQLiteMan
fi

tar zxf $SQLiteMan
sudo cp -r $SQLiteMan_V  $www_home
##back to here
srun cd -
echo 
cd $www_home
sudo chown -R www-data $www_home/$SQLiteMan_V
chmod -R a+rx $SQLiteMan_V/
srun cd -
echo 

sqman_db="/usr/local/apache/sqliteDb/"
mkdir -p $sqman_db
sudo mv /$www_home/$SQLiteMan_V/include/config.db $sqman_db/
sudo chmod -R 700 $sqman_db/
sudo chown -R www-data $sqman_db/

#
pinfo "configure the:"
echo "sudo gedit $www_home/$SQLiteMan_V/include/user_defined.inc.php"
echo 'define("SQLiteDb", "/usr/local/apache/sqliteDb/config.db");'
echo 'define("DEFAULT_DB_PATH", "/usr/local/apache/sqliteDb/config.db");'


pinfo "done"
###clean up 

exit
####################################################################################################################
##### refrence http://www.ooommm.org/sudelwiki/How_to_install_Eclipse_on_Ubuntu_Lucid_Lynx_10.04
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




