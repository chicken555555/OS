//ls command

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

int main() {
    DIR *d;
    struct dirent *de;
    struct stat buf;
    struct passwd *p;
    struct group *g;
    struct tm *t;
    char permissions[10] = "rwxrwxrwx";
    char actual_permissions[10];
    char time[26];

    d = opendir(".");
    if (d == NULL) {
        perror("Unable to open directory");
        return 1;
    }

    while ((de = readdir(d)) != NULL) {
        stat(de->d_name, &buf);

        // File Type
        if (S_ISDIR(buf.st_mode))
            printf("d");
        else if (S_ISREG(buf.st_mode))
            printf("-");
        else if (S_ISCHR(buf.st_mode))
            printf("c");
        else if (S_ISBLK(buf.st_mode))
            printf("b");
        else if (S_ISLNK(buf.st_mode))
            printf("l");
        else if (S_ISFIFO(buf.st_mode))
            printf("p");
        else if (S_ISSOCK(buf.st_mode))
            printf("s");

        // File Permissions
        for (int i = 0, j = (1 << 8); i < 9; i++, j >>= 1) {
            actual_permissions[i] = (buf.st_mode & j) ? permissions[i] : '-';
        }
        actual_permissions[9] = '\0';
        printf("%s", actual_permissions);

        // No. of Hard Links
        printf("%5ld", buf.st_nlink);

        // User Name
        p = getpwuid(buf.st_uid);
        if (p != NULL)
            printf(" %.8s", p->pw_name);
        else
            printf(" %-8d", buf.st_uid);

        // Group Name
        g = getgrgid(buf.st_gid);
        if (g != NULL)
            printf(" %-8.8s", g->gr_name);
        else
            printf(" %-8d", buf.st_gid);

        // File Size
        printf(" %8ld", buf.st_size);

        // Date and Time of modification
        t = localtime(&buf.st_mtime);
        strftime(time, sizeof(time), "%b %d %H:%M", t);
        printf(" %s", time);

        // File Name
        printf(" %s\n", de->d_name);
    }

    closedir(d);
    return 0;
}



// CP command
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUF_SIZE 8192

int main(int argc, char* argv[]) {
    int input_fd, output_fd;
    ssize_t ret_in, ret_out;
    char buffer[BUF_SIZE];

    if (argc != 3) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return 1;
    }

    input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
        perror("open");
        return 2;
    }

    output_fd = open(argv[2], O_WRONLY | O_CREAT , 0644);
    if (output_fd == -1) {
        perror("open");
        close(input_fd);
        return 3;
    }

    while ((ret_in = read(input_fd, buffer, BUF_SIZE)) > 0) {
        ret_out = write(output_fd, buffer, ret_in);
        if (ret_out != ret_in) {
            perror("write");
            close(input_fd);
            close(output_fd);
            return 4;
        }
    }

    if (ret_in == -1) {
        perror("read");
        close(input_fd);
        close(output_fd);
        return 5;
    }

    close(input_fd);
    close(output_fd);
    return 0;
}

//Move command

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    int input_fd, output_fd;

    /* Input and output file descriptors */
    /* Are src and dest file name arguments missing */
    if (argc != 3) {
        printf("Usage: mv file1 file2");
        return 1;
    }

    /* Create input file descriptor */
    input_fd = link(argv[1], argv[2]);
    if (input_fd == -1) {
        perror("link error");
        return 2;
    }

    /* Create output file descriptor */
    output_fd = unlink(argv[1]);
    if (output_fd == -1) {
        perror("unlink");
        return 3;
    }

    return 0;
}

// rm command
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    int input_fd, output_fd;

    /* Input and output file descriptors */
    /* Are src and dest file name arguments missing */
    if (argc != 2) {
        printf("Usage: mv file1 file2");
        return 1;
    }
    

    /* Create output file descriptor */
    output_fd = unlink(argv[1]);
    if (output_fd == -1) {
        perror("unlink");
        return 3;
    }

    return 0;
}
