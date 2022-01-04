# Assegnamento Invernale
# _Homework Inverno 2021_
## Programmazione di sistemi embedded
## Anno 2021 - 2022

Il presente progetto prevede di programmare due linee di produzione industriale, seguite da due cobot e da due robot che portano le scatole piene in magazzino. Il tutto deve lavorare secondo le regole della concorrenza.


## PROGRAMMI
I programmi realizzati sono due: 
- il programma principale che controlla la linea e i robot. Tale programma è composto da più moduli corrispondenti a ogni punto della consegna con i relativi file ".h"
- il programma da usare sul client per collegarsi tramite socket stream


## ESECUZIONE DEI PROGRAMMI
Per eseguire il programma occorre spostare il file client.cpp in un altra cartella, dirigersi da terminale nella cartella Assegnamento_Invernale, scrivere g++ --std=c++2a *.cpp -o server e in seguito ./server 10 3 8 4 4040.

Questo esempio indica che per la prima linea c'è una distanza di 10 metri tra il sistema di visione e il punto in cui il cobot preleva il pezzo e che il pezzo viene trasportato sulla linea di trasporto a una velocità di 3 m/s, per la seconda linea invece la distanza è di 8 metri e la velocità è di 4 m/s.

Il numero 4040 è invece il numero della porta per la socket stream.

Per eseguire il client occorre invece aprire una nuova finestra del terminale, dirigersi nella cartella dove è salvsto il file, scrivere g++ --std=c++2a client.cpp -o client e in seguito ./client localhost 4040.

Il numero 4040 è ancora una volta il numero della porta per la socket stream.

La parte --std=c++2a è un flag da usare solo nel caso in cui si esegua il programma da terminale mac che non compila in automatico con l'ultimo standard.

Per uscire dal programma occorre lanciare il comando ctrl-c da tastiera.
