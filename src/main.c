#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "blahaj.h"

char blahaj_text[] = "blahaj!\n";

static int is_jpeg(const char* path) {
    size_t l = strlen(path);
    if (l > 4 && (strcmp(path+l-5, ".jpeg") == 0)) return 1;
    if (l > 3 && (strcmp(path+l-4, ".jpg") == 0)) return 1;
    return 0;
}

static int do_getattr(const char* path, struct stat* st) {
    printf("[getattr] called, path: %s\n", path);

    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);


    if (strcmp(path, "/") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    } else {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        if (is_jpeg(path)) {
            st->st_size = blahaj_jpeg_len;
        } else {
            st->st_size = strlen(blahaj_text);
        }
    }

    return 0;
}

static int do_readdir(const char* path, void* buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi) {
    printf("[readdir] called, path: %s\n", path);

    filler(buffer, ".", NULL, 0);
    filler(buffer, "..", NULL, 0);

    int i;
    for (i = 0; i < 10000; i++) {
        filler(buffer, "blahaj", NULL, 0);
    }

    return 0;
}

static int do_read(const char* path, char* buffer, size_t size, off_t offset, struct fuse_file_info* fi) {
    printf("[read] called, offset: %ld, path: %s\n", offset, path);

    if (is_jpeg(path)) {
        int sz = (size > (blahaj_jpeg_len - offset)) ? blahaj_jpeg_len - offset : size;

        //printf("want: %ld offset: %ld sz: %d\n", size, offset, sz);

        memcpy(buffer, blahaj_jpeg + offset, sz);
        return sz;
    } else {
        int sz = (size > (strlen(blahaj_text) - offset)) ? strlen(blahaj_text) - offset : size;
        memcpy(buffer, blahaj_text + offset, sz);
        return sz;
    }

    return -1;
}

static struct fuse_operations operations = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .read = do_read,
};

int main(int argc, char** argv) {
    printf("blahajfs - https://cynthia.re\n");
    return fuse_main(argc, argv, &operations, NULL);
}
