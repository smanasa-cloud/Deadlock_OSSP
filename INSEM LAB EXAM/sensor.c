#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t temp_pid, pressure_pid;
    int temp_status, pressure_status;
    int temperature = 28;
    int pressure = 1013;

    setbuf(stdout, NULL);

    printf("=== Dual-Sensor Data Aggregator ===\n");

    temp_pid = fork();

    if (temp_pid < 0) {
        perror("fork");
        return 1;
    }

    if (temp_pid == 0) {
        printf("Temperature Child: Reading = %d C\n",
               temperature);
        printf("Temperature Child: Logging completed.\n");
        exit(0);
    }

    pressure_pid = fork();

     if (pressure_pid < 0) {
        perror("fork");
        waitpid(temp_pid, NULL, 0);
        return 1;
    }

    if (pressure_pid == 0) {
        printf("Pressure Child: Reading = %d hPa\n",
               pressure);
        printf("Pressure Child: Logging completed.\n");
        exit(0);
    }

    printf("Gateway: Waiting for both children...\n");

    waitpid(temp_pid, &temp_status, 0);
    waitpid(pressure_pid, &pressure_status, 0);

    printf("\n=== Termination Status ===\n");

    if (WIFEXITED(temp_status))
        printf("Temperature child status: %d\n",
               WEXITSTATUS(temp_status));

    if (WIFEXITED(pressure_status))
        printf("Pressure child status: %d\n",
               WEXITSTATUS(pressure_status));

    if (WIFEXITED(temp_status) && WEXITSTATUS(temp_status) == 0 &&
        WIFEXITED(pressure_status) &&
        WEXITSTATUS(pressure_status) == 0) {

        printf("\nGateway: Both sensors completed.\n");

        if (temperature <= 40 &&
            pressure >= 950 && pressure <= 1050)
            printf("Aggregated Safety Index: 100/100\n");
        else
            printf("Aggregated Safety Index: 50/100\n");
    }

    printf("Gateway: Exiting.\n");

    return 0;
}
