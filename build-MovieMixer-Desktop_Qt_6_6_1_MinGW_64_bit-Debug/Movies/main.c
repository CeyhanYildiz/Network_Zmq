#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_GENRE_LENGTH 20
#define MAX_LINE_LENGTH 256

// Function to pick a random movie from a CSV file based on genre
void pickRandomMovie(char *genre) {
    // Construct the filename based on the genre
    char filename[MAX_GENRE_LENGTH + 5]; // 5 for ".csv\0"
    sprintf(filename, "%s.csv", genre);

    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }

    // Count the number of lines in the file
    int numLines = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        numLines++;
    }

    // Close and reopen the file to reset the file pointer
    fclose(file);
    file = fopen(filename, "r");

    // Generate a random line number
    srand(time(NULL));
    int randomLine = rand() % numLines;

    // Read and discard lines until the random line is reached
    for (int i = 0; i < randomLine; i++) {
        fgets(line, sizeof(line), file);
    }

    // Remove the newline character from the end of the line
    line[strcspn(line, "\n")] = '\0';

    // Extract the movie title from the randomly selected line
    char *token = strtok(line, ";"); // Tokenize using semicolon as delimiter
    printf("Random Movie from %s:\n", genre);
    
    // Print only relevant fields for the movie title
    while (token != NULL) {
        // Skip empty fields
        if (strcmp(token, "") != 0 && strcmp(token, " ") != 0)
            printf("%s ", token);
        token = strtok(NULL, ";");
    }
    printf("\n");

    // Close the file
    fclose(file);
}

int main() {

    while (1)
    {
        // List of genres
        char genres[][MAX_GENRE_LENGTH] = {"Action", "Adventure", "Animation", "Comedy", "Crime",
                                            "Documentary", "Drama", "Family", "Fantasy", "Historical",
                                            "Horror", "Musical", "Mystery", "Romance", "Sci-Fi",
                                            "Sports", "Thriller", "War"};
        
        // Print available genres
        printf("Available genres:\n");
        for (int i = 0; i < sizeof(genres) / sizeof(genres[0]); i++) {
            printf("%d. %s\n", i + 1, genres[i]);
        }
        
        // Prompt user to choose a genre
        int choice;
        printf("Enter the number corresponding to the desired genre: ");
        scanf("%d", &choice);
        
        // Validate user input
        if (choice < 1 || choice > sizeof(genres) / sizeof(genres[0])) {
            printf("Invalid choice!\n");
            return 1;
        }
        
        // Pick a random movie from the selected genre
        pickRandomMovie(genres[choice - 1]);
    }
    return 0;
}
