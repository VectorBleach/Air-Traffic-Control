int compareFlightId(AVLFlightSchedule *a, AVLFlightSchedule *b)
{
    if (a->flightId > b->flightId)
        return 1;
    else if (a->flightId < b->flightId)
        return -1;
    return 0;
}