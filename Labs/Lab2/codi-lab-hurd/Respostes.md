# Lab Hurd

## 2 Funciona correctament o us dóna un “bus error”? Podeu esbrinar què passa?

No funciona correctament. Segurament, el thread fill no tingui permisos per
fer un printf.


## 7 Creieu que això mateix es pot fer en UNIX/Linux? I en Windows?

En el cas de Linux sí d’una manera similar a Hurd.
