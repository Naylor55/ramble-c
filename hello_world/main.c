#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep function (if you are on Unix-like systems)

int main() {
    while (1) {  // Infinite loop
        printf("Hello, World!\n");
        sleep(1);  // Sleep for 1 second to avoid flooding the terminal with messages
    }
    return 0;
}
