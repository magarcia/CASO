En l'exemple condvar-exercise.c que trobareu a punt per compilar amb un
Makefile al costat de les transparències de sistemes de fitxers
(http://docencia.ac.upc.edu/FIB/grau/CASO), s'ha omès l'ús de la variable
de condició:

workers_ready_condition

la qual cosa fa que el programa no funcioni correctament. Determineu com
s'han de fer les crides a:

- pthread_cond_wait
- pthread_cond_broadcast
- pthread_cond_signal

per a que el programa torni a funcionar correctament.

Proveu també que el programa va més ràpid en fer la feina si definiu:
NWORKERS    2   o   4 en el define que hi ha al començament.

Funcionament del programa:

Té dues parts, seqüencial i paral·lela. La part seqüencial funciona
correctament i serveix de comprovació. Els noms de fitxer indicats pel
fitxer 'index' són buscats a Internet idescarregats seqüencialment. Els
fitxers van a parar al directori serial-out.tmp/.

En la part paral·lela, el flux principal inicialitza les estructures i
crea els treballadors. Llavors, per cada nom que troba a dins del fitxer
'index', el posa en una taula circular i un dels treballadors el descarrega
d'Internet i el deixa en el directori parallel-out.tmp/

Quan el programa funcioni correctament, els dos directoris de sortida
(serial-out.tmp i parallel-out.tmp) hauran de tenir els mateixos fitxers.
