#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

int main(int argc, char *argv[]) {
  int fd;
  struct superblock sb;
  struct dinode di;

  if (argc != 2) {
    printf(2, "Usage: fsck <file_system_image>\n");
    exit();
  }

  // Open the file system image
  if ((fd = open(argv[1], O_RDONLY)) < 0) {
    printf(2, "fsck: cannot open %s\n", argv[1]);
    exit();
  }

  // Read the superblock
  if (read(fd, &sb, sizeof(sb)) != sizeof(sb)) {
    printf(2, "fsck: reading superblock failed\n");
    close(fd);
    exit();
  }

  // Check the superblock for consistency
  if (sb.size <= 0 || sb.ninodes <= 0 || sb.nlog <= 0 || sb.log_start <= 0 || sb.inodestart <= 0 || sb.bmap_start <= 0) {
    printf(2, "fsck: corrupted superblock\n");
    close(fd);
    exit();
  }

  printf(1, "fsck: superblock is consistent\n");

  // Iterate through inodes
  for (int inum = 1; inum < sb.ninodes; inum++) {
    if (pread(fd, &di, sizeof(di), sb.inodestart * BSIZE + inum * sizeof(di)) != sizeof(di)) {
      printf(2, "fsck: reading inode %d failed\n", inum);
      close(fd);
      exit();
    }

    // Check inode consistency
    if (di.type < 0 || di.type > T_DEV) {
      printf(2, "fsck: inode %d is corrupted\n", inum);
      continue;
    }

    // The inode is consistent
    printf(1, "fsck: inode %d is consistent\n", inum);
  }

  close(fd);
  exit();
}
