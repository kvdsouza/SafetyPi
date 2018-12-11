#include "strings.h"


/*
check decimal

*/
int checkDecimal(const char *str) {
  if (*str == 48 || *str == 49 || *str == 50 ||
    *str == 51 || *str == 52 || *str == 53 || *str == 54 || *str == 55 ||
    *str == 56) {
      return 1;
    }
      return 0;
}

int checkUpperHex(const char *str) {
  if (*str == 65 || *str == 66 || *str == 67 ||
          *str == 68 || *str == 69 || *str == 70) {
            return 1;
          } return 0;
}

int checkLowerHex(const char *str) {
  if (*str == 97 || *str == 98 || *str == 99 ||
          *str == 100 || *str == 101 || *str == 102) {
            return 1;
          } return 0;
}

/*
 * Write `n` bytes of value `c` (converted to an unsigned char) to the memory
 * area pointed to by `s`.
 *
 * Return its first argument: `s`.
 */
void *memset(void *s, int c, size_t n)
{
    unsigned char *newS = (unsigned char *)s;
    unsigned char newC = (unsigned char) c; //
    for (int i = 0; i < (unsigned int)n; i++) {
        newS[i] = newC;
    }
    //unsigned int contents = *(int *)(s);
    return s;
    // need to figure out edge case when this is
    // bug exists on the 0xefefabab part.
    // need to fix and test exhaustively.
}

/*
 * Copy `n` bytes from the memory area `src` to the memory area `dst`. If `dst`
 * and `src` overlap, behavior is undefined.
 *
 * Returns its first argument: `dst`.
 */
void *memcpy(void *dst, const void *src, size_t n)
{
    if ((dst < src) && ((dst + n) > src)) {
      return NULL;
    }
    if ((src < dst) && ((src + n) > dst)) {
      return NULL;
    }
    if ((src == dst) && ((src + n) == dst)) {
      return NULL;
    }
  // if ... return null;
  // if an overlap
    unsigned char *chDst = (unsigned char *)dst;
    unsigned char *chSrc = (unsigned char *)src;
    for (int i = 0; i < (unsigned int)n; i++) {
      chDst[i] = chSrc[i];
    }
    return dst;
}

/*
 * Find the length of a string `s`, returned as an `int`.
 *
 * Returns the number of characters that precede the null-terminator.
 */
int strlen(const char *s)
{
    /* Implementation a gift to you from lab3 */
    int i;
    for (i = 0; s[i] != '\0'; i++) ;
    return i;
}

// copies string from src to dst.
char* strcpy(char *dst, const char *src) {
    //char *ptr = dst;
    for (int i = 0; i < strlen(src); i++) {
      dst[i] = src[i];
    }
    return dst;
}


/*
 * Lexicographically compare the null-terminated strings `s1` and `s2`.
 *
 * Returns an integer value greater than, equal to, or less than 0, according
 * as the string `s1` is greater than, equal to, or less than the string `s2`.
 * Comparison is done as unsigned characters.
 */
int strcmp(const char *s1, const char *s2)
{
    //int s1Length = strlen(s1);
    //int s2Length = strlen(s2);
    char c1 = *s1;
    char c2 = *s2;
    while (c1 == c2) {
      if (c1 == '\0') return c1 - c2; // at the end. doesn't accurately calibrate other stuff (i.e. if c2 is longer)?
      c1 = *s1++; // increment!
      c2 = *s2++; // increment!
    }

    //for (int i = 0; )
    // run through each character and check ASCII.
    // if they're equal do this - otherwise add/subract stuff.
    /* TODO: Your code here */
    return c1 - c2;
}

/*
 * Size-bounded string concatenation. Append the null-terminated string `src`
 * to the end of `dst`. Appends at most `maxsize - strlen(dst) - 1` bytes, and
 * null-terminates the result.
 *
 * Returns the initial length of `dst` plus the length of `src`.
 */
int strlcat(char *dst, const char *src, int maxsize)
{
    int dstLength = strlen(dst); // dst string length.
    int srcLength = strlen(src); // src string length.
    int maxAppendLength = maxsize - strlen(dst) - 1; // maxAppendLength from strings.h
    dst = dst + dstLength; // now point dst to null terminator.

    if (srcLength > maxAppendLength) {  // in this case we only add maxAppendLength bytes.
      for (int i = 0; i < maxAppendLength; i++) {
        *dst = *src; // concatenate first character.
        dst++;  // move dst pointer
        src++;  // move src pointer
      }
       // add null terminator.
    } else { // in this case we only add srcLength bytes
      for (int i = 0; i < srcLength; i++) {
        *dst = *src;
        dst++;
        src++;
      }
       // same idea: add null terminator.
    }
    *dst = '\0';
    // extend dst.
    // if equal
    // if otherwise
    // if blank
    //for (i = 0; s[i] != '\0'; i++) {};
    // add src to dst. go to the end of dst and then write src.
    // make sure you only go to 'maxsize - strlen(dst) -1' bytes.
    /* TODO: Your code here */
    return dstLength + srcLength;
}

/*
 * Convert the digit characters in `str` to corresponding unsigned integer
 * value. If `str` begins with the prefix "0x", the characters of `str`
 * will be interpreted as hexadecimal digits (base 16); otherwise characters
 * are interpreted as decimal digits (base 10). No other bases are supported.
 * The conversion processes all characters of `str`, stopping early
 * if it encounters a character that is not a valid digit in base.
 * If `endptr` is not NULL, *endptr is assigned the address of the character
 * in `str` where conversion stopped. This is either the address of the
 * first invalid character in `str` or the address of the terminating null
 * char in `str` if all characters are valid digits.
 *
 * Returns the result of the conversion as an unsigned integer. Returns 0
 * if the first character of `str` is not a valid digit.
 */
unsigned int strtonum(const char *str, const char **endptr)
{
    // connect ascii to other stuff.
    // if endptr not NULL
    unsigned int sum = 0;
    //int strLength = strlen(str);
    // check on this shit.
    // if (endptr == NULL) {
    //   endptr = NULL;
    // }
    //char ch = '';

    // ASCII Table:
    // 0 to 9 is 48 to 57
    // A to F is 65 to 70
    // a to f is 97 to 102
// check hexadecimal case.
    if (*str == 48 && (*(str++) == 88 || *str == 120)) { // right now an error is that 029 is going to this.
      str++;
       //hexadecimal for uppercase and for lowercase.

      while ((checkDecimal(str) + checkUpperHex(str) + checkLowerHex(str)) > 0) {
      // while (*str == 48 || *str == 49 || *str == 50 ||
      //   *str == 51 || *str == 52 || *str == 53 || *str == 54 || *str == 55 ||
      //   *str == 56 || *str == 65 || *str == 66 || *str == 67 || *str == 68
      //   || *str == 69 || *str == 70 || *str == 97 || *str == 98 || *str == 99
      //   || *str == 100 || *str == 101 || *str == 102) {
          // 0-9
          int digit = 0;
          char ch = *str;
          if (checkDecimal(str) > 0) {
          // if (ch == 48 || ch == 49 || ch == 50 ||
          //   ch == 51 || ch == 52 || ch == 53 || ch == 54 || ch == 55 ||
          //   ch == 56)
              digit = ch - '0';  //  48-48
          // A-F
          } else if (checkUpperHex(str) > 0) {
          // else if (ch == 65 || ch == 66 || ch == 67 ||
          //         ch == 68 || ch == 69 || ch == 70) {
              digit = ch - '7'; // 65-55
          // a-f
          } else if (checkLowerHex(str) > 0) {
          // else if (ch == 97 || ch == 98 || ch == 99 ||
          //         ch == 100 || ch == 101 || ch == 102) {
              digit = ch - 'W'; //97-87
          }
          sum = (sum * 16) + digit;
          str++;
        }
    } else {
      // decimal case.
      //while (checkDecimal(str) > 0) {
      while (*str > 47 && *str < 58) {
        // *str == 48 || *str == 49 || *str == 50 ||
        // *str == 51 || *str == 52 || *str == 53 || *str == 54 || *str == 55 ||
        // *str == 56 || *str == 57) {
          // you could check for scope here.
          char ch = *str;
          int digit = ch - '0';
          sum = (sum * 10) + digit;
          str++;
      }
    }
    if (endptr != NULL) {
      //str--; // to send it back
      *endptr = str;
    }

    // place endptr in correct spot.

    /*
    char * input
    Decimal case (need to do Hexadecimal later.)
    check for validity.
    while valid (i.e. when 0 to 9) {
    get 1st char and check if in bounds (check scope).
    if so, subtract '0' or 'a' or 'A' depending on situation.
    store in an int
    int first = char.
    sum = sum * 10 + first;
    'Ascii value 5' and turn it into an 5
    Casting.
    inp++
  }
    */

    //for (int i = 0; i < strLength; i++) {

      // read digit.

    //}
    // process: read digits.

    // Important details: needs to process all characters of 'str'
    // stops early and return number of digits.



    // Case 1: Hexadecimal case.

    // Case 2: Base 10 Case.
    // this is supposed to be tough.
    return sum;
}
