# CASO - Lab 1

sudo apt-get update

Poder compilar:
sudo apt-get install build-essential automake checkinstall

Poder compilar 32:
sudo apt-get install lib32stdc++6 libx32gcc-4.8-dev libc6-dev-i386

Compilar 64:
gcc hello.c -o hello64

Compilar 32:
gcc -m32 hello.c -o hello32