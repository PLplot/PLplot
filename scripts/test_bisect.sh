#!/bin/sh
# test script to generate an error for bisection purposes.
# Expect substantial updates to this script for each new error to be
# bisected.
#
# This particular case is used to generate the test_c_tk run-time
# error in the install-tree.

rm -rf bisect_test_build_dir bisect_test_install bisect_test_install_build_dir
mkdir bisect_test_build_dir bisect_test_install_build_dir
cd bisect_test_build_dir
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../bisect_test_install \
-DDEFAULT_NO_BINDINGS=ON -DENABLE_tcl=ON -DENABLE_tk=ON \
-DDEFAULT_NO_DEVICES=ON -DPLD_tk=ON -DPLD_xwin=ON \
.. >& cmake.out
make -j4 install >& make_install.out
cd ../bisect_test_install_build_dir
cmake ../bisect_test_install/share/plplot5.9.5/examples >& cmake.out
make x01c
c/x01c -dev tk 

