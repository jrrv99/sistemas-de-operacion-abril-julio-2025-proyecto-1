#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "graph.h"
#define DEFAULT_TICKS_PER_DAY 64
#define DEFAULT_TICK_DURATION 1.0
#define DEFAULT_MAX_ZONE_LEVEL 10

typedef struct {
  int ticks_per_day;
  float tick_duration;
  int max_zone_level;
  char input_file[256];
} SimulationConfig;

void print_usage(const char *progname) {
  fprintf(stderr,
          "Uso: %s [-d <ticks por día>] [-t <duración tick>] [-z <nivel máximo "
          "de zona>] [-f <archivo entrada>]\n",
          progname);
}

void parse_arguments(int argc, char *argv[], SimulationConfig *config) {
  int opt;
  config->ticks_per_day = DEFAULT_TICKS_PER_DAY;
  config->tick_duration = DEFAULT_TICK_DURATION;
  config->max_zone_level = DEFAULT_MAX_ZONE_LEVEL;
  config->input_file[0] = '\0';  // vacío por defecto

  while ((opt = getopt(argc, argv, "d:t:z:f:")) != -1) {
    switch (opt) {
      case 'd':
        config->ticks_per_day = atoi(optarg);
        break;
      case 't':
        config->tick_duration = atof(optarg);
        break;
      case 'z':
        config->max_zone_level = atoi(optarg);
        break;
      case 'f':
        strncpy(config->input_file, optarg, sizeof(config->input_file) - 1);
        config->input_file[sizeof(config->input_file) - 1] = '\0';
        break;
      default:
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }
}

void main_menu(Graph *graph) {
  int choice;

  while (true) {
    printf("\n===== Menú Principal =====\n");
    printf("1. Simular otro día\n");
    printf("2. Agregar zona\n");
    printf("3. Agregar arteria vial\n");
    printf("4. Ampliar arteria vial\n");
    printf("5. Guardar grafo actual\n");
    printf("6. Salir del programa\n");
    printf("==========================\n");
    printf("Ingrese su opción: ");

    if (scanf("%d", &choice) != 1) {
      // Limpiar entrada no válida
      while (getchar() != '\n');
      printf("Opción no válida. Intente nuevamente.\n");
      continue;
    }

    switch (choice) {
      case 1:
        // simulate_day(graph);
        break;
      case 2:
        // add_zone(graph);
        break;
      case 3:
        // add_road(graph);
        break;
      case 4:
        // expand_road(graph);
        break;
      case 5:
        // save_graph(graph);
        break;
      case 6:
        printf("Saliendo del programa...\n");
        // free_graph(graph);
        exit(EXIT_SUCCESS);
      default:
        printf("Opción no válida. Intente nuevamente.\n");
        break;
    }
  }
}

int main(int argc, char *argv[]) {
  SimulationConfig config;
  parse_arguments(argc, argv, &config);

  // Mostrar configuración inicial
  printf("Configuración del simulador:\n");
  printf("Ticks por día: %d\n", config.ticks_per_day);
  printf("Duración de un tick: %.2f segundos\n", config.tick_duration);
  printf("Nivel máximo de zona: %d\n", config.max_zone_level);
  Graph *graph;
  if (strlen(config.input_file) > 0) {
    printf("Archivo de entrada: %s\n", config.input_file);
    // TODO: cargar grafo desde archivo CSV
  } else {
    printf("No se especificó archivo de entrada, iniciando grafo vacío.\n");
    // TODO: inicializar grafo vacío
    graph = create_graph();
    if (!graph) {
      fprintf(stderr, "Error al crear el grafo.\n");
      return EXIT_FAILURE;
    }
  }

  // TODO: Inicializar procesos hijos (zonas, tráfico, reloj, pipes, impresión)
  // TODO: Ejecutar loop principal del simulador con menú interactivo
  main_menu(graph);

  return 0;
}
