#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

void tail(const char *filename, int n) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    char buffer[BUFFER_SIZE];
    off_t file_pos = lseek(fd, 0, SEEK_END);
    int line_count = 0;

    while (file_pos > 0 && line_count <= n) {
        ssize_t read_bytes = pread(fd, buffer, BUFFER_SIZE, file_pos - BUFFER_SIZE);
        if (read_bytes == -1) {
            perror("pread");
            break;
        }

        for (ssize_t i = read_bytes - 1; i >= 0; --i) {
            if (buffer[i] == '\n') {
                line_count++;
                if (line_count == n) {
                    file_pos += i + 1 - read_bytes;
                    break;
                }
            }
        }

        file_pos -= read_bytes;
    }

    // Print the last n lines
    lseek(fd, file_pos, SEEK_SET);
    ssize_t read_bytes;
    while ((read_bytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, read_bytes);
    }

    close(fd);
}
