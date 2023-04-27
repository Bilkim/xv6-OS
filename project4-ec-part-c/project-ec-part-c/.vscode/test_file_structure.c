#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_DEPTH 3
#define FILES_PER_DIR 5
#define FILE_SIZE 512

void create_dir_structure(const char *base_dir, int depth) {
    if (depth > MAX_DEPTH) {
        return;
    }

    char path[256];
    for (int i = 0; i < FILES_PER_DIR; i++) {
        snprintf(path, sizeof(path), "%s/subdir_%d", base_dir, i);
        if (mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
            perror("mkdir");
            return;
        }

        // Create small files in the subdirectory
        for (int j = 0; j < FILES_PER_DIR; j++) {
            char file_path[256];
            snprintf(file_path, sizeof(file_path), "%s/file_%d.txt", path, j);
            int fd = open(file_path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (fd < 0) {
                perror("open");
                return;
            }

            char data[FILE_SIZE];
            memset(data, 'A', sizeof(data));
            if (write(fd, data, sizeof(data)) != sizeof(data)) {
                perror("write");
                close(fd);
                return;
            }

            close(fd);
        }

        // Recursively create subdirectories
        create_dir_structure(path, depth + 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <base_directory>\n", argv[0]);
        return 1;
    }

    create_dir_structure(argv[1], 1);
    return 0;
}
