// internal definitions for vfs

#ifndef __VFS_INT_H__
#define __VFS_INT_H__

#include <string.h>
#include <stdint.h>
#include <c_types.h>

enum vfs_filesystems {
  VFS_FS_NONE = 0,
  VFS_FS_SPIFFS,
  VFS_FS_FATFS
};

enum vfs_seek {
  VFS_SEEK_SET = 0,
  VFS_SEEK_CUR,
  VFS_SEEK_END
};

enum vfs_result {
  VFS_RES_OK  = 0,
  VFS_RES_ERR = -1
};

struct vfs_time {
  int year, mon, day;
  int hour, min, sec;
};
typedef struct vfs_time vfs_time;

// generic file descriptor
typedef struct {
  int fs_type;
  void *file_obj;
} vfs_file ;

#define FS_OBJ_NAME_LEN 50

// stat data
struct vfs_stat {
  uint32_t size;
  char name[FS_OBJ_NAME_LEN+1];
  struct vfs_time tm;
  uint8_t tm_valid;
  uint8_t is_dir;
  uint8_t is_rdonly;
  uint8_t is_hidden;
  uint8_t is_sys;
  uint8_t is_arch;
};

// generic volume descriptor
typedef struct {
  int fs_type;
  void *vol_obj;
  int num;
} vfs_vol;

#endif
