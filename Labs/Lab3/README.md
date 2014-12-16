# Avaluació del rendiment

[Report final](https://github.com/magarcia/CASO/blob/master/Labs/Lab3/exemple-rendiment/Report.pdf?raw=true)

## Info

- [write-to-disk.c](https://github.com/magarcia/CASO/blob/master/Labs/Lab3/exemple-rendiment/write-to-disk.c)
Pruebas de escritura a disco version "final"

- [write-to-disk1.c](https://github.com/magarcia/CASO/blob/master/Labs/Lab3/exemple-rendiment/write-to-disk1.c)
Pruebas de escritura a disco version 1

- [write-to-disk2.c](https://github.com/magarcia/CASO/blob/master/Labs/Lab3/exemple-rendiment/write-to-disk2.c)
Pruebas de escritura a disco version 2

- [read-from-disk.c](https://github.com/magarcia/CASO/blob/master/Labs/Lab3/exemple-rendiment/read-from-disk.c)
Realiza pruevas de lectura en ficheros en disco.

- [process_data.sh](https://github.com/magarcia/CASO/blob/master/Labs/Lab3/exemple-rendiment/process_data.sh) Realiza
diferentes pruebas y genera dos ficheros de salida usb.dat y hdd.dat. Necesita
los ficheros `usb-test-write.dat` y `hdd-test-write.dat`. Para generarlos:

```bash
#!/usr/bin/env

# Compilamos todo
make all

HDD_FILE_PATH=/path/to/hdd/file
USB_FILE_PATH=/path/to/usb/file

# Bucle, de 100MB a 500MB de 5 en 5
for i in `seq 100 +5 500`
do
    # 10 iteraciones por disco y tamaño para hacer la media
    for j in `seq 10`
    do
        ./write-to-disk2-debug $HDD_FILE_PATH >> hdd-test-write.dat
        ./write-to-disk2-debug $USB_FILE_PATH >> usb-test-write.dat
    done
done
```

- [plotgenerator.py](https://github.com/magarcia/CASO/blob/master/Labs/Lab3/exemple-rendiment/plotgenerator.py) Genera
los graficos a partir de los datos generados por `process_data.sh`. Librerias
necesarias para instalar:
`easy_install plotly`