/*
    Trabajo Final Introducción a la Concurrencia, 2016
    UNPSJB

    Cátedra:
        Marcelo Eleazar Gomez
        Nahuel Defossé

    Luciano Serruya Aloisi

*/

Para correr las versiones paralelas (coordinator.c y no_coordinator.c)

    mpicc coordinator.c -o coordinator
    mpirun -np NUMERO_DE_PROCESOS ./coordinator CANTIDAD_DE_FILAS_POR_STRIP

    El tamaño de la matriz está definida por NUMERO_DE_PROCESOS * CANTIDAD_DE_FILAS_POR_STRIP

Para correr la versión secuencial
    
    gcc sequential.c -o sequential -lm
    ./sequential N

    N va a ser el tamaño de la matriz