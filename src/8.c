#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_PATH "input/8.txt"

#define MAP_WIDTH 50
#define MAP_HEIGHT 50

typedef struct antenna_t {
    char id;
    int y, x;
} antenna_t;

char** read_map(const char* path) {
    FILE* fp = fopen(path, "r");

    char** map = calloc(MAP_HEIGHT, sizeof(char*));
    for (int y = 0; y < MAP_HEIGHT; ++y)
        map[y] = calloc(MAP_WIDTH + 1, sizeof(char));

    char* line = NULL;
    size_t len = 0;

    int lines_read = 0;

    while (lines_read < MAP_HEIGHT && getline(&line, &len, fp) != -1) {
        strncpy(map[lines_read], line, MAP_WIDTH);
        map[lines_read++][MAP_WIDTH] = '\0';
    }

    free(line);
    fclose(fp);

    return map;
}

void free_map(char** map) {
    for (int y = 0; y < MAP_HEIGHT; ++y) free(map[y]);
    free(map);
}

antenna_t set_antenna(char id, int y, int x) {
    return (antenna_t){ .id = id, .y = y, .x = x };
}

antenna_t* parse_antennas(char** map, size_t *n_antennas) {
    antenna_t* antennas = NULL;
    *n_antennas = 0;

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (map[y][x] != '.') {
                antennas = realloc(antennas, ((*n_antennas) + 1) * sizeof(antenna_t));
                antennas[(*n_antennas)++] = set_antenna(map[y][x], y, x);
            }
        }
    }

    return antennas;
}

void insert_antinodes(char** map, antenna_t ant1, antenna_t ant2) {
    if (ant1.id != ant2.id) return;

    int dy = ant2.y - ant1.y;
    int dx = ant2.x - ant1.x;

    int an1_y = ant1.y - dy;
    int an1_x = ant1.x - dx;

    int an2_y = ant2.y + dy;
    int an2_x = ant2.x + dx;

    if (an1_y >= 0 && an1_y < MAP_HEIGHT && an1_x >= 0 && an1_x < MAP_WIDTH) {
        if (map[an1_y][an1_x] != '#') map[an1_y][an1_x] = '#';
    }

    if (an2_y >= 0 && an2_y < MAP_HEIGHT && an2_x >= 0 && an2_x < MAP_WIDTH) {
        if (map[an2_y][an2_x] != '#') map[an2_y][an2_x] = '#';
    }
}

int count_antinodes(char** map) {
    int num_antinodes = 0;
    for (int y = 0; y < MAP_HEIGHT; ++y) for (int x = 0; x < MAP_WIDTH; ++x)
        if (map[y][x] == '#') num_antinodes++;

    return num_antinodes;
}

int main(void) {
    char** map = read_map(MAP_PATH);

    size_t num_antennas = 0;
    antenna_t* antennas = parse_antennas(map, &num_antennas);

    for (size_t i = 0; i < num_antennas; ++i) for (size_t j = i + 1; j < num_antennas; ++j)
        insert_antinodes(map, antennas[i], antennas[j]);

    printf("num antinodes: %d\n", count_antinodes(map));

    free_map(map);
    free(antennas);

    return 0;
}
