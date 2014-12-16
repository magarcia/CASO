# Install
```bash
wget http://docencia.ac.upc.edu/FIB/grau/CASO/lab2014/rt-tests.tar.xz
tar xf rt-tests.tar.xz
sudo apt-get build-dep linux-image-`uname -r`
sudo apt-get install libnuma-dev
cd rt-tests
make all
```
