#ifndef __SPIFFS_FNS_H__
#define __SPIFFS_FNS_H__

#include "vfs_int.h"

void set_spiffs_file_obj(void *in_filep);
vfs_file *spiffs_open( const char *name, const char *mode );
sint32_t spiffs_remove( const char *name );
sint32_t spiffs_exists( const char *name );
sint32_t spiffs_close( vfs_file *fd );
sint32_t spiffs_read( vfs_file *fd, void *ptr, size_t len );
sint32_t spiffs_write( vfs_file *fd, const void *ptr, size_t len );
sint32_t spiffs_lseek( vfs_file *fd, sint32_t off, int whence );
sint32_t spiffs_flush( vfs_file *fd );
uint32_t spiffs_size( vfs_file *fd );

#endif
