#
# No any warrant for any correctness or safety. 
#
# hyl 2/04 2009
#

#---------------------------------------------------------------------------------#
#
# 必须设置这个选项, 否则脚本不会打开别名功能. 
#
shopt -s expand_aliases
#---------------------------------------------------------------------------------#





#---------------------------------------------------------------------------------#
#
# print the colorful messages
# info color, fail color, normal color 
#
#---------------------------------------------------------------------------------#

N_C="[0;39m"  #normal 

INFO_C="[1;42m"  #info
MINOR_C="[1;31m"

START_C="[0;47m"  #minor
FAIL_C="[1;41m"  #fail 
OK_C="[1;42m" #OK

pinfo()  { echo "$INFO_C$1 $N_C";}
pminor() { echo -n "$MINOR_C$1 $N_C";}

pstart() { echo -n "$START_C$1 $N_C" ;}
pok()  { echo "$OK_C$1 $N_C" ;}
perror() { echo "$FAIL_C$1 $N_C";} 

pstatus() {
    if [ $? = 0 ] ; then
	pok "OK"
    else
        perror "Fail"
        exit
    fi
}
_check(){
    if [ $? = 0 ] ; then
	pstart "."
    else
        perror "Fail"
        exit
    fi
}


#---------------------------------------------------------------------------------#
#
# auto silence run a cmd
#---------------------------------------------------------------------------------#
unset silence
silence="no"
if [ ! "X$1" = "X" ]; then
unset silence
silence='yes'
fi

# usage: srun cmd ...
srun() {
    if [ "$silence" = "yes" ]; then
	$@ > /dev/null 
	return
    fi
    $@
} 


# 强制silence 
fsrun() {
    $@ > /dev/null 
} 



is_yes() {
while true; do
    read -p "$INFO_C$1 $N_C" yn
    case $yn in
        [Yy]* ) break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done
}


fix_NO_PUBKEY() {
	gpg --keyserver subkeys.pgp.net --recv-keys $1
	gpg -a --export $1 | sudo apt-key add -
}

#---------------------------------------------------------------------------------#
#  backup backup_critical() backup_critical_dir() backup-config()
#
#
#
#---------------------------------------------------------------------------------#


# get code name :DISTRIB_CODENAME=hardy
code_name=$(cat /etc/lsb-release  | grep DISTRIB_CODENAME | awk -F'[=]' '{print $2}')

if [ "$code_name" = "" ]; then
    perror "不知道是什么发行版........"
    exit 1
fi


# backup (file)
# usage: 
#  backup file in the defualt download folder, put any file in the same directory
backup() {
    mkdir -p backup
    if [ ! -f backup/$(basename $1).origin ]; then
 	cp $1 backup/$(basename $1).origin
    fi
    cp $1  backup/$(basename $1).$code_name.bakup.$(date +%Y.%m.%d)
}


#/*with in one mintes get into same directory*/
backup_critical() {
    bak_dir=$(date +%Y.%m.%d-%H%M)
    mkdir -p backup/$bak_dir
    if [ ! -f backup/$bak_dir/$(basename $1).origin ]; then
 	cp $1 backup/$bak_dir/$(basename $1).origin
    fi
    cp $1  backup/$bak_dir/$(basename $1)
}
backup_critical_dir() {
  for f in $1/*
  do
     if [ -f $f ]; then
       echo backup $f
       backup_critical $f
     fi
  done
}

# give a configfile path(from / path)
# save in a config directory, this directory looklike a system root
backup-config() {
    if [ ! -e $1 ]; then
       pminor ". $1"
       return
    fi
     
    back_dir=backup/$(date +config%Y.%m.%d)
    mkdir -p $back_dir
    
    if [ -d $1 ]; then
        mkdir -p  $back_dir/$1
        cp -rf $1/  $back_dir/$(dirname $1)/       #directory 
    else
        #normal file
        mkdir -p  $back_dir/$(dirname $1)
        cp -f  $1  $back_dir/$1
    fi 
}





#---------------------------------------------------------------------------------#
#  wbuild-tarball()
#  wget-tarball(){
#   
#---------------------------------------------------------------------------------#

#我們可以用 ${ } 分別替換獲得不同的值：
#${file#*/}：拿掉第一條 / 及其左邊的字串：dir1/dir2/dir3/my.file.txt
#${file##*/}：拿掉最後一條 / 及其左邊的字串：my.file.txt
#${file#*.}：拿掉第一個 .  及其左邊的字串：file.txt
#${file##*.}：拿掉最後一個 .  及其左邊的字串：txt
#${file%/*}：拿掉最後條 / 及其右邊的字串：/dir1/dir2/dir3
#${file%%/*}：拿掉第一條 / 及其右邊的字串：(空值)
#${file%.*}：拿掉最後一個 .  及其右邊的字串：/dir1/dir2/dir3/my.file
#${file%%.*}：拿掉第一個 .  及其右邊的字串：/dir1/dir2/dir3/my



# download and build tarball :  x.tar.(gz/bz/bz2) x will be the dir
#if [ ! -f yasm-0.7.2.tar.gz ]; then  
#	wget http://www.tortall.net/projects/yasm/releases/yasm-0.7.2.tar.gz
#    fi
#    tar xzvf yasm-0.7.2.tar.gz
#    cd yasm-0.7.2
#    fsrun ./configure
#    make
#    fsrun sudo make install
#    cd ..
wbuild-tarball(){
    tarball=$(basename $1)
    
    iszip=$(echo $tarball | grep zip)

    if [ ! $iszip ]; then
     tardir=${tarball%.*}
     tardir=${tardir%.*}
     #perror "*****tar"
    else
     tardir=${tarball%.*}
     #perror "*****zip"      
    fi

    pstart "下载并编译 $tarball..................."
    
    #echo $tardir, $tarball

    if [ ! -f $tarball ]; then
	wget $1
    fi
   
    if [ ! $iszip  ]; then
      srun tar xvf $tarball
    else
      srun 7z e -y $tarball
    fi
   
    cd $tardir
    srun ./configure && srun make &&  srun sudo make install 
    cd ..

    pstatus 
}

wget-tarball(){
 

    tarball=$(basename $1)
    
    iszip=$(echo $tarball | grep zip)

    if [ ! $iszip ]; then
     tardir=${tarball%.*}
     tardir=${tardir%.*}
     #perror "*****tar"
    else
     tardir=${tarball%.*}
     #perror "*****zip"      
    fi

    pstart "下载 $tarball..................."
    #echo $tardir, $tarball

    if [ ! -f $tarball ]; then
	wget $1
    fi
  
    if [ ! $iszip  ]; then
      sudo rm -rf $tardir
      srun tar xvf $tarball
    else
      srun sudo unzip -o  $tarball 
    fi

    pstatus
}



#---------------------------------------------------------------------------------#
# apt get function
#   fapt-install()
#   fapt-install-desc()
#
#---------------------------------------------------------------------------------#

# $1 is pkg, but also description
fapt-install(){
   pstart "Installing $1."
   for pkg in $*
   do
      srun sudo apt-get install -y --force-yes -f -m $pkg
     _check
   done
   pstatus
}

# $1 is the description ... then the pks
fapt-install-desc(){
   pstart "Installing $1."
   shift  #drop $1
   for pkg in $*
   do
      srun sudo apt-get install -y --force-yes -f -m $pkg
     _check
   done
   pstatus
}


#---------------------------------------------------------------------------------#
# mutithred apt install function
#
# refer to: 
#   https://help.ubuntu.com/community/AptGet/Offline/PrintUris   #get uris from pkgs
#---------------------------------------------------------------------------------#
MAXJOBS=10


jobs_wait() 
{
   jobs=$(jobs -p)
   echo "downloading: $jobs"
   for pid in   $jobs
   do
     echo "wait $pid"
     wait $pid
   done

}

 
start_job() 
{
   $*  2&>1 > /dev/null &
}
 

para-download (){

   pstart "Para downloading $1." 
   echo  # \n

   shift  #drop $1

   pstart "Anylisis $*"
   echo 

   for pkg in $*
   do
      pinfo $pkg
      deb=$(apt-get --print-uris --yes install $pkg | grep ^\' | cut -d\' -f2)
      if [ ! "Xnull$deb" = "Xnull" ]; then
        echo -n "downloading $deb"
	start_job  wget -c $deb
      fi
#     _check
   done

   jobs_wait

   pstatus
}





