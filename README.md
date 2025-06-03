# Dining Philosopher Problem (C Implementation)

This project provides a C implementation of the classic Dining Philosopher Problem using POSIX threads (pthreads). The solution demonstrates thread synchronization, mutual exclusion, and the use of condition variables to avoid deadlock and starvation.

## Features

- Five philosophers (threads) alternately think and eat.
- Each philosopher needs two spoons (shared resources) to eat.
- Uses mutexes and condition variables to synchronize access to spoons.
- Prevents deadlock and ensures fairness among philosophers.

## File Structure

- `assignment_din_ph.c` — Main implementation file.
- `din_ph.h` — Header file with structure definitions (not included here).

## How to Build

Make sure you have GCC and pthreads installed.  
To compile, run:

```sh
gcc assignment_din_ph.c -o din_ph -lpthread
```

If you have a header file or additional source files, include them as needed.

## How to Run

```sh
./din_ph
```

## Code Overview

- **phil_t**: Philosopher structure.
- **spoon_t**: Spoon structure (with mutex and condition variable).
- **philosopher_fn**: Thread function for each philosopher.
- **philosopher_get_access_both_spoons**: Attempts to acquire both spoons.
- **philosopher_release_both_spoons**: Releases both spoons after eating.

## Author

- Original: Abhishek Sagar (Juniper Networks)
- Modified by: Howie Chang

---

This project is for educational purposes and demonstrates basic multithreading and synchronization concepts in C.
