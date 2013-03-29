%define name trident
%define version 0.4.2
%define release 0
%define repo /home/dcoss/projects/trident

Name: %{name}
Version: %{version}
Release:	%{release}
Summary: Trident

Group: Scientific
License: GPLv3
URL: http://trident.stjude.org		
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
Provides: trident
Requires: python >= 2.6.6

%description
Trident!

%prep
if [[ ! -d %{name}-%{version} ]];then
	git clone %{repo} %{name}-%{version}
else
	cd %{name}-%{version}
	git pull origin master
fi

%build
cd %{name}-%{version}
sh setup.sh
%configure
make %{?_smp_mflags}
cd python
python2.6 setup.py install --prefix %{buildroot}/usr

%install
cd %{name}-%{version}
make install DESTDIR=%{buildroot}


%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
/usr


%changelog

