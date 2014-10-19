# Mesures de temps i bandwidth d’una escriptura a disc

- Fer un programa que escrigui al disc 200 MBytes, mesurant el temps que triga a fer-ho amb gettimeofday.
- I que imprimeixi els temps invertit i el bandwidth que ha aconseguit la transferència d'informació.
- Comparar el resultat amb el temps i bandwidth que obtenen les següents comandes:
```
$ dd if=/dev/zero of=file count=3276800 bs=64 $ dd if=/dev/zero of=file count=409600 bs=512 $ dd if=/dev/zero of=file count=25600 bs=8192
```
- Feu diverses mesures de cada experiment i calculeu les mitjanes per assegurar que els resultats són més representatius del que passa en realitat.

## Build
make clean && make

## Run
`./filewriter`

## Test all
`./test.sh`
