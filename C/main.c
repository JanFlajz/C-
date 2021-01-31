#ifndef __PROGTEST__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>

typedef struct {
    char *m_Scope;
    char *m_Text;
} TMEASURE;

typedef struct TMeasureList {
    char m_From[12];
    char m_To[12];
    char *m_Text;
    struct TMeasureList *m_Next;
} TMEASURELIST;
#endif /* __PROGTEST__ */

//Struct to store parts of measure in more
typedef struct lockdownData {

    char dFrom[12];
    char dTo[12];
    char *mPlace;
    char *mText;


} TLOCKDOWNDATA;
//says if Scope is involved in array
int isInvolved(int *pos, TLOCKDOWNDATA structure, TMEASURE *array) {

    if(!array)
        return 0;

    for (int i = 0; array[i].m_Scope && array[i].m_Text; i++) {
        if (!strcasecmp(array[i].m_Scope, structure.mPlace)) {

            *pos = i;
            return 1;
        }

    }
    return 0;
}
//Converts text representation of month to number
int determineMonth(char *str) {

    if (!strcasecmp(str, "Jan") || !strcasecmp(str, "January"))
        return 1;
    else if (!strcasecmp(str, "feb") || !strcasecmp(str, "February"))
        return 2;
    else if (!strcasecmp(str, "mar") || !strcasecmp(str, "March"))
        return 3;
    else if (!strcasecmp(str, "Apr") || !strcasecmp(str, "April"))
        return 4;
    else if (!strcasecmp(str, "May"))
        return 5;
    else if (!strcasecmp(str, "Jun") || !strcasecmp(str, "June"))
        return 6;
    else if (!strcasecmp(str, "Jul") || !strcasecmp(str, "July"))
        return 7;
    else if (!strcasecmp(str, "Aug") || !strcasecmp(str, "August"))
        return 8;
    else if (!strcasecmp(str, "Sep") || !strcasecmp(str, "september"))
        return 9;
    else if (!strcasecmp(str, "Oct") || !strcasecmp(str, "October"))
        return 10;
    else if (!strcasecmp(str, "Nov") || !strcasecmp(str, "November"))
        return 11;
    else if ((!strcasecmp(str, "dec") || !strcasecmp(str, "December")))
        return 12;
    else
        return 0;

}
//counts non-empty members of array
int numOfMembers(TMEASURE *m) {
    int size = 0;

    while (m[size].m_Scope && m[size].m_Text)
        size++;


    return size;

}

// counts the date to one number to compare
int countDate(const char *date) {
    /*Y-m-d = 0
    * d.m.Y = 1
    * m/d/Y = 2
    * m d, Y =3 month in words
    * m d Y = 4 month in words
    */

    char month[10];
    month[0] = '\0';
    int yearNum = 0;
    int monthNum = 0;
    int dayNum = 0;

    if (index(date, '-'))
        sscanf(date, "%d-%d-%d", &yearNum, &monthNum, &dayNum);
    else if (index(date, '.'))
        sscanf(date, "%d.%d.%d", &dayNum, &monthNum, &yearNum);
    else if (index(date, '/'))
        sscanf(date, "%d/%d/%d", &monthNum, &dayNum, &yearNum);
    else if (index(date, ',')) {
        sscanf(date, "%s %d, %d", month, &dayNum, &yearNum);
        monthNum = determineMonth(month);
    } else {
        sscanf(date, "%s %d %d", month, &dayNum, &yearNum);
        monthNum = determineMonth(month);
    }

    return yearNum * 365 + monthNum * 30 + dayNum;
}


TLOCKDOWNDATA createStructure(char *measure, int * lengthPlace, int * lengthText) {

    //allocate new structure members
    TLOCKDOWNDATA structOfData;
    structOfData.dTo[0] = '\0';
    structOfData.dFrom[0] = '\0';

    structOfData.mText = NULL;
    structOfData.mPlace = NULL;
    structOfData.mText = (char *) malloc(100 * sizeof(char));
    structOfData.mPlace = (char *) malloc(20 * sizeof(char));
    structOfData.mText[0] = '\0';
    structOfData.mPlace[0] = '\0';



    char delimiters[4];

    int switcher = 0;
    int i = 0;
    char c;
    int index;


    while (switcher < 4) {

        c = measure[i];
        if (isspace(c)) {
            measure++;
            continue;
        }
        //Determine if text is in quotation marks
        if (c == '\'') {

            delimiters[0] = '\'';
            delimiters[1] = '\'';
            delimiters[2] = '\'';
            delimiters[3] = '\0';
            measure++;
        } else if (c == '\"') {
            delimiters[0] = '\"';
            delimiters[1] = '\"';
            delimiters[2] = '\"';
            delimiters[3] = '\0';

            measure++;
        } else {
            delimiters[0] = '\n';
            delimiters[1] = '\t';
            delimiters[2] = ' ';
            delimiters[3] = '\0';
        }

        index = (int) strcspn(measure, delimiters);

        switch (switcher) {
            case 0:
                strncpy(structOfData.dFrom, measure, index);
                break;
            case 1:
                strncpy(structOfData.dTo, measure, index);
                break;
            case 2:
                strncpy(structOfData.mPlace, measure, index);
                *lengthPlace = index;
                break;
            case 3:
                strncpy(structOfData.mText, measure, index);
                *lengthText = index;
                break;
            default:
                break;

        }


        measure += index + 1;
        switcher++;
    }

    return structOfData;


}


TMEASURE *listOneDay(const char *src,
                     const char *date) {
    /* TODO */
    //Array of structures to return
    int sizeOfArray = 0;
    int allocatedMeasures = 0;
    TMEASURE *arrayOfMeasures = NULL;



    // one part of measure string
    int lengthOfSrc = (int) strlen(src);
    char *measure = (char *) malloc(lengthOfSrc * sizeof(char));

    //date translated to number
    int certainDay = countDate(date);
    //dates of start, dates of end, in one number
    int startDate = 0;
    int endDate = 0;

    //neccessary tools to find the end of the measure
    int indexOfDelimiter;
    char delimiter[2] = {'\n', '\t'};
    int positionOfPreviousScope = 0;



    TLOCKDOWNDATA structureOfData;

    int lengthOfPlace = 0;
    int lengthOfText = 0;



    while (1) {

        // index of delimiter
        indexOfDelimiter = (int) strcspn(src, delimiter);

        // copy to measure to its own text
        strncpy(measure, src, indexOfDelimiter +1);

         //create structure with measure data
        structureOfData = createStructure(measure, &lengthOfPlace,&lengthOfText);

        // count date to one number
        startDate = countDate(structureOfData.dFrom);
        endDate = countDate(structureOfData.dTo);

        //measure isc valid at this time
        if (startDate <= certainDay && certainDay <= endDate) {


            if (!isInvolved(&positionOfPreviousScope, structureOfData, arrayOfMeasures)) {

                //allocate longer array of structs
                if (allocatedMeasures >= sizeOfArray) {

                    sizeOfArray = 2 * sizeOfArray + 1;


                    TMEASURE *measureTmp = (TMEASURE *) realloc(arrayOfMeasures, (sizeOfArray * sizeof(TMEASURE)));

                    //Realloc doesnt work
                    if (!measureTmp) {
                        free(arrayOfMeasures);
                        return NULL;
                    }


                    arrayOfMeasures = measureTmp;

                }


                //Allocate new string at new position
                arrayOfMeasures[allocatedMeasures].m_Text = NULL;
                arrayOfMeasures[allocatedMeasures].m_Scope = NULL;
                arrayOfMeasures[allocatedMeasures].m_Scope = (char *) realloc(
                        arrayOfMeasures[allocatedMeasures].m_Scope, 100 * sizeof(char));
                arrayOfMeasures[allocatedMeasures].m_Text = (char *) realloc(
                        arrayOfMeasures[allocatedMeasures].m_Text, 100);


                //Copy strings to array

                strcpy(arrayOfMeasures[allocatedMeasures].m_Scope, structureOfData.mPlace);
                strcpy(arrayOfMeasures[allocatedMeasures].m_Text, structureOfData.mText);
                allocatedMeasures++;

            } else {

                //Scope is involved so I only change the text at this position
                strcpy(arrayOfMeasures[positionOfPreviousScope].m_Text, structureOfData.mText);

            }


        }

        //Empty allocated strings for reuse
        memset(measure, '\0', indexOfDelimiter);
        free(structureOfData.mPlace);
        free(structureOfData.mText);



        if (!strcmp(src, ""))
            break;


        src += ++indexOfDelimiter;

    }

    free(measure);


    //There is no measure at the time so empty array is created
    if(!arrayOfMeasures){
        arrayOfMeasures = (TMEASURE*) malloc(sizeof(TMEASURE));
        arrayOfMeasures[0].m_Text = NULL;
        arrayOfMeasures[0].m_Scope = NULL;
    }

    return arrayOfMeasures;
}


//Free the memory used by array of structures

void freeOneDay(TMEASURE *m) {
    /* TODO */

    int i = 0;

    while (m[i].m_Text && m[i].m_Scope) {

        free(m[i].m_Text);
        m[i].m_Text = NULL;
        free(m[i].m_Scope);
        m[i].m_Scope = NULL;
        i++;
    }


}

TMEASURELIST *listOneScope(const char *src,
                           const char *scope) {
    return NULL; /* TODO */
}

void freeOneScope(TMEASURELIST *m) {
    /* TODO */
}

#ifndef __PROGTEST__


int hasMeasure(const TMEASURE *m,
               const char *scope,
               const char *text) {

    for (int i = 0; m[i].m_Scope && m[i].m_Text; i++) {

        if (!strcasecmp(m[i].m_Scope, scope)
            && !strcasecmp(m[i].m_Text, text))
            return 1;

    }

    return 0;
}

int main(void) {
    TMEASURE *m;
    TMEASURELIST *l;
    const char *str1 =
            "2021-01-01 2021-2-1 FaceMasks Everywhere\t2021-1-5 2021-03-5 FaceMasks AlmostEverywhere\n"
            "2021-1-5 2021-03-5 FaceMasks AlmostEverywhere!\n"
            "2021-01-10 2021-02-05 Lockdown Complete\n"
            "2021-01-02 2021-01-15 lockdown ComlpeteExceptDogwalking\n"
            "2021-1-20 2021-01-20 LOCKDOWN Between9PMand6AM\n";
    //const char * str1 = "";
    //const char *str1 = "2021-01-01 2021-2-1 FaceMasks Everywhere\t2021-1-5 2021-03-5 FaceMasks AlmostEverywhere\n";
    const char *str2 =
            "2021-01-01 2021-7-1 \'Face masks\' \"Wear everywhere\"\t1/5/2021 \'March 5, 2021\' \"Face Masks\" \'Wear almost everywhere\'\n"
            "1/5/2021 \'March 5, 2021\' \"Face Masks\" \'Wear almost everywhere!\'\n"
            "10.1.2021 \'5.2.2021\' Lockdown \"Complete\"\n"
            "\'jan 2 2021\' \"JANUARY 15 2021\" lockdown \'Comlpete except dogwalking\'\n"
            "2021-1-20 2021-01-20 LOCKDOWN \'Between 9PM and 6AM\'\n";
    const char *str3 =
            "2020-05-12 2021-7-1 \'Face masks\' \"Wear everywhere\"\t1/15/2021 \'March 5, 2021\' \"Face Masks\" \'Wear almost everywhere\'\n"
            "2/5/2021 \'Feb 28, 2021\' \"Face Masks\" \'Wear almost everywhere!\'\n"
            "2021-07-02 2021-10-05 \'FAce masks\' \"WEAR everywhere\"\t10.1.2021 \'5.2.2021\' Lockdown \"Complete\"\n"
            "\'Mar 5 2021\' \"June 15 2021\" lockdown \'Comlpete except dogwalking\'\n"
            "2021-05-20 2021-09-20 LOCKDOWN \'Between 9PM and 6AM\'\n";
    m = listOneDay(str1, "2021-01-02");
    assert (m);
    // assert(numOfMembers(m) == 2);
    assert (m[2].m_Scope == NULL && m[2].m_Text == NULL);
    assert (hasMeasure(m, "lockdown", "ComlpeteExceptDogwalking"));
    assert (hasMeasure(m, "FaceMasks", "Everywhere"));
    assert (m[2].m_Scope == NULL && m[2].m_Text == NULL);
    assert(numOfMembers(m) == 2);
    freeOneDay(m);
    assert(numOfMembers(m) == 0);

    m = listOneDay(str1, "2021-2-5");
    assert (m);
    //assert(numOfMembers(m) == 2);
    assert (hasMeasure(m, "Lockdown", "Complete"));
    assert (hasMeasure(m, "FaceMasks", "AlmostEverywhere!"));
    assert (m[2].m_Scope == NULL && m[2].m_Text == NULL);
    freeOneDay(m);
    assert(numOfMembers(m) == 0);
   m = listOneDay(str1, "2021-1-20");
    assert (m);
    assert (hasMeasure(m, "LOCKDOWN", "Between9PMand6AM"));
    assert (hasMeasure(m, "FaceMasks", "AlmostEverywhere!"));
    assert (m[2].m_Scope == NULL && m[2].m_Text == NULL);
    assert(numOfMembers(m) == 2);
    freeOneDay(m);

    assert(numOfMembers(m) == 0);
    m = listOneDay(str1, "2000-1-1");
    assert (m);
    assert (m[0].m_Scope == NULL && m[0].m_Text == NULL);
    freeOneDay(m);
    assert(numOfMembers(m) == 0);

    m = listOneDay(str2, "2.1.2021");
    assert (m);
    assert (hasMeasure(m, "lockdown", "Comlpete except dogwalking"));
    assert (hasMeasure(m, "Face masks", "Wear everywhere"));
    assert (m[2].m_Scope == NULL && m[2].m_Text == NULL);
    freeOneDay(m);
    m = listOneDay(str2, "Feb 5, 2021");
    assert (m);
    assert (hasMeasure(m, "Lockdown", "Complete"));
    assert (hasMeasure(m, "Face Masks", "Wear almost everywhere!"));
    assert (m[2].m_Scope == NULL && m[2].m_Text == NULL);
    freeOneDay(m);

    m = listOneDay(str2, "1/20/2021");
    assert (m);
    assert (hasMeasure(m, "LOCKDOWN", "Between 9PM and 6AM"));
    assert (hasMeasure(m, "Face Masks", "Wear almost everywhere!"));
    assert (m[2].m_Scope == NULL && m[2].m_Text == NULL);
    freeOneDay(m);

    m = listOneDay(str2, "January 1 2000");
    assert (m);
    assert (m[0].m_Scope == NULL && m[0].m_Text == NULL);
    freeOneDay(m);

    //This asserts does nothing, because funtctions are not implemented

    /* l = listOneScope(str2, "lockdown");
     assert (l);
     assert (!strcasecmp(l->m_From, "2021-01-02"));
     assert (!strcasecmp(l->m_To, "2021-01-15"));
     assert (!strcasecmp(l->m_Text, "Comlpete except dogwalking"));
     assert (l->m_Next);
     assert (!strcasecmp(l->m_Next->m_From, "2021-01-16"));
     assert (!strcasecmp(l->m_Next->m_To, "2021-01-19"));
     assert (!strcasecmp(l->m_Next->m_Text, "Complete"));
     assert (l->m_Next->m_Next);
     assert (!strcasecmp(l->m_Next->m_Next->m_From, "2021-01-20"));
     assert (!strcasecmp(l->m_Next->m_Next->m_To, "2021-01-20"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Text, "Between 9PM and 6AM"));
     assert (l->m_Next->m_Next->m_Next);
     assert (!strcasecmp(l->m_Next->m_Next->m_Next->m_From, "2021-01-21"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Next->m_To, "2021-02-05"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Next->m_Text, "Complete"));
     assert (!l->m_Next->m_Next->m_Next->m_Next);
     freeOneScope(l);

     l = listOneScope(str2, "face masks");
     assert (l);
     assert (!strcasecmp(l->m_From, "2021-01-01"));
     assert (!strcasecmp(l->m_To, "2021-01-04"));
     assert (!strcasecmp(l->m_Text, "Wear everywhere"));
     assert (l->m_Next);
     assert (!strcasecmp(l->m_Next->m_From, "2021-01-05"));
     assert (!strcasecmp(l->m_Next->m_To, "2021-03-05"));
     assert (!strcasecmp(l->m_Next->m_Text, "Wear almost everywhere!"));
     assert (l->m_Next->m_Next);
     assert (!strcasecmp(l->m_Next->m_Next->m_From, "2021-03-06"));
     assert (!strcasecmp(l->m_Next->m_Next->m_To, "2021-07-01"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Text, "Wear everywhere"));
     assert (!l->m_Next->m_Next->m_Next);
     freeOneScope(l);

     l = listOneScope(str3, "lockdown");
     assert (l);
     assert (!strcasecmp(l->m_From, "2021-01-10"));
     assert (!strcasecmp(l->m_To, "2021-02-05"));
     assert (!strcasecmp(l->m_Text, "Complete"));
     assert (l->m_Next);
     assert (!strcasecmp(l->m_Next->m_From, "2021-03-05"));
     assert (!strcasecmp(l->m_Next->m_To, "2021-05-19"));
     assert (!strcasecmp(l->m_Next->m_Text, "Comlpete except dogwalking"));
     assert (l->m_Next->m_Next);
     assert (!strcasecmp(l->m_Next->m_Next->m_From, "2021-05-20"));
     assert (!strcasecmp(l->m_Next->m_Next->m_To, "2021-09-20"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Text, "Between 9PM and 6AM"));
     assert (!l->m_Next->m_Next->m_Next);
     freeOneScope(l);

     l = listOneScope(str3, "face masks");
     assert (l);
     assert (!strcasecmp(l->m_From, "2020-05-12"));
     assert (!strcasecmp(l->m_To, "2021-01-14"));
     assert (!strcasecmp(l->m_Text, "Wear everywhere"));
     assert (l->m_Next);
     assert (!strcasecmp(l->m_Next->m_From, "2021-01-15"));
     assert (!strcasecmp(l->m_Next->m_To, "2021-02-04"));
     assert (!strcasecmp(l->m_Next->m_Text, "Wear almost everywhere"));
     assert (l->m_Next->m_Next);
     assert (!strcasecmp(l->m_Next->m_Next->m_From, "2021-02-05"));
     assert (!strcasecmp(l->m_Next->m_Next->m_To, "2021-02-28"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Text, "Wear almost everywhere!"));
     assert (l->m_Next->m_Next->m_Next);
     assert (!strcasecmp(l->m_Next->m_Next->m_Next->m_From, "2021-03-01"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Next->m_To, "2021-03-05"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Next->m_Text, "Wear almost everywhere"));
     assert (l->m_Next->m_Next->m_Next->m_Next);
     assert (!strcasecmp(l->m_Next->m_Next->m_Next->m_Next->m_From, "2021-03-06"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Next->m_Next->m_To, "2021-10-05"));
     assert (!strcasecmp(l->m_Next->m_Next->m_Next->m_Next->m_Text, "Wear everywhere"));
     assert (!l->m_Next->m_Next->m_Next->m_Next->m_Next);
     freeOneScope(l);
 */
    return 0;
}

#endif /* __PROGTEST__ */