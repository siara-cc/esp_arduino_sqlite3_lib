#include "vfs_int.h"
#include "spiffs_fns.h"
#include <FS.h>

File *filep;
void set_spiffs_file_obj(void *in_filep) {
    filep = (File *) in_filep;
}

vfs_file *spiffs_open( const char *name, const char *mode ) {
  vfs_file *ret = (vfs_file *) malloc(sizeof(vfs_file));
  ret->fs_type = VFS_FS_SPIFFS;
  ret->file_obj = NULL;
  *filep = SPIFFS.open(name + 6, mode);
  if (!*filep)
      return NULL;
  ret->file_obj = filep;
  return ret;
}

sint32_t spiffs_remove( const char *name ) {
  if (SPIFFS.remove(name + 6))
      return VFS_RES_OK;
  return VFS_RES_ERR;
}

sint32_t spiffs_exists( const char *name ) {
  if (SPIFFS.exists(name + 6))
      return VFS_RES_OK;
  return VFS_RES_ERR;
}

sint32_t spiffs_close( vfs_file *fd ) {
  free(fd);
  return VFS_RES_OK;
}

sint32_t spiffs_read( vfs_file *fd, void *ptr, size_t len ) {
    sint32_t ret = ((File *)fd->file_obj)->read((uint8_t *) ptr, len);
    if (!ret)
       ret = VFS_RES_ERR;
    return ret;
}

sint32_t spiffs_write( vfs_file *fd, const void *ptr, size_t len ) {
    sint32_t ret = ((File *)fd->file_obj)->write((uint8_t *) ptr, len);
    if (!ret)
       ret = VFS_RES_ERR;
    return ret;
}

sint32_t spiffs_lseek( vfs_file *fd, sint32_t off, int whence ) {
  if ( ((File *)fd->file_obj)->seek(off, 
          (whence == VFS_SEEK_SET ? SeekSet : (whence == VFS_SEEK_CUR ? SeekCur : SeekEnd))) )
      return off;
  return VFS_RES_ERR;
}

sint32_t spiffs_flush( vfs_file *fd ) {
  ((File *)fd->file_obj)->flush();
  return VFS_RES_OK;
}

uint32_t spiffs_size( vfs_file *fd ) {
  return ((File *)fd->file_obj)->size();
}
