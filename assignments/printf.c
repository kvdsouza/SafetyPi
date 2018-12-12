#include "printf.h"
#include "strings.h"
#include <stdarg.h>

#define MAX_OUTPUT_LEN 1024



/*
These helper functions handle the task of converting a number to a string representation,
 for example, converting the number 0x85 to the string "133".

The caller passes the arguments buf and bufsize. buf points to a character
array where the function should write the string. bufsize is
the size of this array. You can assume that bufsize is not negative. Your
functions should never write past the end of the array. If the output cannot
fit, truncate it. Don't forget that you also need space for a null terminator.

The val argument is the integer value to be converted to a string. In the
signed function,
if the value is negative, you will need to include a leading minus sign
in the string.

The base argument controls the base the string is written in. You have to support
 decimal (base 10) and hexadecimal (base 16). When you implement the conversion,
  do not copy/paste to create two nearly identical conversion functions, one
  for each base! You want to minimize repeated code, because if you find a bug
  you'll have to fix it in both copies. Instead, identify how to, with a single
   implementation, flexibly allow for either base. Similarly, although there
   is both a signed and unsigned conversion function, you should also not
   duplicate code between the two. Instead consider how you can call the
   unsigned function from the signed function.

The min_width is the minimum number of characters in the output string. If the
formatted string is less than min_width, pad the beginning of the string with
zeros so it has length min_width. If the formatted string is already at least
as long as min_width, ignore the min_width argument. The minus sign should precede the zero padding and is included in the min_width. You can assume min_width is not negative.

The string your function writes into buf must be a valid, null-terminated C
string.

These functions should return the count of characters written to buf if
there is room to fit them all. If not, it should return the count of
characters that would have been written if there were room. The null terminator
 is not included in the count.

Some invocation examples ('\0' is the null terminator):

signed_to_base(buf, 20, 94305, 10, 0) writes 6 bytes into buf: "94305\0"
and returns 5.
unsigned_to_base(buf, 20, 128, 16, 6) writes 7 bytes into buf: "000080\0"
and returns 6.
signed_to_base(buf, 5, -9999, 10, 6) writes 5 bytes into buf: "-099\0" and
returns 6.
Note about bufsize and memory corruption: You might notice that some of
these parameters overlap in what they specify about the output string. For
example, bufsize is a hard upper limit on how much space is available to store
the output string, but nothing in the calling convention says that min_width
must fit within bufsize. In all cases bufsize wins: not writing past of the
array and corrupting memory is more important than writing out the string
specified by the arguments. If bufsize is too small to fit your output,
even if min_width says you should go past it, you must cut your output off
and make buf[bufsize - 1] a null terminator. Finally, bufsize can be zero:
if so, you should not write anything to the buffer, not even a null terminator.

*/
// collects length of int.
int intLength(int val, int base) {
    int length = 0;
    while(val != 0) {
      val = (val/base);
      length++;
    }
    return length;
}
// needs to deal with hexadecimal and decimal case.
// takes the power
int power(int base,int exponent)
{
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return(result);
}

int unsigned_to_base(char *buf, int bufsize, unsigned int val, int base, int min_width)
{
    char temp_array[1024]; // create a new array to store string values.
    char *ch = temp_array; // initial pointer.
    int returnLength;   // this will be the variable returned.
    int valLength = intLength(val, base); // length of value.

    // figure out returnLength value based on spec.
    if (valLength >= min_width) {
        returnLength = valLength;
    } else {
        returnLength = min_width;
    }

    // base case.
    if (bufsize == 0) {
        return returnLength;
    }

    // mash a bunch of 0's to make 'em happy
    while (valLength < min_width) {
        *ch = '0';
        ch++;
        valLength++;
    }


    int restLength = intLength(val, base);
    for (int i = 0; i < restLength; i++) {
        int digit;
        digit = val / power(base, restLength-1-i);
        if (digit >= 0 && digit <= 9) {
          *ch = digit + '0';
        } else if (digit > 9 && digit < 17) {
          *ch = digit + 'W';
        }
        // need to deal with the times when this is abcdef...
        ch++;
        val = val % power(base, restLength-1-i);
    }
// READ FIRST DIGIT IN ONE'S PLACE AND MULTIPLY BY 10.
// array to store digits // ok so you're reading you need to store in your array right to left.

    //*ch = '\0';

    //
    char *arrayPtr = temp_array;
    int finalBufLength; // final buffer length of string
    if (bufsize - 1 <= returnLength) {
        finalBufLength = (bufsize - 1);
    } else {
        finalBufLength = returnLength;
    }
    //int bufString = min(bufsize - 1, returnlength);

    for (int i = 0; i < finalBufLength; i++) {
        // buf[i] = temp_array[i]; (potential fix).
        *buf = *arrayPtr;
        buf++;
        arrayPtr++;
    }

    *buf = '\0';

    return returnLength;
}

int signed_to_base(char *buf, int bufsize, int val, int base, int min_width)
{
    int lengthReturn;
    if (val < 0) {
      val = -val;
      lengthReturn = unsigned_to_base(buf, bufsize, val, base, min_width);
      *buf = '-';
    } else {
      lengthReturn = unsigned_to_base(buf, bufsize, val, base, min_width);
    }
    /* TODO: Your code here */
    return lengthReturn;
}

/* These three functions from the printf family are used to produce
 * formmatted output. The functions accept the same kinds of
 * format strings with the same conversions, but differ slightly in
 * how they are called or where they put the output, e.g.: whether
 * the output is written to the Raspberry Pi UART (printf) or stored
 * as a string in the client's buffer (snprintf and vsnsprintf).
 *
 * The format conversions support by this printf module are
 *   %c    for single characters
 *   %s    for strings
 *   %d    for signed decimal integers
 *   %x    for unsigned hexadecimal integers
 *   %p    for pointers (printed as addresses)
 *
 * The %d and %x formats allow an optional field width.
 *
 * Other fancy features of printf (padding with spaces,
 * left versus right justification, precision, etc.) are not supported.
 */
int vsnprintf(char *buf, int bufsize, const char *format, va_list args)
{
  char temp_array[MAX_OUTPUT_LEN];
  char *ch = temp_array;
  int formatLength = strlen(format); // just in case.
  int returnLength; // defined later.
//variadic stuff.
// variable name of the last thing.
// number of %
  va_list argList = args;


//looping
  for (int i = 0; i < formatLength; i++) {
    // insert the ton of if commands here.
    if (*format == '%') {
      format++;
      // now here we insert each of the cases.
      if (*format == 'c') {
          char var = (char)va_arg(argList, int);
          *ch = var;
          ch++;
          format++;
      //strings
      } else if (*format == 's') {
          char *str = (char *)va_arg(argList, int);
          int length = strlen(str);
          for (int i = 0; i < length; i++) {
            *ch = *str; // concatenate first character.
            ch++;  // move dst pointer
            str++;
          }
          format++;
      //decimal
      } else if (*format == 'd') {
          int decimal = va_arg(argList, int);
          char newBuf[1024];
          char *bufPtr = newBuf;

          signed_to_base(bufPtr, 1024, decimal, 10, 0);
          int length = strlen(bufPtr);
          strcpy(ch, bufPtr);
          for (int i = 0; i < length; i++) {
            ch++;
          }
          format++;

      } else if (*format == 'x') {
          unsigned int hex = (unsigned int)va_arg(argList, int);
          char hexBuf[1024];
          char *hexPtr = hexBuf;

          unsigned_to_base(hexPtr, 1024, hex, 16, 0);
          int length = strlen(hexPtr);
          strcpy(ch, hexPtr);
          for (int i = 0; i < length; i++) {
            ch++;
          }
          format++;
// unsigned hexadecimal - unsigned_to_base (base16)
      //pointer
      } else if (*format == 'p') {
          void* read = va_arg(argList, int);
          unsigned int address = (unsigned int)read;
          char pointerBuf[1024];
          char *pointerPtr = pointerBuf;
          unsigned_to_base(pointerPtr, 1024, address, 16, 0);
          int length = strlen(pointerPtr);

          //add '0x'.
          *ch = '0';
          ch++;
          *ch = 'x';
          ch++;
          strcpy(ch, pointerPtr);
          for (int i = 0; i < length; i++) {
            ch++;
          }
          format++;

      // finally do the mash 0's one.
      //} else if (*format == 'dx') {}
      //%d and %x
      } else if (*format == '0') {
          const char *endstr; // endstring value
          //format++; // to go to a non-zero value.
          unsigned int minimumWidth = strtonum(format, &endstr); // put that in
          // for (int i = 0; i < value; i++) {
          //   *ch = 0;
          //   ch++;
          // } don't add yet.

          while(*format != 'd' && *format != 'x') {
            format++;
          }

          if (*format == 'd') {
            int dec = va_arg(argList, int);
            char newBuffer[1024];
            char *bufferPtr = newBuffer;

            signed_to_base(bufferPtr, 1024, dec, 10, minimumWidth);

            strcpy(ch, bufferPtr);
            int length1 = strlen(bufferPtr);
            for (int i = 0; i < length1; i++) {
                ch++;
              }
            format++;

          } else if (*format == 'x') {
              unsigned int hex1 = (unsigned int)va_arg(argList, int);
              char hexBuf1[1024];
              char *hexPtr1 = hexBuf1;

              unsigned_to_base(hexPtr1, 1024, hex1, 16, minimumWidth);
              int length3 = strlen(hexPtr1);
              strcpy(ch, hexPtr1);
              for (int i = 0; i < length3; i++) {
                ch++;
              }
              format++;
          }

      /* code */
      //mash 0's and shit
      } else {
        *ch = *format;
        ch++;
        format++;
      }

    } else {
      // This will be the else case (last resort).
      *ch = *format;
      ch++;
      format++;
    }
  }
// add null terminator.
  *ch = '\0';

// move into buf.
  // first collect distance.
  char *tempPtr = temp_array;
  returnLength = strlen(tempPtr);
  if (bufsize == 0) {
      return returnLength;
  }

  //char *arrayPtr = temp_array;
  int finalBufLength; // final buffer length of string
  if (bufsize - 1 <= returnLength) {
      finalBufLength = (bufsize - 1);
  } else {
      finalBufLength = returnLength;
  }
  //int bufString = min(bufsize - 1, returnlength);
  // add this stuff.
  for (int i = 0; i < finalBufLength; i++) {
      *buf = *tempPtr;
      buf++;
      tempPtr++;
  }

  *buf = '\0';

  return returnLength;
}

/*
 * `buf` is the destination of the formatted string.
 * `bufsize` is the size of `buf`
 * `format` is a character string which contains plain characters (copied to
 * `buf`), character escape sequences (converted and copied to `buf`), and form
 * specifications (cause copying of the next successive argument).
 *
 * Returns the number of characters written if it fits in the buffer, or the
 * number of characters it would have written if there were space.
 */
int snprintf(char *buf, int bufsize, const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    int returnLength = vsnprintf(buf, bufsize, format, argList);
    va_end(argList);
    return returnLength;

}

int printf(const char *format, ...)
{
    char array[MAX_OUTPUT_LEN];
    va_list args;
    va_start(args, format);
    int returnLength = vsnprintf(array, MAX_OUTPUT_LEN, format, args);
    va_end(args);

    int i = 0;
    while (array[i] != '\0') {
      uart_putchar(array[i]);
      i++;
    }
    return returnLength;
}

// Citation - Discussed Concepts with Thakur Kharel, Abla Ghaleb, Schuyler Tilney-Volk,
// TA's, Chris Gregg.
