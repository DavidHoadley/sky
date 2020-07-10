/*==============================================================================
 * rdwrite.c - output and formatting routines
 *
 * Author:  David Hoadley
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * Description: (see output.h)
 * 
 * Character set: UTF-8. Non-ASCII characters appear in this file.
 *==============================================================================
 */

/* ANSI includes etc. */
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                     /* for memcpy() */

/* Local and project includes */
#include "rdwrite.h"

#include "astron.h"
#include "asttime.h"
#include "general.h"

/*
 * Local #defines and typedefs
 */
DEFINE_THIS_FILE;                       // For use by REQUIRE() - assertions.


/*
 * Prototypes for local functions (not called from other modules)
 */

/*
 * Global variables accessible by other modules
 */


/*
 * Local variables (not accessed by other modules)
 */
LOCAL const char digits[] = "0123456789";
LOCAL const char cDegree[] = "°";
LOCAL const char cMinute[] = "′";
LOCAL const char cSecond[] = "″";

LOCAL const char decimalChar = '.';         // use ',' if you prefer.
LOCAL const char fieldSeparator = ',';      

/*
 *==============================================================================
 *
 * Implementation
 *
 *==============================================================================
 *
 * Global functions callable by other modules
 *
 *------------------------------------------------------------------------------
 */
GLOBAL char *fmtDms(char destStr[],
                    int destStrLen,
                    double angle_rad,
                    int decimals)
/*! Routine to take an angle in radian and return a string of the form
        [±]DDD°MM′SS.sss″
    correctly rounding according to the number of decimal places to be shown.
    The angle is assumed to be within the range (-2*Pi, 2*Pi). Angles which
    round to ±360° will be written out as 0°.
 \returns                Pointer to \a destStr
 \param[out] destStr     Destination character string
 \param[in]  destStrLen  Length of destination string
 \param[in]  angle_rad   The angle to be written out (radian).
                           Valid range:(-2*Pi, 2*Pi); larger numbers may well be
                           written OK, but there is a risk of overflowing an
                           intermediate variable. No error will be detected,
                           just a wrong answer will be written.
 \param[in]  decimals    Number of digits after the decimal point to display.
                           Valid range: [0,9] (or [0,3] if long int is only a
                           32-bit number); numbers outside this range will be
                           clamped to this range.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    const int  minLen = 8 + (sizeof(cDegree) - 1) + (sizeof(cMinute) - 1)
                          + (sizeof(cSecond) - 1);

    long int angle_asxd;    // angle_rad, converted to arcseconds x 10^decimals
    int      reqLen;        // required length of string to fit the output
    int      i, j;
    int      roundup;
    ldiv_t   q;

    if (sizeof(angle_asxd) > 4) {
        // Size of angle_asxd is not an issue for us.
        if (decimals > 9) { decimals = 9; }
    } else {
        // angle_asxd is only a 32-bit variable. Make sure we don't overflow it.
        if (decimals > 3) { decimals = 3; }
    }

    // How many chars will we need to write the angle, including decimal point?
    reqLen = minLen + decimals + (decimals > 0 ? 1 : 0);
    // Make sure caller has supplied a big enough string to hold all the digits
    // that they have requested
    REQUIRE(destStrLen > reqLen);

    roundup = 1;
    for (i = 0; i < decimals; i++) {
        roundup *= 10;
    }
    angle_asxd = lround(radToDeg(angle_rad) * 3600.0 * roundup);
    if (angle_rad < 0) {
        angle_asxd = -angle_asxd;
    }


    i = reqLen;
    destStr[i] = '\0';
    // Insert "″"
    i -= sizeof(cSecond) - 1;
    memcpy(&destStr[i], cSecond, sizeof(cSecond) - 1);
    // Arcseconds and any fraction
    q = ldiv(angle_asxd, 10);
    destStr[--i] = digits[q.rem];
    for (j = decimals; j > 0; j--) {
        if (j == 1) {
            destStr[--i] = decimalChar;
        }
        q = ldiv(q.quot, 10);
        destStr[--i] = digits[q.rem];
    }
    q = ldiv(q.quot, 6);
    destStr[--i] = digits[q.rem];

    // Insert "′"
    i -= sizeof(cMinute) - 1;
    memcpy(&destStr[i], cMinute, sizeof(cMinute) - 1);
    // Arcminutes
    q = ldiv(q.quot, 10);
    destStr[--i] = digits[q.rem];
    q = ldiv(q.quot, 6);
    destStr[--i] = digits[q.rem];

    // Insert "°"
    i -= sizeof(cDegree) - 1;
    memcpy(&destStr[i], cDegree, sizeof(cDegree) - 1);
    // Degrees
    if (q.quot == 360) {
        q.quot = 0;
    }

    for (j = 0; (j < 1) || (q.quot != 0); j++) {
        q = ldiv(q.quot, 10);
        destStr[--i] = digits[q.rem];        
    }
    destStr[--i] = (angle_rad < 0.0) ? '-' : '+';
    while (i > 0) {
        destStr[--i] = ' ';
    }
    
    return destStr;
}



GLOBAL char *fmtHms(char destStr[],
                    int destStrLen,
                    double angle_rad,
                    int decimals)
/*! Routine to take an angle in radian and return a string in hours, minutes and
    seconds form - "±HH:MM:SS.sss" - 
    correctly rounding according to the number of decimal places to be shown.
    The angle is assumed to be within the range (-2*Pi, 2*Pi). Angles which
    round to ±24:00:00 will be written out as 0:00:00.
 \returns                Pointer to \a destStr
 \param[out] destStr     Destination character string
 \param[in]  destStrLen  Length of destination string
 \param[in]  angle_rad   The angle to be written out (radian).
                         Valid range:(-2*Pi, 2*Pi); larger numbers may well be
                         written OK, but there is a risk of overflowing an
                         intermediate variable. No error will be detected,
                         just a wrong answer will be written.
 \param[in]  decimals    Number of digits after the decimal point to display.
                         Valid range: [0,9] (or [0,3] if long int is only a
                         32-bit number); numbers outside this range will be
                         clamped to this range.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    long int angle_sxd;     // angle_rad, converted to seconds x 10^decimals
    int      i, j;
    int      reqLen;        // required length of string to fit the output
    int      roundup;
    ldiv_t   q;

    if (sizeof(angle_sxd) > 4) {
        // Size of angle_sxd is not an issue for us.
        if (decimals > 9) { decimals = 9; }
    } else {
        // angle_sxd is only a 32-bit variable. Make sure we don't overflow it.
        if (decimals > 4) { decimals = 4; }
    }

    // How many chars will we need to write the angle, including decimal point?
    reqLen = 9 + decimals + (decimals > 0 ? 1 : 0);
    // Make sure caller has supplied a big enough string to hold all the digits
    // that they have requested
    REQUIRE(destStrLen > reqLen);


    roundup = 1;
    for (i = 0; i < decimals; i++) {
        roundup *= 10;
    }
    angle_sxd = lround(radToHrs(angle_rad) * 3600.0 * roundup);
    if (angle_rad < 0) {
        angle_sxd = -angle_sxd;
    }

    i = reqLen;
    destStr[i] = '\0';
    // Seconds and any fraction
    q = ldiv(angle_sxd, 10);
    destStr[--i] = digits[q.rem];
    for (j = decimals; j > 0; j--) {
        if (j == 1) {
            destStr[--i] = decimalChar;
        }
        q = ldiv(q.quot, 10);
        destStr[--i] = digits[q.rem];
    }
    q = ldiv(q.quot, 6);
    destStr[--i] = digits[q.rem];

    destStr[--i] = ':';
    // Minutes
    q = ldiv(q.quot, 10);
    destStr[--i] = digits[q.rem];
    q = ldiv(q.quot, 6);
    destStr[--i] = digits[q.rem];

    destStr[--i] = ':';
    // Hours
    if (q.quot == 24) {
        q.quot = 0;
    }

    for (j = 0; j < 2; j++) {
        q = ldiv(q.quot, 10);
        destStr[--i] = digits[q.rem];        
    }

    destStr[--i] = (angle_rad < 0.0) ? '-' : '+';
    
    ENSURE(i == 0);     // If not, we have stuffed up filling the string

    return destStr;
}



GLOBAL double readSexagesimal(const char angleStr[],
                              int strSize,
                              const char **endPtr,
                              int *error)
/*! Convert a string containing an angle (or a time) to the angle's value. The
 *  field containing the angle consists of one, two or three subfields. The
 *  location of the decimal point (if any) is what determines how many subfields
 *  are present. The following are all examples of valid angles (or times):
 *      - 21.625 or +21.625°
 *      - -21 37.5 or 21:37.5 or -21°37.5′ or 21h37.5m
 *      - +21 37 30 or 21:37:30.0 or -21°37′30.0″

    If the string contains degrees, minutes and seconds, the result will be in
    degrees. If the string contains hours, minutes and seconds, the result will
    be in hours.

    The routine will accept almost anything appearing between the numbers,
    such as °, ′, & ″; or d, ' & "; or h, m, & s; or colons or spaces or tabs.
    These are ignored and are not checked for whether they make sense or not.
 \returns    The angle or time
 \param[in]  angleStr  The string of text containing the angle
 \param[in]  strSize   The size of the array \a angleStr (i.e. the maximum
                       possible length of the string. Fewer characters may
                       actually be in use.)
 \param[out] endPtr    A pointer to the end of that part of \a angleStr that was
                       read to obtain the number. This may be pointing to some
                       white space between the number just decoded and further
                       data, or it may be the end of the string. A NULL
                       pointer may be passed to this parameter if you do not
                       need this value.
 \param[out] error     What errors were detected during decoding?
                       This is set to NO_ANGLE (-1) if no value was found in the
                       string.
                       It is set to INVALID_ANGLE (-2) if the minutes or
                       seconds fields are outside the range [0.0, 60.0).
                       A NULL pointer may be passed to this parameter if you
                       do not care about the conversion status.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    const char  *ch;                // Step through the string
    bool        isNegative = false; // Have we encountered a -ve sign?
    bool        pointFound = false; // Have we encountered a decimal point?
    bool        done = false;       // Have we finished decoding the string?
    double      divideBy;
    double      frac;               // value of digits that follow decimal point
    int         status;             // error code
    // Hold the results of decoding in separate array members for degrees/hours,
    // minutes and seconds. Put an invalid value into one of them, so that we
    // can later detect the case when angleStr contains no decodable value.
    double      results[4] = {0.0, -1.0, 0.0, 0.0};
    double      *res = results;     // point to relevant member of results[]

    enum {
        stStarting,     stDegHr,    stSeparator1,   stMins,
        stSeparator2,   stSecs,     stEnding
    } decodeState = stStarting;     // state variable - stage of decoding

    for (ch = angleStr; ch < (angleStr + strSize) && *ch != '\0'; ch++) {
        switch (decodeState) {
        case stStarting:
            if (*ch == '-') { isNegative = true; }
            NOBREAK;
        case stSeparator1:
        case stSeparator2:
            if (isdigit(*ch)) {
                *(++res) = (double)(*ch - '0');
                decodeState++;
            }
            break;

        case stDegHr:
        case stMins:
        case stSecs:
            if (*ch == decimalChar) {
                pointFound = true;
                divideBy = 1.0;

            } else if (isdigit(*ch)) {
                if (pointFound) {
                    divideBy *= 10.0;
                    frac = (double)(*ch - '0') / divideBy;
                    *res += frac;
                } else {
                    *res = (10.0 * *res) + (double)(*ch - '0');
                }

            } else {
                // Not a digit or point, so we have reached the end of the field
                if ((pointFound) || (decodeState == stSecs)) {
                    // This must be the last field we will decode
                    if (isblank(*ch)) {
                        done = true;            // exit now
                    } else if (*ch == fieldSeparator) {
                        ch++;                   // if a separator, move to char
                        done = true;            //  after the separator and exit
                    } else {
                        decodeState = stEnding; // skip non-blanks, then exit
                    }
                } else {
                    decodeState++;
                }
            }
            break;

        case stEnding:
            // Skip over any trailing non-blank chars.
            // Stop when we find white space
            if (isblank(*ch)) {
                done = true;
            }
            break;

        default:
            break;
        }
        
        if (done) {
            // exit the loop without incrementing our character pointer ch
            break;
        }
    }
    status = 0;
    if (results[1] < 0.0) {
        // There was no number in angleStr. Return zero and an error code.
        results[1] = 0.0;
        status = NO_ANGLE;
    }
    if (results[3] >= 60.0) { status = INVALID_ANGLE; }
    if (results[2] >= 60.0) { status = INVALID_ANGLE; }
    if (error != NULL) { *error = status; }
    if (endPtr != NULL) { *endPtr = ch; }

    results[0] = results[1] + results[2] / 60.0 + results[3] / 3600.0;
    if (isNegative) { results[0] = -results[0]; }
    return results[0];
}



GLOBAL void printMatrix(const V3D_Matrix *m)
/*!  Write out the contents of a V3D_Matrix
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    printf("%12.9f, %12.9f, %12.9f\n", m->a[0][0], m->a[0][1], m->a[0][2]);
    printf("%12.9f, %12.9f, %12.9f\n", m->a[1][0], m->a[1][1], m->a[1][2]);
    printf("%12.9f, %12.9f, %12.9f\n", m->a[2][0], m->a[2][1], m->a[2][2]);
}



GLOBAL void printVector(const V3D_Vector *v)
/*!  Write out the contents of a V3D_Vector
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    printf("x = %12.9f, y = %12.9f, z = %12.9f\n", v->a[0], v->a[1], v->a[2]);
}



#ifdef INCLUDE_MJD_ROUTINES
GLOBAL void printMjd(double mjd)
/*!  Write out a Modified Julian Date as a calendar date and time.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    int y, m, d, h, mins;
    double s;

    asttime_mjdToCal(mjd, &y, &m, &d, &h, &mins, &s);
//    printf("%04d-%02d-%02d %02d:%02d:%05.2f", y, m, d, h, mins, s);
    printf("%04d-%02d-%02d %02d:%02d:%08.5f", y, m, d, h, mins, s);
}
#endif



GLOBAL void printJ2kd(double j2kd)
/*!  Write out a J2KD as a calendar date and time. The date and time are written
 *   in ISO format, separated by a space. No newline is written at the end.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    int y, m, d, h, mins;
    double s;

    asttime_j2kdToCal(j2kd, &y, &m, &d, &h, &mins, &s);
//    printf("%04d-%02d-%02d %02d:%02d:%05.2f", y, m, d, h, mins, s);
    printf("%04d-%02d-%02d %02d:%02d:%08.5f", y, m, d, h, mins, s);
}

/*
 *------------------------------------------------------------------------------
 *
 * Local functions (not called from other modules)
 *
 *------------------------------------------------------------------------------
 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


