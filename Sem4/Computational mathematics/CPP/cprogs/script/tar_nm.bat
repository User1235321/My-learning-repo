@echo off
rem tar_nm.bat : package the NM_LIB source files and documents...
rem set up for GNU tar and gzip on a Win32 system

rem It is assumed that we are starting in the nm_lin/script directory
if exist ..\cmathsrc\spline.c goto :do_it

:failed
echo Could not find the source code.
echo Start this script in the nm_lib\script directory.
echo ...the tar file has not been built.
goto :end

:do_it
rem Go up a couple of levels so that the tar file will
rem include the full nm_lib home directory.
cd ..\..
tar -cvf nm_lib.tar nm_lib/c_source 
tar -uvf nm_lib.tar nm_lib/pascal 
tar -uvf nm_lib.tar nm_lib/fortran 
tar -uvf nm_lib.tar nm_lib/dmathpak 
tar -uvf nm_lib.tar nm_lib/cmathsrc 
tar -uvf nm_lib.tar nm_lib/cmathtxt 
tar -uvf nm_lib.tar nm_lib/cmathchi 
tar -uvf nm_lib.tar nm_lib/script 
tar -uvf nm_lib.tar nm_lib/doc 
tar -uvf nm_lib.tar nm_lib/index.html
echo Now, compress the tar file.
gzip nm_lib.tar
echo The package should be in the file nm_lib.tar.gz

:end
echo Script finished.
