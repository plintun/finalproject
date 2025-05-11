/*
Adopted by CS department, West Valley College, Nov 2019.
*/

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

void displayMenu() {
    printf("\n=== Sensor Data Query Menu ===\n");
    printf("1. Show 3-day temperature & humidity average\n");
    printf("2. Show all sensor readings from last 3 days\n");
    printf("3. Exit\n");
    printf("Enter choice: ");
}
int main(void) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "localhost", "your_user", "your_pass", "sensordata", 0, NULL, 0)) {
        fprintf(stderr, "Connection error: %s\n", mysql_error(conn));
        exit(1);
    }

    int choice;
    while (1) {
        displayMenu();
        scanf("%d", &choice);

        if (choice == 1) {
            mysql_query(conn, "SELECT AVG(temperature), AVG(humidity) FROM readings WHERE datainserttime > NOW() - INTERVAL 3 DAY");
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            printf("3-day average temp: %s°F, humidity: %s%%\n", row[0], row[1]);
            mysql_free_result(res);
        }
        else if (choice == 2) {
            mysql_query(conn, "SELECT * FROM readings WHERE datainserttime > NOW() - INTERVAL 3 DAY");
            res = mysql_store_result(conn);
            printf("Temp | Humidity | Timestamp\n");
            while ((row = mysql_fetch_row(res))) {
                printf("%s°F | %s%% | %s\n", row[1], row[2], row[3]);
            }
            mysql_free_result(res);
        }
        else if (choice == 3) {
            break;
        }
        else {
            printf("Invalid choice. Try again.\n");
        }
    }

    mysql_close(conn);
    return 0;
}