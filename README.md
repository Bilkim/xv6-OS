# xv6-OS
xv6-Operating System modification
# 5a. Implementing Better Locks
1.	spinlock_t:
•	A simple spinlock implementation that uses an atomic integer to represent the lock state.
•	spinlock_init initializes the lock to an unlocked state.
•	spinlock_acquire spins in a loop while trying to acquire the lock until it succeeds.
•	spinlock_release releases the lock by setting the lock state to 0.
2.	ticket_lock_t:
•	A ticket lock implementation that uses two atomic integers to represent the ticket number and the currently serving ticket number.
•	ticket_lock_init initializes the lock to an initial state.
•	ticket_lock_acquire increments the ticket number and spins in a loop while waiting for the currently serving ticket number to match the thread's ticket number.
•	ticket_lock_release increments the currently serving ticket number.
3.	rw_lock_t:
•	A reader-writer lock implementation that uses a combination of a spinlock and a ticket lock for writers, and an atomic integer for readers.
•	rw_lock_init initializes the lock to an initial state.
•	rw_lock_acquire_reader increments the number of active readers.
•	rw_lock_release_reader decrements the number of active readers.
•	rw_lock_acquire_writer increments the number of waiting writers, acquires the ticket lock, and spins in a loop while waiting for all active readers to finish. Then it sets the writer active flag to 1.
•	rw_lock_release_writer sets the writer active flag to 0 and releases the ticket lock.
•	rw_lock_try_acquire_writer attempts to acquire the writer lock without blocking and returns a success or failure status.
4.	queue_lock_t:
•	A lock-free queue implementation that uses an atomic integer for the lock state and atomic pointers for the queue head and tail.
•	queue_lock_init initializes the queue and the lock to an initial state.
•	queue_lock_acquire adds a new node to the queue and spins in a loop while waiting for the lock to become available. When the lock is available, it sets the lock state to 1 and updates the queue head.
•	queue_lock_release removes the first node from the queue and updates the queue head and tail. If the queue is now empty, it sets the lock state to 0.
5.	main functions for performance testing:
•	These are main functions that create multiple threads and perform a large number of lock acquisitions and releases, measuring the time taken by the lock operations.
•	There are three main functions, each for testing a different type of lock: ticketlock, non-starving reader-writer lock, and queue lock.

# 5b. Copy-on-write fork
The fork.c tests the behavior of the fork system call when multiple processes share the same memory page and one of them writes to it. The program creates N child processes using fork and attempts to modify a shared memory page in each child process, causing a page fault and triggering a copy-on-write operation by the operating system.
The purpose of this program is to test the correctness of the copy-on-write mechanism in the operating system's virtual memory subsystem. When a process forks, the operating system creates a new process that shares the same memory pages with the parent process, but marks them as read-only. If either process attempts to write to a shared memory page, a page fault occurs and the operating system creates a copy of the page for the process that wrote to it, and marks it as writable. This allows multiple processes to share the same memory pages without interfering with each other's data.
By testing the behavior of the fork system call in this way, the program ensures that the operating system correctly implements the copy-on-write mechanism and that multiple processes can safely share memory pages without causing data corruption or other errors.
The purpose of forktest1.c program is to compare the performance of basic fork and copy-on-write fork in terms of the time it takes to create and exit multiple child processes. By measuring the elapsed time, the program provides a quantitative comparison of the two approaches to process creation. Forktest2.c demonstrates the copy-on-write fork implementation, which delays the creation of a new process until it modifies a shared page. In this program, the child process allocates a memory buffer and writes to it, triggering a page fault that causes the kernel to create a new copy of the page with write permissions for the child process. The parent process and child process each have their own copy of the page, and modifications made by one process do not affect the other process.
# 5c. Implementing the Fast File System
To build a new mkfs tool with the fast file system optimizations, we need to modify the existing code and add new functionality to handle block groups, directory boundary-based block allocation, and large files. Here is an outline of the changes we need to make:
1.	Block groups: We need to divide the disk into block groups and allocate blocks from within each group. This will help to reduce fragmentation and improve performance. We can add a new field to the superblock to store the number of block groups and the size of each group. We can also modify the block allocation code to select a block group based on the inode number.
2.	Directory boundary-based block allocation: We need to allocate blocks for files based on the directory boundaries, rather than allocating blocks sequentially. This will help to reduce fragmentation and improve performance. We can add a new field to the inode to store the directory inode number, and modify the block allocation code to select blocks based on the directory inode number.
3.	Large file support: We need to handle block allocation for large files properly. We can add a new field to the inode to store the number of blocks allocated to the file, and modify the block allocation code to allocate more blocks as needed.


# 5d. File System Checking
In order to generate broken file systems, we can modify mkfs to create file systems that fail one or more of the basic file system checks. Here are some ways to do this:
1.	Superblock: Modify mkfs to create a file system with a corrupted superblock. For example, change the superblock signature to a value that does not match the file system type, or set the number of inodes to a value that exceeds the number of blocks available in the file system.
2.	Block size: Modify mkfs to create a file system with an unsupported block size. For example, set the block size to a value that is smaller than the minimum block size, or set it to a value that is not a power of two.
3.	Inode size: Modify mkfs to create a file system with an unsupported inode size. For example, set the inode size to a value that is smaller than the minimum inode size, or set it to a value that is not a multiple of the block size.
4.	Number of inodes: Modify mkfs to create a file system with too few or too many inodes. For example, set the number of inodes to a value that is smaller than the number of reserved inodes, or set it to a value that exceeds the maximum number of inodes supported by the file system.
5.	Number of blocks: Modify mkfs to create a file system with too few or too many blocks. For example, set the number of blocks to a value that is smaller than the number of reserved blocks, or set it to a value that exceeds the maximum number of blocks supported by the file system.
6.	Block group descriptor: Modify mkfs to create a file system with a corrupted block group descriptor. For example, set the number of blocks in a block group to a value that exceeds the number of blocks available in the file system, or set the block bitmap block number to a value that is outside the block group.
By modifying mkfs to create file systems with one or more of these conditions, we can generate broken file systems that fail one or more of the basic file system checks. These file systems can be used for testing purposes, to ensure that file system utilities and tools can handle error conditions correctly.


