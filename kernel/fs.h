// On-disk file system format.
// Both the kernel and user programs use this header file.

#define ROOTINO 1  // root i-number
#define BSIZE 1024 // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock
{
    uint magic;      // Must be FSMAGIC
    uint size;       // Size of file system image (blocks)
    uint nblocks;    // Number of data blocks
    uint ninodes;    // Number of inodes.
    uint nlog;       // Number of log blocks
    uint logstart;   // Block number of first log block
    uint inodestart; // Block number of first inode block
    uint bmapstart;  // Block number of first free map block
};

// TODO: Follow instruction to make change of structure
#define FSMAGIC 0x10203040

#define NDIRECT 11                                 // 12->11 By 3.a., we sacrifice 1 block for "doubly-indirec"
#define NINDIRECT (BSIZE / sizeof(uint))           // = 1024/4 = 256
#define DNINDIRECT (NINDIRECT * NINDIRECT)         // = 256*256
#define MAXFILE (NDIRECT + NINDIRECT + DNINDIRECT) // = 256*256 + 256 + 11 = 65547

// On-disk inode structure
struct dinode
{
    short type;  // File type
    short major; // Major device number (T_DEVICE only)
    short minor; // Minor device number (T_DEVICE only)
    short nlink; // Number of links to inode in file system
    uint size;   // Size of file (bytes)
    /// NOTE: +2 instead of +1 because we NDIRECT is change from 12 to 11
    uint addrs[NDIRECT + 2]; // Data block addresses
};

// Inodes per block.
#define IPB (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb) ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB (BSIZE * 8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) ((b) / BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent
{
    ushort inum;
    char name[DIRSIZ];
};
