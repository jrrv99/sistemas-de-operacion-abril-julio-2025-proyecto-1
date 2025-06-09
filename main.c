#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>
#include "graph.h"  // Nuestro archivo de estructuras y funciones del grafo

// Funciones a implementar en los archivos .c
void* zone_thread_handler(void* arg);      // Cada zona (hilo)
void zone_process_handler(Graph* city);    // Proceso de zonas   YORGELIS
void traffic_process_handler(Graph* city); // Proceso de tráfico
void print_process_handler(Graph* city);   // Proceso de impresión
void clock_process_handler();              // Proceso de reloj
void pipe_process_handler();               // Proceso de comunicación por pipe

int main(int argc, char* argv[]) {
    // Crear estructura inicial del grafo
    Graph* city = create_graph(); // Carga de archivo opcional va aquí

    pid_t zone_pid = fork(); //
    if (zone_pid == 0) {
        // Proceso de zonas
        zone_process_handler(city);
        exit(0);
    }

    pid_t traffic_pid = fork();
    if (traffic_pid == 0) {
        // Proceso de tráfico
        traffic_process_handler(city);
        exit(0);
    }

    pid_t print_pid = fork();
    if (print_pid == 0) {
        // Proceso de impresión
        print_process_handler(city);
        exit(0);
    }

    pid_t clock_pid = fork();
    if (clock_pid == 0) {
        // Proceso de reloj
        clock_process_handler();
        exit(0);
    }

    pid_t pipe_pid = fork();
    if (pipe_pid == 0) {
        // Proceso de pipes
        pipe_process_handler();
        exit(0);
    }

    // Proceso principal: esperar a que terminen los procesos hijo
    int status;
    for (int i = 0; i < 5; i++) {
        wait(&status);
    }

    destroy_graph(city); // Liberar memoria
    printf("Simulación finalizada correctamente.\n");
    return 0;
}
