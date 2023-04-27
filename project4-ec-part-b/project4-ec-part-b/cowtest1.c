#include "user.h"

const int data_size = 1000000;
const int data[data_size] = { /* fill with constant data */ };

int main() {
  for (int i = 0; i < 5; i++) {
    int start_ticks, end_ticks;
    start_ticks = uptime();

    int pid = fork();
    if (pid == 0) {
      // Child process: read and process data
      int sum = 0;
      for (int j = 0; j < data_size; j++) {
        sum += data[j];
      }
      printf(1, "Child %d: sum = %d\n", getpid(), sum);
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
