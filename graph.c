#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Graph* create_graph() {
  Graph* graph = (Graph*)malloc(sizeof(Graph));
  if (!graph) return NULL;
  graph->count = 0;
  pthread_mutex_init(&graph->lock, NULL);
  memset(graph->zones, 0, sizeof(graph->zones));
  return graph;
}

Zone* create_zone(const char* code, ZoneType type, int level) {
  Zone* zone = (Zone*)malloc(sizeof(Zone));
  if (!zone) return NULL;
  strncpy(zone->code, code, MAX_ZONE_CODE);
  zone->type = type;
  zone->level = level;
  zone->points = 0;
  zone->unemployed = (type == SOURCE) ? (1 << level) : 0;
  zone->open_positions = (type == SINK) ? (1 << level) : 0;
  zone->active = 1;

  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    zone->connections[i] = NULL;
    zone->roads[i] = NULL;
  }

  return zone;
}

int add_zone(Graph* graph, Zone* zone) {
  pthread_mutex_lock(&graph->lock);
  if (graph->count >= MAX_ZONES) {
    pthread_mutex_unlock(&graph->lock);
    return -1;
  }
  graph->zones[graph->count++] = zone;
  pthread_mutex_unlock(&graph->lock);
  return 0;
}

Zone* get_zone_by_code(Graph* graph, const char* code) {
  for (int i = 0; i < graph->count; i++) {
    if (strncmp(graph->zones[i]->code, code, MAX_ZONE_CODE) == 0)
      return graph->zones[i];
  }
  return NULL;
}

int connect_zones(Graph* graph, const char* from_code, const char* to_code,
                  Direction dir, int capacity) {
  Zone* from = get_zone_by_code(graph, from_code);
  Zone* to = get_zone_by_code(graph, to_code);
  if (!from || !to || from->connections[dir] != NULL) return -1;

  Road* road = (Road*)malloc(sizeof(Road));
  if (!road) return -2;
  road->capacity = capacity;
  road->vehicles = 0;

  from->connections[dir] = to;
  from->roads[dir] = road;

  // Conexión inversa
  Direction inverse = (dir == NORTH)   ? SOUTH
                      : (dir == SOUTH) ? NORTH
                      : (dir == EAST)  ? WEST
                                       : EAST;

  to->connections[inverse] = from;
  to->roads[inverse] = road;

  return 0;
}

// Genera una representación string 4x8 del nodo dado
void render_zone_ascii(Zone* zone, char output[4][9]) {
  if (!zone) return;

  // 1. Nivel del nodo
  snprintf(output[0], 9, "NIV:%-5d", zone->level);  // eg. "NIV:3   "

  // 2. Código del nodo
  snprintf(output[1], 9, "COD:%-5s", zone->code);  // eg. "COD:PTE "

  // 3. Estado: puestos disponibles o desempleados
  if (zone->type == SOURCE) {
    snprintf(output[2], 9, "DES:%-5d", zone->unemployed);
  } else {
    snprintf(output[2], 9, "JOB:%-5d", zone->open_positions);
  }

  // 4. Puntos acumulados
  snprintf(output[3], 9, "PTS:%-5d", zone->points);
}