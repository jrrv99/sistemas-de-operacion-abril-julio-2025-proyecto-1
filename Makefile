# Nombre del ejecutable
TARGET = simcaracas

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -g

# Archivos fuente
SRCS = main.c graph.c # zone.c traffic.c print.c clock.c pipe.c

# Archivos objeto (los .o se generan a partir de los .c)
OBJS = $(SRCS:.c=.o)

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regla para compilar archivos .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f *.o $(TARGET)

# Regla para ejecutar el programa con valores por defecto
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
