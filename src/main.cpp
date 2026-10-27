#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>

#pragma GCC optimize("O3,unroll-loops,tree-vectorize")
#ifndef NO_AVX
#if defined(__x86_64__) || defined(_M_X64)
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#endif
#endif

constexpr size_t MAX_WIDTH = 32768;

static short s_hist[MAX_WIDTH];

inline void resolve_bsq(char *grid, int width, int stride, int height) {
    int max_sq = 0;
    int best_x = 0;
    int best_y = 0;

    for (int y = 0; y < height; ++y) {
        char *row = grid + y * stride;
        for (int x = 0; x < width; ++x)
            s_hist[x] = (row[x] == '.') ? s_hist[x] + 1 : 0;

        int x = 0;

        while (x <= width - (max_sq + 1)) {
            int target_sq = max_sq + 1;
            int k = x + target_sq - 1;
            bool can_place = true;

            while (k >= x) {
                if (s_hist[k] < target_sq) {
                    x = k + 1;
                    can_place = false;
                    break;
                }
                k--;
            }

            if (can_place) {
                max_sq = target_sq;
                best_x = x;
                best_y = y;
            }
        }
    }

    if (max_sq > 0) {
        int start_y = best_y - max_sq + 1;

        for (int y = start_y; y <= best_y; ++y) {
            char *row_ptr = grid + y * stride + best_x;

            for (int x = 0; x < max_sq; ++x)
                row_ptr[x] = 'x';
        }
    }
}

inline void bsq_from_file(const char* filepath) {
    int fd = open(filepath, O_RDONLY);

    if (fd == -1) [[unlikely]]
        exit(84);

    struct stat sb;

    if (fstat(fd, &sb) == -1) [[unlikely]]
        exit(84);

    char *buffer = (char *)mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (buffer == MAP_FAILED) [[unlikely]]
        exit(84);

    close(fd);

    char *grid = buffer;
    while (*grid != '\n') [[likely]] {
        ++grid;
    }
    ++grid;

    char *line_end = grid;
    while (*line_end != '\n') [[likely]] {
        ++line_end;
    }

    int width = line_end - grid;
    int stride = width + 1;
    int height = (sb.st_size - (grid - buffer)) / stride;

    resolve_bsq(grid, width, stride, height);

    if (write(1, grid, sb.st_size - (grid - buffer)) < 0) [[unlikely]]
        exit(84);
}

inline void bsq_from_generation(const char *size_str, const char *pattern) {
    (void)size_str;
    (void)pattern;
    if (write(1, "oooohhh noooo\n", 14) < 0) [[unlikely]]
        exit(84);
}

int main(int argc, char **argv) {
    if (argc == 2) [[likely]] {
        bsq_from_file(argv[1]);
        return 0;
    }
    if (argc == 3) {
        bsq_from_generation(argv[1], argv[2]);
        return 0;
    }
    if (write(2, "Error: Invalid arguments\n", 25) < 0) [[unlikely]]
        return 84;
    return 84;
}
