#include <stdio.h>
#include <stdlib.h>
static int BucketIDStart = 500;
typedef struct Time
{
    int hrs;
    int min;
} Time;

typedef struct FlightScheduleTag
{
    int flightId;
    Time departureTime;
    Time eta;
    struct FlightScheduleTag *next;

} FlightSchedule;

typedef struct BucketTag
{
    int bucketId;
    Time etaStart;
    Time etaEnd;
    FlightSchedule *flightSchedule;
    struct BucketTag *next;
} Bucket;

int timeDiff(Time a, Time b)
{
    int diff = 0;
    diff = (a.hrs * 60 + a.min) - (b.hrs * 60 + b.min);
    return diff;
}

int maxTime(Time a, Time b)
{
    int diff = timeDiff(a, b);
    if (diff == 0)
    {
        return 0;
    }
    else if (diff > 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

Bucket *createBucket(int bucketId, Time etaStart, Time etaEnd);
FlightSchedule *createFlightSchedule(int flightId, Time departureTime, Time eta);
void Print(Bucket *bucket);
void showStatus(Bucket *buketList, int flightId);
void insertFlightPlan(Bucket **bucketList, int flightId, Time departureTime, Time ETA);
Bucket *insertBucket(Bucket *bucketList, Bucket *newBucket);
void deleteFlightPlan(Bucket **bucketList, int flightId);
void reArrangeBucket(Bucket **bucket, Time t);

Bucket *createBucket(int bucketId, Time etaStart, Time etaEnd)
{
    Bucket *newBucket = (Bucket *)malloc(sizeof(Bucket));
    newBucket->bucketId = bucketId;
    newBucket->etaStart = etaStart;
    newBucket->etaEnd = etaEnd;
    newBucket->flightSchedule = NULL;
    newBucket->next = NULL;
    return newBucket;
}

FlightSchedule *createFlightSchedule(int flightId, Time departureTime, Time eta)
{
    FlightSchedule *newSchedule = (FlightSchedule *)malloc(sizeof(FlightSchedule));
    newSchedule->flightId = flightId;
    newSchedule->departureTime = departureTime;
    newSchedule->eta = eta;
    newSchedule->next = NULL;
    return newSchedule;
}

int liesBetween(Time etaStart, Time etaEnd, Time GivenETA)
{
    int ans = 0;
    // if(etaEnd.hrs == 0) etaEnd.hrs = 24 ;
    if (timeDiff(GivenETA, etaStart) >= 0 && timeDiff(GivenETA, etaStart) < 60 && timeDiff(etaEnd, GivenETA) >= 0 && timeDiff(etaEnd, GivenETA) < 60)
    {
        ans = 1;
    }
    return ans;
}

void Print(Bucket *bucket)
{
    Bucket *temp = bucket;
    while (temp)
    {
        printf("\n");
        printf("Bucket Id: %d\n", temp->bucketId);
        printf("ETA Start: %d:%d\n", temp->etaStart.hrs, temp->etaStart.min);
        printf("ETA End: %d:%d\n", temp->etaEnd.hrs, temp->etaEnd.min);
        FlightSchedule *temp2 = temp->flightSchedule;
        while (temp2)
        {
            printf("Flight Id: %d\n", temp2->flightId);
            printf("Departure Time: %d:%d\n", temp2->departureTime.hrs, temp2->departureTime.min);
            printf("ETA: %d:%d\n", temp2->eta.hrs, temp2->eta.min);
            temp2 = temp2->next;
        }
        temp = temp->next;
        printf("\n");
    }
}

void showStatus(Bucket *bucketList, int flightId)
{
    Bucket *temp = bucketList;
    int flag = 1;
    while (temp && flag)
    {
        FlightSchedule *temp2 = temp->flightSchedule;
        while (temp2 && flag)
        {
            if (temp2->flightId == flightId)
            {
                printf("Flight Id: %d\n", temp2->flightId);
                printf("Departure Time: %d:%d\n", temp2->departureTime.hrs, temp2->departureTime.min);
                printf("ETA: %d:%d\n", temp2->eta.hrs, temp2->eta.min);
                flag = 0;
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    if (flag)
    {
        printf("Flight Id %d does not exist\n", flightId);
    }
    printf("\n");
    return;
}

Bucket *insertBucket(Bucket *bucketList, Bucket *newBucket)
{

    if (bucketList == NULL)
    {
        bucketList = newBucket;
    }
    else if (bucketList->etaStart.hrs > newBucket->etaEnd.hrs)
    {
        newBucket->next = bucketList;
        bucketList = newBucket;
    }
    else
    {
        Bucket *temp = bucketList;
        Bucket *prev;
        int flag = 1;
        while (temp && flag)
        {
            if (temp->etaEnd.hrs < newBucket->etaStart.hrs)
            {
                prev = temp;
                temp = temp->next;
            }
            else
            {
                flag = 0;
            }
        }
        if (flag && !temp)
        {
            newBucket->next = temp;
            prev->next = newBucket;
        }
        else if (temp)
        {
            prev->next = newBucket;
            newBucket->next = temp;
        }
    }
    return bucketList;
}
void insertFlightPlan(Bucket **bucketList, int flightId, Time departTime, Time ETA)
{
    FlightSchedule *newSchedule = createFlightSchedule(flightId, departTime, ETA);
    if (*bucketList == NULL)
    {
        Time etaS, etaE;
        etaS.hrs = ETA.hrs;
        etaS.min = 0;
        etaE.hrs = ETA.hrs;
        etaE.min = 59;
        Bucket *newBucket = createBucket(BucketIDStart++, etaS, etaE);
        *bucketList = insertBucket(*bucketList, newBucket);
        (*bucketList)->flightSchedule = newSchedule;
    }
    else
    {
        Bucket *temp = *bucketList;
        Bucket *prev;
        int flag = 1;
        while (temp && flag)
        {
            if (liesBetween(temp->etaStart, temp->etaEnd, ETA))
                flag = 0;
            else
            {
                prev = temp;
                temp = temp->next;
            }
        }

        if (flag == 1)
        {
            printf("New bucket added with bucketID %d\n", BucketIDStart);
            Time etaS, etaE;
            etaS.hrs = ETA.hrs;
            etaS.min = 0;
            etaE.hrs = ETA.hrs;
            etaE.min = 59;
            Bucket *newBucket = createBucket(BucketIDStart++, etaS, etaE);
            newBucket->flightSchedule = newSchedule;
            *bucketList = insertBucket(*bucketList, newBucket);
        }

        else
        {
            if (temp->flightSchedule == NULL)
            {
                temp->flightSchedule = newSchedule;
            }
            else if (maxTime(newSchedule->departureTime, temp->flightSchedule->departureTime) < 0)
            {
                newSchedule->next = temp->flightSchedule;
                temp->flightSchedule = newSchedule;
            }
            else
            {
                FlightSchedule *prev = temp->flightSchedule;
                FlightSchedule *curr = temp->flightSchedule;
                while (curr && maxTime(newSchedule->departureTime, curr->departureTime) >= 0)
                {
                    prev = curr;
                    curr = curr->next;
                }
                prev->next = newSchedule;
                newSchedule->next = curr;
            }
        }
    }
    return;
}
void deleteFlightPlan(Bucket **bucketList, int flightId)
{
    Bucket *temp = *bucketList;
    int flag = 1;
    while (temp && flag)
    {
        FlightSchedule *curr = temp->flightSchedule;
        if (curr && curr->flightId == flightId)
        {
            flag = 0;
            FlightSchedule *ptr = curr;
            temp->flightSchedule = curr->next;
            free(ptr);
        }
        else
        {
            FlightSchedule *prev = temp->flightSchedule;
            while (curr && flag)
            {
                if (curr->flightId == flightId)
                {
                    flag = 0;
                    FlightSchedule *ptr = curr;
                    prev->next = curr->next;
                    free(ptr);
                }
                else
                {
                    prev = curr;
                    curr = curr->next;
                }
            }
        }
        temp = temp->next;
    }
    if (flag == 1)
    {
        printf("Flight Id %d does not exist\n", flightId);
    }
    printf("\n");
    return;
}

FlightSchedule *mergeSortedLists(FlightSchedule *l1, FlightSchedule *l2)
{
    Time notTime;
    notTime.hrs = -1;
    notTime.min = -1;
    FlightSchedule *dummy = createFlightSchedule(-1, notTime, notTime);
    FlightSchedule *current = dummy;

    while (l1 != NULL && l2 != NULL)
    {
        if (l1->departureTime.hrs < l2->departureTime.hrs)
        {
            current->next = l1;
            l1 = l1->next;
        }
        else if (l1->departureTime.hrs == l2->departureTime.hrs)
        {
            if (l1->departureTime.min < l2->departureTime.min)
            {
                current->next = l1;
                l1 = l1->next;
            }
            else
            {
                current->next = l2;
                l2 = l2->next;
            }
        }
        else
        {
            current->next = l2;
            l2 = l2->next;
        }
        current = current->next;
    }

    // If one of the lists is longer than the other or if either of the list is empty
    if (l1 != NULL)
    {
        current->next = l1;
    }
    else
    {
        current->next = l2;
    }
    FlightSchedule *ans = dummy->next;
    free(dummy);
    return ans;
}

void reArrangeBucket(Bucket **bucket, Time t)
{
    Time notTime;
    notTime.hrs = -1;
    notTime.min = -1;
    Bucket *tempBucket = createBucket(-1, notTime, notTime);
    Bucket *curr = *bucket;
    Bucket *prev = NULL;
    Bucket *start;
    Bucket *n;

    if (t.hrs == 0 && t.min == 0)
    {
        printf("!! Original Time Interval !!\n");
    }
    else
    {
        while (curr)
        {
            curr->etaStart.min = t.min;
            curr->etaEnd.hrs = (curr->etaEnd.hrs + 1) % 24;
            curr->etaEnd.min = t.min - 1;
            FlightSchedule *temp = curr->flightSchedule;
            FlightSchedule *dummy = createFlightSchedule(-11, notTime, notTime);
            FlightSchedule *dummyTail = dummy;
            FlightSchedule *store = createFlightSchedule(-1, notTime, notTime), *storeTail = store;
            while (temp)
            {
                FlightSchedule *next = temp->next;
                if (temp->eta.min >= t.min)
                {
                    dummyTail->next = temp;
                    dummyTail = dummyTail->next;
                }
                else
                {
                    if (storeTail == NULL)
                    {
                        storeTail = temp;
                        store = storeTail;
                    }
                    else
                    {
                        storeTail->next = temp;
                        storeTail = storeTail->next;
                    }
                }
                temp->next = NULL;
                temp = next;
            }
            // adding data to previous bucket
            curr->flightSchedule = NULL;
            curr->flightSchedule = dummy->next;
            if (prev == NULL)
            {
                tempBucket->flightSchedule = store->next;
            }
            else
            {
                FlightSchedule *l1 = prev->flightSchedule;
                FlightSchedule *l2 = store->next;
                prev->flightSchedule = mergeSortedLists(l1, l2);
            }
            if (curr->next == NULL)
            {
                FlightSchedule *l1 = curr->flightSchedule;
                FlightSchedule *l2 = tempBucket->flightSchedule;
                curr->flightSchedule = mergeSortedLists(l1, l2);
            }
            prev = curr;
            curr = curr->next;
        }
        // changing *bucket
        Bucket *Start = *bucket;
        Bucket *current = *bucket;
        Bucket *previous = NULL;
        if (t.hrs != 0)
        {
            while (current->etaStart.hrs < t.hrs)
            {
                previous = current;
                current = current->next;
            }
            *bucket = current;
            previous->next = NULL;
            current = (*bucket)->next;
            while (current->next)
            {
                current = current->next;
            }
            current->next = Start;
        }
    }
}

void printMenu()
{
    printf("1. Add a new flight\n");
    printf("2. Delete a flight\n");
    printf("3. Show Status\n");
    printf("4. Rearrange \n");
    printf("5. Exit\n");
}

int main()
{

    Bucket *bucketList = NULL;
    int flightId;
    Time departTime;
    Time ETA;
    Time currTime;
    FILE *fptr;
    fptr = fopen("data.txt", "r");
    if (fptr == NULL)
    {
        printf("File not found\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        while (!feof(fptr))
        {
            fscanf(fptr, "%d %d %d %d %d", &flightId, &departTime.hrs, &departTime.min, &ETA.hrs, &ETA.min);
            insertFlightPlan(&bucketList, flightId, departTime, ETA);
        }
    }
    Print(bucketList);
    fclose(fptr);
    int choice;
    while (1)
    {
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the Flight Id\n");
            scanf("%d", &flightId);
            printf("Enter Departure Time (hrs min)\n");
            scanf("%d %d", &departTime.hrs, &departTime.min);
            printf("Enter Expected Time of Arrival (hrs min)\n");
            scanf("%d %d", &ETA.hrs, &ETA.min);
            insertFlightPlan(&bucketList, flightId, departTime, ETA);
            Print(bucketList);
            break;
        case 2:
            printf("Enter the Flight Id (100 to 340 already exists)\n");
            scanf("%d", &flightId);
            deleteFlightPlan(&bucketList, flightId);
            Print(bucketList);
            break;
        case 3:
            printf("Enter the Flight Id (100 to 340 only)\n");
            scanf("%d", &flightId);
            printf("\n");
            showStatus(bucketList, flightId);
            break;
        case 4:
            printf("Enter current time hours and minutes\n");
            scanf("%d %d", &currTime.hrs, &currTime.min);
            if(currTime.hrs == 0 && currTime.min == 0){
                printf("!! Original Time Interval!!\n\n");
            }
            else{
                reArrangeBucket(&bucketList, currTime);
                Print(bucketList);
            }
            break;
        case 5:
            printf("..Happy Hacking..\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }

    return 0;
}