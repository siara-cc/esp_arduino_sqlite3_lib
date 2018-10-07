#include "stdlib.h"
#include "stdio.h"
#include "vfs.h"
#include "Arduino.h"

#include <sdfat_fns.h>
#include <spiffs_fns.h>

#define LDRV_TRAVERSAL 0

static int dir_level = 1;

vfs_vol *vfs_mount( const char *name, int num ) {
  vfs_vol *ret = NULL;
  if (memcmp(name, "/SD", 3) == 0) {
    return sdfat_mount(name, num);
  }
  return ret;
}

vfs_file *vfs_open( const char *name, const char *mode ) {
  vfs_file *ret = NULL;
  if (memcmp(name, "/SD", 3) == 0) {
    ret = sdfat_open(name, mode);
  } else {
    ret = spiffs_open(name, mode);
  }
  return ret;
}

void vfs_set_spiffs_file_obj(void *in_filep) {
    set_spiffs_file_obj(in_filep);
}

sint32_t vfs_stat( const char *name, struct vfs_stat *buf ) {
  strcpy(buf->name, name);
  if (memcmp(name, "/SD", 3) == 0) {
      return sdfat_exists(name);
  } else {
      return spiffs_exists(name);
  }
  return VFS_RES_ERR;
}

sint32_t vfs_remove( const char *name ) {
  if (memcmp(name, "/SD", 3) == 0) {
    return sdfat_remove(name);
  } else {
    return spiffs_remove(name);
  }
  return VFS_RES_ERR;
}

// vfs_close - close file descriptor and free memory
//   fd: file descriptor
//   Returns: VFS_RES_OK or negative value in case of error
sint32_t vfs_close( vfs_file *fd ) {
  if (fd->fs_type == VFS_FS_FATFS) {
    return sdfat_close(fd);
  } else {
    return spiffs_close(fd);
  }
  return VFS_RES_OK;
}

// vfs_read - read data from file
//   fd: file descriptor
//   ptr: destination data buffer
//   len: requested length
//   Returns: Number of bytes read, or VFS_RES_ERR in case of error
sint32_t vfs_read( vfs_file *fd, void *ptr, size_t len ) {
  if (fd->fs_type == VFS_FS_FATFS) {
    return sdfat_read(fd, ptr, len);
  } else {
    return spiffs_read(fd, ptr, len);
  }
  return VFS_RES_ERR;
}

// vfs_write - write data to file
//   fd: file descriptor
//   ptr: source data buffer
//   len: requested length
//   Returns: Number of bytes written, or VFS_RES_ERR in case of error
sint32_t vfs_write( vfs_file *fd, const void *ptr, size_t len ) {
  if (fd->fs_type == VFS_FS_FATFS) {
    return sdfat_write(fd, ptr, len);
  } else {
    return spiffs_write(fd, ptr, len);
  }
  return VFS_RES_ERR;
}

// vfs_lseek - move read/write pointer
//   fd: file descriptor
//   off: offset
//   whence: VFS_SEEK_SET - set pointer to off
//           VFS_SEEK_CUR - set pointer to current position + off
//           VFS_SEEK_END - set pointer to end of file + off
//   Returns: New position, or VFS_RES_ERR in case of error
sint32_t vfs_lseek( vfs_file *fd, sint32_t off, int whence ) {
  if (fd->fs_type == VFS_FS_FATFS) {
    return sdfat_lseek(fd, off, whence);
  } else {
    return spiffs_lseek(fd, off, whence);
  }
  return VFS_RES_ERR;
}

// vfs_flush - flush write cache to file
//   fd: file descriptor
//   Returns: VFS_RES_OK, or VFS_RES_ERR in case of error
sint32_t vfs_flush( vfs_file *fd ) {
  if (fd->fs_type == VFS_FS_FATFS) {
    return sdfat_flush(fd);
  } else {
    return spiffs_flush(fd);
  }
  return VFS_RES_ERR;
}

// vfs_size - get current file size
//   fd: file descriptor
//   Returns: File size
uint32_t vfs_size( vfs_file *fd ) {
  if (fd->fs_type == VFS_FS_FATFS) {
    return sdfat_size(fd);
  } else {
    return spiffs_size(fd);
  }
  return 0;
}
