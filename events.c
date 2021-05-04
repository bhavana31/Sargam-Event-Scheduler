#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define EVENT_LIMIT 10 
#define DESC_LIMIT 101 
typedef struct { 
    int hour; 
    int minute; 
    char description[DESC_LIMIT];
} event;


// Add Event to calender
// Delete event 

void displayMenu() {
    puts("--------- Options --------\n"
        "  1. Add Event          \n"
        "  2. Display All Events \n"
        "  3. Delete event \n"
        "  4. Add Event to calender \n"
        "  5. Exit               \n"
        "-------------------------\n");
}

bool isNull(const event *e) { return e == NULL; }

event *initEvent() {
    event *e = (event*)malloc(sizeof(event));

    e->hour = 0;
    e->minute = 0;
    strcpy(e->description, "");

    return e;
}

int inputRange(const int min, const int max) {

    int input = 0;
    char temp[21];
    char *prompt = "Enter a number between %d and %d: ";

    printf(prompt, min, max);

    fgets(temp, 21, stdin);
    input = atoi(temp);

    while (input > max || input < min) {
        printf(prompt, min, max);
        fgets(temp, 21, stdin);
        input = atoi(temp);
    }

    return input;

}

event* newEvent(event *e) {

    if (isNull(e)) { 
        e = initEvent(); 
    }
 
    puts("Time:");

    e->hour = inputRange(0, 23);
    e->minute = inputRange(0, 59);

    puts("--------------------------------");
    puts("\nDescription:");
    printf("%s", "Enter a description: ");

    fgets(e->description, DESC_LIMIT, stdin);
    puts("Event added successfully.\n");
    
    return e;

}

void sort(event list[], const int size) {

    for (int i = 1; i < size; i++) {
        for (int j = i; j > 0 && (list[j - 1].hour > list[j].hour || (list[j - 1].hour == list[j].hour && list[j - 1].minute > list[j].minute)); j--) {
            int hourJ = list[j].hour;
            int minuteJ = list[j].minute;
            char descriptionJ[DESC_LIMIT];
            strcpy(descriptionJ, list[j].description);

            int hourJMinus1 = list[j - 1].hour;
            int minuteJMinus1 = list[j - 1].minute;
            char descriptionJMinus1[DESC_LIMIT];
            strcpy(descriptionJMinus1, list[j - 1].description);

            list[j].hour = hourJMinus1;
            list[j].minute = minuteJMinus1;
            strcpy(list[j].description, descriptionJMinus1);

            list[j - 1].hour = hourJ;
            list[j - 1].minute = minuteJ;
            strcpy(list[j - 1].description, descriptionJ);
        }
    }

}

void addEventAtIndex(event list[], const event e, const int i) {
    list[i].hour = e.hour;
    list[i].minute = e.minute;
    strcpy(list[i].description, e.description);
}
 

void insertEvent(event list[], int *size, event e) {

    addEventAtIndex(list, e, *size);
    (*size)++;

    sort(list, *size);
}

void printEvent(const event e) {

    char h1 = { (e.hour / 10) + '0' }; 
    char h2 = { (e.hour - (e.hour / 10) * 10) + '0' }; 

    char m1 = { (e.minute / 10) + '0' };
    char m2 = { (e.minute - (e.minute / 10) * 10) + '0' };

    printf("%c%c:%c%c - %s", h1, h2, m1, m2, e.description);

}

void displayAllEvents(const event list[], const int size) {

    if (size == 0) {
        puts("\n No events! \n");
        return;
    }

    char *seperator = "--------------------------------";

    printf("\n%s\n        Events List        \n%s\n\n", seperator, seperator);

    for (int i = 0; i < size; i++) {
        printf("[%d] ", i);
        printEvent(list[i]);

    }

    putchar('\n');

}


void deleteEvent(event list[], int *size) {

    if (*size == 0) { // If list is empty
        puts("\nEvent list already empty.\n");
        return;
    }

    char temp[21];
    int id;

    char *seperator = "\n--------------------------------";
    printf("%s\n          DELETE EVENT          %s\n\n", seperator, seperator);

    for (int i = 0; i < *size; i++) { 
        printf(" [%d] ", i);
        printEvent(list[i]);
    }

    printf("%s", "\nEnter the ID of an event to delete: ");

    fgets(temp, 21, stdin);
    id = atoi(temp);

    if (id > *size - 1) {
        printf("\n No events with given id, please select from list. %d\n", id);
        return;
    }

    printf("Event [%d] deleted successfully. \n\n", id);

    list[id].hour = 99;
    list[id].minute = 99;
    strcpy(list[id].description, "");

    if (id != (*size - 1)) {
        sort(list, *size);
    }

    (*size)--; 

}

char *encode(char *s) {

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == ' ') {
            s[i] = '_';
        }
    }

    return s;

}
char *replaceChars(const char *s, const char *oldW, const char *newW) { 
    char *result; 
    int i, cnt = 0; 
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
  
    // Counting the number of times old word 
    // occur in the string 
    for (i = 0; s[i] != '\0'; i++) { 
        if (strstr(&s[i], oldW) == &s[i]) { 
            cnt++; 
  
            // Jumping to index after the old word. 
            i += oldWlen - 1; 
        } 
    } 
  
    // Making new string of enough length 
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
    i = 0; 
    while (*s) { 
        // compare the substring with the result 
        if (strstr(s, oldW) == s) { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            s += oldWlen; 
        } else {
            result[i++] = *s++; 
        }
    } 
  
    result[i] = '\0'; 
    return result; 
} 
char *getISOTimeFormat(int hrs, int mins) { 

    char hoursString[20];
    char minsString[20];
    char dateString[50] = "2019.10.26";

    sprintf(hoursString, "%d", hrs); 
    sprintf(minsString, "%d", mins);

    strcat(dateString, " ");
    strcat(dateString, hoursString);
    strcat(dateString, ":");
    strcat(dateString, minsString);
    
    time_t result = 0;
    int year = 0, month = 0, day = 0, hour = 0, min = 0;
    char dest[70];                                                                                                                          
    struct tm *timeptr; 
    char colons[] = ":"; 
    char dash[] = "-";
    char emptyStr[] = "";
    char *formattedDateString = NULL; 
       
    sscanf(dateString, "%4d.%2d.%2d %2d:%2d", &year, &month, &day, &hour, &min);
    struct tm dateSections = {0};
    dateSections.tm_year = year - 1900;
    dateSections.tm_mon = month - 1;
    dateSections.tm_mday = day;
    dateSections.tm_hour = hour;
    dateSections.tm_min = min;
    
    result = mktime(&dateSections);
                                                         
    timeptr = localtime(&result);                                                   
    strftime(dest, sizeof(dest)-1, "%FT%TZ \n", timeptr);       

    formattedDateString = replaceChars(dest, colons, emptyStr); 
    formattedDateString = replaceChars(formattedDateString, dash, emptyStr);  
    formattedDateString[16] = '\0';

    return formattedDateString; 
}
void saveEventList(event list[], int size) {

    char temp[21];
    int id;
    char *startTimeString = NULL; 
    char *endTimeString = NULL; 

    char *seperator = "\n--------------------------------";
    printf("%s\n          EVENTS LIST          %s\n\n", seperator, seperator);

    for (int i = 0; i < size; i++) { 
        printf(" [%d] ", i);
        printEvent(list[i]);
    }

    printf("%s", "\nEnter the ID of an event to download: ");

    fgets(temp, 21, stdin);
    id = atoi(temp);

    if (id > size - 1) {
        printf("\n No events with given id, please select from list. %d\n", id);
        return;
    }

    startTimeString = getISOTimeFormat(list[id].hour, list[id].minute);
    endTimeString = getISOTimeFormat(list[id].hour + 1, list[id].minute);
    
    FILE *f = fopen("sargam-event.ics", "w");

    fprintf(f, "%s", "BEGIN:VCALENDAR");
    fprintf(f, "\n%s", "VERSION:2.0");
    fprintf(f, "\n%s", "BEGIN:VEVENT");
    fprintf(f, "\nDTSTART:%s", startTimeString);
    fprintf(f, "\nDTEND:%s", endTimeString);
    fprintf(f, "\nSUMMARY: Reminder: %s", list[id].description);
    fprintf(f, "\nDESCRIPTION:%s", list[id].description);
    fprintf(f, "\nLOCATION:%s", "New Horizon College of Engineering, Marathahalli, Bangalore, IN.");
    fprintf(f, "\nUID:%s", "http://www.newhorizonindia.edu");
    fprintf(f, "\n%s", "END:VEVENT");
    fprintf(f, "\n%s", "END:VCALENDAR");
    fclose(f);
    printf("\nPlease click the downloaded file to add event to your calender. File name - \"%s\". \n\n", "sargam-event.ics"); 

}
 
int main() {

    event list[EVENT_LIMIT];
    int index = 0;
    int selection = 0;
    char file[FILENAME_MAX];
    char response = 'Y';
    char temp[21];

    while (selection != 5) {

        displayMenu(); 

        printf("%s", "Please select an option: "); 
        fgets(temp, 21, stdin);
        selection = atoi(temp);

        switch (selection) {

            case 1:
                if (index + 1 > EVENT_LIMIT) {
                    printf("Maximum events %d !\n\n", index);
                    break;
                }
                insertEvent(list, &index, *newEvent(&list[index]));
                break;
            case 2: 
                displayAllEvents(list, index);
                break;
            case 3: 
                deleteEvent(list, &index);
                break;
            case 4: 
                if (index == 0) { // No events, don't save anything
                    puts("| You have no events in your schedule!\n");
                }
                else {
                    // printf("%s", "| Please enter a \"filename.txt\": ");
                    
                    saveEventList(list, index);
                }
                break;
            default: 
                break;
        }

    }

}