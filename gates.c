# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <math.h>

/* a struct representing one flight with all included variables;
    information given from one row entry in csv file 
    note: relevant variables are departure and arrival airports and departure and arrival times */
    struct Flight {
        char flight_code[10];
        int flight_num;
        char depAirport[10];
        char arrAirport[10];
        int depTime;
        int arrTime;
    };

/* sorting method 1; sorting the array based on arrival and departure times in order from least to greatest,
    based on atlanta airport associated times */
void insertionsort(struct Flight flights[2208]) {
    char atl[4] = "ATL";            
    struct Flight key;
    int comp;                       // flight time to compare to when looping through
    int keytime;                    // key flight's arrival or departure time
    int i = 0;

    for (i = 1; i < 2208; i++) {
        key = flights[i];

        int comparrival = 0;        // comparisons flight's time is an arrival time, set to false
        int keyarrival = 0;         // key flight's time is an arrival time, set to false

        // implement insertion sort
        int j;
        for(j = i-1; j >= 0; j--) { 
            
            // if the arrival airport is ATL, compare using flight's arrival time
            if (strcmp(flights[j].arrAirport, atl) == 0) {
                comp = flights[j].arrTime;
                comparrival = 1;            // comparison flight is using an arrival time 
            // if the departure airport is ATL, compare using flight's departure time
            } else if (strcmp(flights[j].depAirport, atl) == 0) {
                comp = flights[j].depTime;
            }
            // same logic as above for key flight
            if (strcmp(key.arrAirport, atl) == 0) {
                keytime = key.arrTime;
                keyarrival = 1;             
            } else if (strcmp(key.depAirport, atl) == 0) {
                keytime = key.depTime;
            }
            // if key flight's time is earlier, swap flight positions in the array
            if (comp > keytime) {
                flights[j+1] = flights[j];
            // if flight times are the same, swap if key flight is arriving and comp flight is departing
            } else if (comp == keytime) {
                if (keyarrival && !comparrival) {
                    flights[j+1] = flights[j];
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        flights[j+1] = key;
    }
}

/* helper method used in MergeSort, merges two subarrays into one sorted list */
void merge(struct Flight flights[], int p, int q, int r) {
    int n1 = q-p+1;                 // length of left subarray
    int n2 = r-q;                   // length of right subarray
    struct Flight L[n1];            // temporary subarray (smaller values)
    struct Flight R[n2];            // temporary subarray (larger values)
    
    // copies half of the data in original flight array to temporary array
    for (int i = 0; i < n1; i++) {
        L[i] = flights[p+i];
    }

    // copies the other half of the data in the original flight array to temporary array
    for (int i = 0; i < n2; i++) {
        R[i] = flights[q+1+i];
    }

    int i = 0;
    int j = 0;
    int k; 
    char atl[4] = "ATL";

    // implement merge operation
    for (k = p; i < n1 && j < n2 && k <= r; k++) {

        int left;                       // left array's flight time (for comparison)
        int right;                      // right array's flight time (for comparison)
        int leftarrival = 0;
        int rightarrival = 0;

        // if the left array's flight arrival airport is ATL, compare using flight's arrival time
        if (strcmp(L[i].arrAirport, atl) == 0) {
            left = L[i].arrTime;
            leftarrival = 1;            // left flight is using an arrival time 
        // if the departure airport is ATL, compare using flight's departure time
        } else if (strcmp(L[i].depAirport, atl) == 0) {
            left = L[i].depTime;
        }
        // same logic for flight in the right array
        if (strcmp(R[j].arrAirport, atl) == 0) {
            right = R[j].arrTime;
            rightarrival = 1;            // right flight is using an arrival time 
        } else if (strcmp(R[j].depAirport, atl) == 0) {
            right = R[j].depTime;
        }

        // if the left array's flight time is earlier than the right's, 
        // add the left flight to the original array
        if (left < right) {
            flights[k] = L[i];
            i++;
        // if the flight times are the same, add left flight to the original array if
        // it's arriving flight and right flight is departing
        // otherwise, add right flight to the original array
        } else if (left == right) {         
            if (leftarrival && !rightarrival) {
                flights[k] = L[i];
                i++;
            } else {
                flights[k] = R[j];
                j++;
            }
        } else {
            flights[k] = R[j];
            j++;
        }
    }

    // make sure all values have been accounted for and add any remaining flights to the array
    for (; i < n1; i++) {
        flights[k] = L[i];
        k++;
    }
    for (; j < n2; j++) {
        flights[k] = R[j];
        k++;
    }
}

/* sorting method 2; sorts the flights from least to greatest based on ATL arrival & departure times */
void MergeSort(struct Flight flights[2208], int p, double r) {
    if (p < r) {
        double q = floor((p+r)/2);
        MergeSort(flights, p, q);
        MergeSort(flights, q+1, r);
        merge(flights, p, q, r);
    }
}

/* determine output by creating a running sum;
    incoming flights add 1, departing flights subtract 1;
    prints output to the screen */
void countgates(struct Flight flights[2208]) {
    int gates = 0;          // total running sum
    int max = 0;            // max value achieved (most arriving flights at once)
    int min = 0;            // min value achieved (most departing flights at once)

    char atl[4] = "ATL";
    // iterate through flight array updating the running sum, max, and min values
    for (int i = 0; i < 2208; i++) {
        if (strcmp(flights[i].arrAirport, atl) == 0) {
            gates += 1;
        }
        else if (strcmp(flights[i].depAirport, atl) == 0) {
            gates -= 1;
        }

        if (gates > max) {
            max = gates;
        }
        if (gates < min) {
            min = gates;
        }
    }

    // gates to accomodate highest number of arriving flights (max) and highest number of departing flights
    int totalgates = max - min;        
    printf("The number of gates needed is: %d\n", totalgates);
    printf("The number of planes that should be parked by midnight: %d\n", -min);
}

int main(int argc, char *argv[]) {
    
    /* open file */
    FILE *data;
    data = fopen("flights_file.csv", "r");

    /* initialize an array of structs to hold all flights */
    struct Flight flights[2208];

    /* read flights in from file */
    // temporary values to copy individual flights into the array 
    char tempfc[10];
    int tempfnum;
    char tempdair[10];
    char tempaair[10];
    int tempdtime;
    int tempatime;

    // create a flight to add to the array, then copy attributes for each flight
    // add flight to the array 
    int i = 0;
    while (i < 2208) {
        struct Flight flight;
        fscanf(data, "%2s,%d,%3s,%3s,%d,%d\n", tempfc, &tempfnum, tempdair, tempaair, &tempdtime, &tempatime);
        strcpy(flight.flight_code, tempfc);
        flight.flight_num = tempfnum;
        strcpy(flight.depAirport, tempdair);
        strcpy(flight.arrAirport, tempaair);
        flight.depTime = tempdtime;
        flight.arrTime = tempatime;
        flights[i] = flight;
        i++;
    }

    fclose(data);

    // determines if command line argument was given to choose sorting method
    // default uses insertion sort, "-ms" tag calls mergesort
    char ms[4] = "-ms";
    if (argc > 1) {
        if (strcmp(argv[1], ms) == 0){
            MergeSort(flights, 0, 2207);
        }
    } else {
        insertionsort(flights);
    }

    countgates(flights);

    return 0;
}