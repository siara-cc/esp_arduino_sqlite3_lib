#include "vfs_int.h"
#include "sdfat_fns.h"
#include <SdFat.h>

static int ss_pins[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
static SdFat *sdFats[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

vfs_vol *sdfat_mount( const char *name, int num ) {
    int sd_idx = name[3] - '0';
    if (sd_idx < 0 || sd_idx > 9)
        return NULL;
    ss_pins[sd_idx] = num;
    vfs_vol *ret = (vfs_vol *) malloc(sizeof(vfs_vol));
    ret->fs_type = VFS_FS_FATFS;
    SdFat *sdFat = new SdFat();
    ret->vol_obj = sdFat;
    ret->num = num;
    sdFats[sd_idx] = sdFat;
    pinMode(num, OUTPUT);
    if (!sdFat->begin(num)) {
      return NULL;
    }
    return ret;
}

vfs_file *sdfat_open( const char *name, const char *mode ) {
    vfs_file *ret = NULL;
    int sd_idx = name[3] - '0';
    if (sd_idx < 0 || sd_idx > 9)
        return NULL;
    ret = (vfs_file *) malloc(sizeof(vfs_file));
    ret->fs_type = VFS_FS_FATFS;
    ret->file_obj = NULL;
    uint8_t imode = 0;
    if (strchr(mode, 'r')) {
        imode |= O_READ;
        if (strchr(mode, '+'))
           imode |= O_WRITE;
    }
    if (strchr(mode, 'w')) {
        imode |= O_WRITE;
        if (strchr(mode, '+'))
            imode |= O_CREAT;
        else
            imode |= O_TRUNC;
    }
    if (strchr(mode, 'a')) {
        imode |= O_AT_END;
        imode |= O_CREAT;
    }
    File file = sdFats[sd_idx]->open(name + 5, imode);
    if (!file)
        return NULL;
    ret->file_obj = malloc(sizeof(File));
    memcpy(ret->file_obj, &file, sizeof(File));
    return ret;
}

sint32_t sdfat_remove( const char *name ) {
    int sd_idx = name[3] - '0';
    if (sd_idx < 0 || sd_idx > 9)
        return VFS_RES_ERR;
    SdFat *sdFat = sdFats[sd_idx];
    if (sdFat->remove(name + 5))
        return VFS_RES_OK;
    return VFS_RES_ERR;
}

sint32_t sdfat_exists( const char *name ) {
    int sd_idx = name[3] - '0';
    if (sd_idx < 0 || sd_idx > 9)
        return VFS_RES_ERR;
    SdFat *sdFat = sdFats[sd_idx];
    if (sdFat->exists(name + 5))
        return VFS_RES_OK;
    return VFS_RES_ERR;
}

sint32_t sdfat_close( vfs_file *fd ) {
    if (fd->file_obj) {
        ((File *)fd->file_obj)->close();
        free(fd->file_obj);
    }
    free(fd);
    return VFS_RES_OK;
}

sint32_t sdfat_read( vfs_file *fd, void *ptr, size_t len ) {
    sint32_t ret = ((File *)fd->file_obj)->read(ptr, len);
    if (ret < 0)
       ret = VFS_RES_ERR;
    return ret;
}

sint32_t sdfat_write( vfs_file *fd, const void *ptr, size_t len ) {
    sint32_t ret = ((File *)fd->file_obj)->write(ptr, len);
    if (ret < 0)
       ret = VFS_RES_ERR;
    return ret;
}

sint32_t sdfat_lseek( vfs_file *fd, sint32_t off, int whence ) {
    switch (whence) {
      case VFS_SEEK_SET:
        if ( ((File *)fd->file_obj)->seekSet(off) )
            return off;
      case VFS_SEEK_CUR:
        if ( ((File *)fd->file_obj)->seekCur(off) )
            return off;
      case VFS_SEEK_END:
        if ( ((File *)fd->file_obj)->seekEnd(off) )
            return off;
    }
    return VFS_RES_ERR;
}

sint32_t sdfat_flush( vfs_file *fd ) {
  ((File *)fd->file_obj)->sync();
  return VFS_RES_OK;
}

uint32_t sdfat_size( vfs_file *fd ) {
  return ((File *)fd->file_obj)->fileSize();
}
