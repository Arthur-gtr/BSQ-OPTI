#include <unistd.h>

inline void bsq_from_file(const char* filepath) {
}

inline void bsq_from_generation(const char* size_str, const char* pattern) {
}

int main(int argc, char **argv) {
    if (argc == 2) [[likely]] {
        bsq_from_file(argv[1]);
        return 0;
    }
    else if (argc == 3) [[unlikely]] {
        bsq_from_generation(argv[1], argv[2]);
        return 0;
    }
    else [[unlikely]] {
        write(2, "Error: Invalid arguments\n", 25);
        return 84;
    }
}