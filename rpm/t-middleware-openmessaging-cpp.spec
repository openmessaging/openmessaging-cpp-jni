##############################################################
# http://baike.corp.taobao.com/index.php/%E6%B7%98%E5%AE%9Drpm%E6%89%93%E5%8C%85%E8%A7%84%E8%8C%83 #
# http://www.rpm.org/max-rpm/ch-rpm-inside.html              #
##############################################################
Name: t-middleware-openmessaging-cpp
Version:1.0.4
Release: %(echo $RELEASE)%{?dist}
# if you want use the parameter of rpm_create on build time,
# uncomment below
Summary: C++ Client of Open Messaging Specification
Group: alibaba/application
License: Apache License Version 2.0
%define _prefix /usr/local
%define _ld_conf /etc/ld.so.conf.d



# uncomment below, if your building depend on other packages
#BuildRequires: package_name = 1.0.0

BuildRequires: cmake
BuildRequires: ali-jdk = 1.8.0_66

# uncomment below, if depend on other packages

Requires: ali-jdk


%description
# if you want publish current svn URL or Revision use these macros
C++ Client of Open Messaging Specification

%debug_package
# support debuginfo package, to reduce runtime package size

# prepare your files
%install
# OLDPWD is the dir of rpm_create running
# _prefix is an inner var of rpmbuild,
# can set by rpm_create, default is "/home/a"
# _lib is an inner var, maybe "lib" or "lib64" depend on OS

# create dirs
mkdir -p $RPM_BUILD_ROOT/%{_prefix}
SRC_DIR=$OLDPWD/..
cd $SRC_DIR/third_party
wget -O vendor.tar.gz http://shutian.oss-cn-hangzhou.aliyuncs.com/vendor.tar.gz
tar -xzvf vendor.tar.gz
mkdir -p $SRC_DIR/build
cd $SRC_DIR/build
export JAVA_HOME=/opt/taobao/java
cmake -DCMAKE_INSTALL_PREFIX=${RPM_BUILD_ROOT}/%{_prefix} -D TEST=OFF ..
make %{_smp_mflags}
make install
mkdir -p ${RPM_BUILD_ROOT}/%{_ld_conf}
cp $SRC_DIR/oms.conf ${RPM_BUILD_ROOT}/%{_ld_conf}

# create a crontab of the package
#echo "
#* * * * * root /home/a/bin/every_min
#3 * * * * ads /home/a/bin/every_hour
#" > %{_crontab}

# package information
%files
# set file attribute here
%defattr(-,root,root)
# need not list every file here, keep it as this
%{_prefix}
%{_ld_conf}
## create an empy dir

# %dir %{_prefix}/var/log

## need bakup old config file, so indicate here

# %config %{_prefix}/etc/sample.conf

## or need keep old config file, so indicate with "noreplace"

# %config(noreplace) %{_prefix}/etc/sample.conf

## indicate the dir for crontab

# %attr(644,root,root) %{_crondir}/*

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%changelog
* Tue Apr 24 2018 shutian.lzh
- add spec of t-middleware-openmessaging-cpp
