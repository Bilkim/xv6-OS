#include "user.h"

const int data_size = 100000;
int data[data_size];

int main() {
  for (int i = 0; i < 5; i++) {
    int start_ticks, end_ticks;
    start_ticks = uptime();

    int pid = fork();
    if (pid == 0) {
      // Child process: modify data
      for (int j = 0; j < data_size; j++) {
        data[j] = data[j] * (i + 1);
      }
      printf(1, "Child %d: modified data\n", getpid());
      exit();
    }
  }

  // Parent process: wait for children
  for (int i = 0; i < 5; i++) {
    wait();
    end_ticks = uptime();
    printf(1, "Elapsed time: %d ticks\n", end_ticks - start_ticks);
  }

  exit();
}
