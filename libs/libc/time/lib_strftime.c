/****************************************************************************
 * libs/libc/time/lib_strftime.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <sys/types.h>

#include <stdio.h>
#include <time.h>
#include <debug.h>

#include <nuttx/time.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Public Constant Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const char * const g_abbrev_wdayname[7] =
{
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static const char * const g_wdayname[7] =
{
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
  "Saturday"
};

static const char * const g_abbrev_monthname[12] =
{
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static const char * const g_monthname[12] =
{
  "January", "February", "March",     "April",   "May",      "June",
  "July",    "August",   "September", "October", "November", "December"
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  strftime
 *
 * Description:
 *   The  strftime()  function  formats the broken-down time tm according to
 *   the format specification format and places the result in the  character
 *   array s of size max.
 *
 *   Ordinary characters placed in the format string are copied to s without
 *   conversion.  Conversion specifications are introduced by a '%'  charac-
 *   ter,  and  terminated  by  a  conversion  specifier  character, and are
 *   replaced in s as follows:
 *
 *   %a     The abbreviated weekday name according to the current locale.
 *   %A     The full weekday name according to the current locale.
 *   %b     The abbreviated month name according to the current locale.
 *   %B     The full month name according to the current locale.
 *   %C     The century number (year/100) as a 2-digit integer. (SU)
 *   %d     The day of the month as a decimal number (range 01 to 31).
 *   %e     Like %d, the day of the month as a decimal number, but a leading
 *          zero is replaced by a space.
 *   %h     Equivalent to %b.  (SU)
 *   %H     The hour as a decimal number using a 24-hour clock
 *          (range 00 to 23).
 *   %I     The  hour as a decimal number using a 12-hour clock
 *          (range 01 to 12).
 *   %j     The day of the year as a decimal number (range 001 to 366).
 *   %k     The hour (24-hour clock) as a decimal number (range  0  to  23);
 *          single digits are preceded by a blank.  (See also %H.)  (TZ)
 *   %l     The  hour  (12-hour  clock) as a decimal number (range 1 to 12);
 *          single digits are preceded by a blank.  (See also %I.)  (TZ)
 *   %m     The month as a decimal number (range 01 to 12).
 *   %M     The minute as a decimal number (range 00 to 59).
 *   %n     A newline character. (SU)
 *   %p     Either "AM" or "PM" according to the given time  value, or the
 *          corresponding  strings  for the current locale.  Noon is treated
 *          as "PM" and midnight as "AM".
 *   %P     Like %p but in lowercase: "am" or "pm" or a corresponding string
 *          for the current locale. (GNU)
 *   %s     The number of seconds since the Epoch, that is, since 1970-01-01
 *          00:00:00 UTC. (TZ)
 *   %S     The second as a decimal number (range 00 to 60).  (The range is
 *          up to 60 to allow for occasional leap seconds.)
 *   %t     A tab character. (SU)
 *   %w     The weekday as a decimal number (range 0 to 6).
 *   %y     The year as a decimal number without a century (range 00 to 99).
 *   %Y     The year as a decimal number including the century.
 *   %%     A literal '%' character.
 *
 * Returned Value:
 *   The strftime() function returns the number of characters placed in  the
 *   array s, not including the terminating null byte, provided the string,
 *   including the terminating null byte, fits.  Otherwise,  it returns 0,
 *   and the contents of the array is undefined.
 *
 ****************************************************************************/

size_t strftime(FAR char *s, size_t max, FAR const char *format,
                FAR const struct tm *tm)
{
  FAR const char *str;
  FAR char       *dest   = s;
  int             chleft = max;
  int             value;
  int             len;

  while (*format && chleft > 0)
    {
      /* Just copy regular characters */

      if (*format != '%')
        {
           *dest++ = *format++;
           chleft--;
           continue;
        }

      /* Handle the format character */

       format++;
       len   = 0;

       switch (*format++)
         {
           /* %a: A three-letter abbreviation for the day of the week. */

           case 'a':
             {
               if (tm->tm_wday < 7)
                 {
                   str = g_abbrev_wdayname[tm->tm_wday];
                   len = snprintf(dest, chleft, "%s", str);
                 }
             }
             break;

           /* %A: The full name for the day of the week. */

           case 'A':
             {
               if (tm->tm_wday < 7)
                 {
                   str = g_wdayname[tm->tm_wday];
                   len = snprintf(dest, chleft, "%s", str);
                 }
             }
             break;

           /* %h: Equivalent to %b */

           case 'h':

           /* %b: The abbreviated month name according to the current
            * locale.
            */

           case 'b':
             {
               if (tm->tm_mon < 12)
                 {
                   str = g_abbrev_monthname[tm->tm_mon];
                   len = snprintf(dest, chleft, "%s", str);
                 }
             }
             break;

           /* %B: The full month name according to the current locale. */

           case 'B':
             {
               if (tm->tm_mon < 12)
                 {
                   str = g_monthname[tm->tm_mon];
                   len = snprintf(dest, chleft, "%s", str);
                 }
             }
             break;

           /* %C: The century number (year/100) as a 2-digit integer. */

           case 'C':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_year / 100);
             }
             break;

           /* %d: The day of the month as a decimal number
            * (range 01 to 31).
            */

           case 'd':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_mday);
             }
             break;

           /* %e: Like %d, the day of the month as a decimal number, but
            * a leading zero is replaced by a space.
            */

           case 'e':
             {
               len = snprintf(dest, chleft, "%2d", tm->tm_mday);
             }
             break;

           /* %H: The hour as a decimal number using a 24-hour clock
            * (range 00  to 23).
            */

           case 'H':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_hour);
             }
             break;

           /* %I: The  hour as a decimal number using a 12-hour clock
            * (range 01 to 12).
            */

           case 'I':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_hour % 12);
             }
             break;

           /* %j: The day of the year as a decimal number
            * (range 001 to 366).
            */

           case 'j':
             {
               if (tm->tm_mon < 12)
                 {
                   value = clock_daysbeforemonth(tm->tm_mon,
                           clock_isleapyear(tm->tm_year)) + tm->tm_mday;
                   len   = snprintf(dest, chleft, "%03d", value);
                 }
             }
             break;

           /* %k: The hour (24-hour clock) as a decimal number
            * (range  0  to  23);
            * single digits are preceded by a blank.
            */

           case 'k':
             {
               len = snprintf(dest, chleft, "%2d", tm->tm_hour);
             }
             break;

           /* %l: The  hour  (12-hour  clock) as a decimal number
            * (range 1 to 12);
            * single digits are preceded by a blank.
            */

           case 'l':
             {
               len = snprintf(dest, chleft, "%2d", tm->tm_hour % 12);
             }
             break;

           /* %m: The month as a decimal number (range 01 to 12). */

           case 'm':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_mon + 1);
             }
             break;

           /* %M: The minute as a decimal number (range 00 to 59). */

           case 'M':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_min);
             }
             break;

           /* %n: A newline character. */

           case 'n':
             {
               *dest = '\n';
               len   = 1;
             }
             break;

           /* %p: Either "AM" or "PM" according to the given time  value. */

           case 'p':
             {
               if (tm->tm_hour >= 12)
                 {
                   str = "PM";
                 }
               else
                 {
                   str = "AM";
                 }

               len = snprintf(dest, chleft, "%s", str);
             }
             break;

           /* %P: Like %p but in lowercase: "am" or "pm" */

           case 'P':
             {
               if (tm->tm_hour >= 12)
                 {
                   str = "pm";
                 }
               else
                 {
                   str = "am";
                 }

               len = snprintf(dest, chleft, "%s", str);
             }
             break;

            /* %R: Shortcut for %H:%M. */

           case 'R':
             {
               len = snprintf(dest, chleft, "%02d:%02d",
                              tm->tm_hour, tm->tm_min);
             }
             break;

           /* %s: The number of seconds since the Epoch, that is,
            * since 1970-01-01 00:00:00 UTC.
            * Hmmm... mktime argume is not 'const'.
            */

           case 's':
             {
               len = snprintf(dest, chleft, "%ju",
                              (uintmax_t)mktime((FAR struct tm *)tm));
             }
             break;

           /* %S: The second as a decimal number (range 00 to 60).
            * (The range is up to 60 to allow for occasional leap seconds.)
            */

           case 'S':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_sec);
             }
             break;

           /* %t: A tab character. */

           case 't':
             {
               *dest = '\t';
               len   = 1;
             }
             break;

           /* %T: Shortcut for %H:%M:%S. */

           case 'T':
             {
               len = snprintf(dest, chleft, "%02d:%02d:%02d",
                              tm->tm_hour, tm->tm_min, tm->tm_sec);
             }
             break;

           /* %w: The weekday as a decimal number (range 0 to 6). */

           case 'w':
             {
               len = snprintf(dest, chleft, "%d", tm->tm_wday);
             }
             break;

           /* %y: The year as a decimal number without a century
            * (range 00 to 99).
            */

           case 'y':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_year % 100);
             }
             break;

           /* %Y: The year as a decimal number including the century. */

           case 'Y':
             {
               len = snprintf(dest, chleft, "%04d", tm->tm_year + 1900);
             }
             break;

           /* %%:  A literal '%' character. */

           case '%':
             {
               *dest = '%';
               len   = 1;
             }
             break;
        }

      /* Update counts and pointers */

      dest   += len;
      chleft -= len;
    }

  /* We get here because either we have reached the end of the format string
   * or because there is no more space in the user-provided buffer and the
   * resulting string has been truncated.
   *
   * Is there space remaining in the user-provided buffer for the NUL
   * terminator?
   */

  if (chleft > 0)
    {
      /* Yes, append terminating NUL byte */

      *dest = '\0';

      /* And return the number of bytes in the resulting string (excluding
       * the NUL terminator).
       */

      return max - chleft;
    }

  /* The string was truncated and/or not properly terminated.  Return
   * zero.
   */

  return 0;
}
