EXPIRATION_YEAR 2023
#define EXPIRATION_MONTH 10 // October
#define EXPIRATION_DAY 31   // 31st

int main() {
    // Get the current time
    time_t now;
    time(&now);
    
    // Convert to local time
    struct tm *local = localtime(&now);

    // Check if the current date is past the expiration date
    if (local->tm_year + 1900 > EXPIRATION_YEAR ||
        (local->tm_year + 1900 == EXPIRATION_YEAR && local->tm_mon + 1 > EXPIRATION_MONTH) ||
        (local->tm_year + 1900 == EXPIRATION_YEAR && local->tm_mon + 1 == EXPIRATION_MONTH && local->tm_mday > EXPIRATION_DAY)) {
        
        printf("The software has expired. Please contact support.\n");
        return EXIT_FAILURE;
    }

    printf("The software is valid. Welcome!\n");
    // Continue with the program

    return EXIT_SUCCESS;
}
