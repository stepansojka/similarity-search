# similarity-search
Work in progress - at the moment this only contains levenshtein distance.

Dependencies
===============
This code requires fairly recent GCC. To install on RHEL 7, first enable software collections:
```
sudo yum-config-manager --enable rhel-server-rhscl-7-rpms

```
then the devtoolset-7 collection, which install GCC 7:
```
sudo yum install devtoolset-7
```
then start using the software collection using your favorite shell:
```
scl enable devtoolset-7 bash
```
To run the test, install gtest:
```
sudo yum install gtest gtest-devel
```
