# Tarea4-SO

Suma de matrices cuadradas en paralelo con la librería MPI.h. Fue testeado en una y dos máquinas. Para compilar el programa, utilizar:
```
mpicc name.c –o name
```
Para ejecutar el programa, utilizar(se realiza una conexión ssh) donde user e ip corresponden al usuario e ip a la que se conectarán y NP corresponde al número de procesos que se ejecutarán:
```
mpiexec –host localhost,user@ip –n NP name
```
Para que funcione en ambas máquinas deben estar los archivos, tanto el archivo de las matrices como el programa name.
Se mostrará por consola el tiempo de procesado de la suma.


Se utilizó la siguiente referencia para construir el programa:

[An introduction to the Message Passing Interface (MPI) using C](http://condor.cc.ku.edu/~grobe/docs/intro-MPI-C.shtml)
