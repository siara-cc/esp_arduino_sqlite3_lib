#include "vfs_int.h"
#include "spiffs_fns.h"
#include <FS.h>

File file;
vfs_file *spiffs_open( const char *name, const char *mode ) {
  vfs_file *ret = (vfs_file *) malloc(sizeof(vfs_file));
  ret->fs_type = VFS_FS_SPIFFS;
  ret->file_obj = NULL;
  file = SPIFFS.open(name + 6, mode);
  if (!file)
      return NULL;
  ret->file_obj = malloc(sizeof(File));
  memcpy(ret->file_obj, &file, sizeof(File));
  unsigned char *fo = (unsigned char*) ret->file_obj;
  for (int i = 0; i < sizeof(File); i++) {
    Serial.print((int)fo[i]);
    Serial.print(",");
  }
  Serial.println();
  Serial.print("position:");
  Serial.print((unsigned long)ret->file_obj);
  Serial.print(((File *)ret->file_obj)->position());
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
  if (fd->file_obj) {
      ((File *)fd->file_obj)->close();
      free(fd->file_obj);
  }
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
  Serial.print("position:");
  Serial.print((unsigned long)fd->file_obj);
  Serial.print(file.position());
  if (((File *)fd->file_obj)->size() < off)
      return VFS_RES_ERR;
  if (((File *)fd->file_obj)->position() == off)
      return off;
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
