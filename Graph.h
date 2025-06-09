#ifndef GRAPH_H
#define GRAPH_H

#include <pthread.h>

#define MAX_ZONE_CODE 4
#define MAX_CONNECTIONS 4 // Norte, Sur, Este, Oeste
#define MAX_ZONES 100

typedef enum {
    NORTH = 0,
    SOUTH = 1,
    EAST = 2,
    WEST = 3
} Direction;

typedef enum {
    SOURCE,
    SINK
} ZoneType;

typedef struct Road {
    int capacity;           // Capacidad por tick
    int vehicles;           // Vehículos actuales
} Road;

typedef struct Zone {
    char code[MAX_ZONE_CODE];   // Código de 3 letras
    ZoneType type;              // Fuente o sumidero
    int level;                  // Nivel actual (2^n personas o puestos)
    int points;                 // Puntos acumulados para subir de nivel

    int unemployed;             // Solo si es fuente
    int open_positions;         // Solo si es sumidero

    struct Zone* connections[MAX_CONNECTIONS]; // Norte, Sur, Este, Oeste
    Road* roads[MAX_CONNECTIONS];              // Vías conectadas

    pthread_t thread_id;       // Hilo asociado
    int active;                // Si está activa o no
} Zone;

typedef struct Graph {
    Zone* zones[MAX_ZONES];
    int count;
    pthread_mutex_t lock; // Para acceder zonas[] de forma segura
} Graph;

// Funciones públicas
Graph* create_graph();
Zone* create_zone(const char* code, ZoneType type, int level);
int add_zone(Graph* graph, Zone* zone);
int connect_zones(Graph* graph, const char* from_code, const char* to_code, Direction dir, int capacity);
Zone* get_zone_by_code(Graph* graph, const char* code);

#endif // GRAPH_H
