#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fd1[2]; // for sending the original message from the first to the second process
    int fd2[2]; // for sending the modified message from the second to the first process
    pid_t pid;

    // Create the pipes
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork(); // create a child process

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // This block is executed by the parent process
    if (pid > 0) {
        // Declare and initialize the array to be sent
        int arr[] = {5, 6, 7, 8, 9};

        // Close the unused end of the pipes
        close(fd1[0]);
        close(fd2[1]);

        // Send the array through the first pipe
        write(fd1[1], arr, sizeof(arr));

        // Receive the reversed array through the second pipe
        int rev_arr[5];
        read(fd2[0], rev_arr, sizeof(rev_arr));

        // Print the reversed array
        for (int i = 0; i < 5; i++) {
            printf("%d ", rev_arr[i]);
        }
        printf("\n");
    }
    // This block is executed by the child process
    else {
        // Close the unused end of the pipes
        close(fd1[1]);
        close(fd2[0]);

        // Receive the array through the first pipe
        int arr[5];
        read(fd1[0], arr, sizeof(arr));

        // Reverse the array
        for (int i = 0; i < 5 / 2; i++) {
            int temp = arr[i];
            arr[i] = arr[5 - i - 1];
            arr[5 - i - 1] = temp;
        }

        // Send the reversed array through the second pipe
        write(fd2[1], arr, sizeof(arr));
    }

    return 0;
}
