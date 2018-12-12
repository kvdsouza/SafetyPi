#include "shell.h"
#include "shell_commands.h"
#include "uart.h"
#include "keyboard.h"
#include "strings.h"
#include "malloc.h"
#include "pi.h"

#define LINE_LEN 80

static int (*shell_printf)(const char * format, ...);

static const command_t commands[] = {  // list of commands - wrote a few myself.
    {"help",   "<cmd> prints a list of commands or description of cmd", cmd_help},
    {"echo",   "<...> echos the user input to the screen", cmd_echo},
    {"reboot", "This command reboots your pi", cmd_reboot},
    {"peek", "Peek prints the 4-byte value stored at memory address given", cmd_peek},
    {"poke", "takes two arguments, address and value, and stores value at address", cmd_poke},
};

static const int numCommands = 5;   // keeps track of total commands.

/*
 * cmd_echo
 * ========
 *
 * Prints its arguments and returns 0.
*/
int cmd_echo(int argc, const char *argv[])
{
    for (int i = 1; i < argc; ++i)
        shell_printf("%s ", argv[i]);
    shell_printf("\n");
    return 0;
}

/*
 * cmd_help
 * ========
 * Prints a list of all available commands and their descriptions.
 */
int cmd_help(int argc, const char *argv[])
{
    if (argc == 1) {
        for (int i = 0; i < numCommands; i++) { // PRINT ALL OUT.
            shell_printf(commands[i].name);
            shell_printf(": ");
            shell_printf(commands[i].description);
            shell_printf("\n");
        }
    } else {
        for (int i = 0; i < numCommands; i++) { // PRINT ONE CASE.
            if (strcmp(argv[1], commands[i].name) == 0) {
                shell_printf(commands[i].name);
                shell_printf(": ");
                shell_printf(commands[i].description);
                shell_printf("\n");
                return 0;
            }
        }
        shell_printf("error: no such command `%s`\n", argv[1]); // ERROR CASE.
        return 1;
    }
    return 0;
}

/* reboot reboots the pi. */
int cmd_reboot(int argc, const char* argv[]) {
    pi_reboot();
    return 0;
}

/*
 * cmd_peek
 * ========
 * Usage: peek [address]
 * Prints the contents (4 bytes) of memory at address.
 */
int cmd_peek(int argc, const char* argv[]) {
    // check if only 1 argument - check if valid, if not, print error message.
    if (argc != 2) {
        shell_printf("error: peek expects 1 argument [address]\n");
        return 1;
    }
    if (argc == 2) {
        const char *endstr;
        unsigned int address = (unsigned int)strtonum(argv[1], &endstr);
        if (*endstr != '\0') { //  CHECK FOR VALIDITY.
            shell_printf("error: peek cannot convert '%s'\n", argv[1]);
            return 1;
        } else if ((address % 4) != 0) { // alignment check.
            shell_printf("error: peek address must be 4-byte aligned\n");
            return 1;
        } else {
            unsigned int value = *(unsigned int *)address; // set peek.
            shell_printf("0x");
            shell_printf("%08x: %08x\n", address, value);
            return 0;
        }
    }
    return 0;
}

/*
 * cmd_poke
 * ========
 * Usage: poke [adress] [value]
 * Stores `value` into the memory at `address`.
 */
int cmd_poke(int argc, const char* argv[]) {
    // check for two arguments.
    if (argc != 3) {
        shell_printf("error: poke expects 2 arguments [address] [value]\n");
        return 1;
    } else {
        const char *endstr;
        const char *endstrval;
        unsigned int address = (unsigned int)strtonum(argv[1], &endstr);
        unsigned int value = (unsigned int)strtonum(argv[2], &endstrval);

        if (*endstr != '\0') { //  IF ENDPTR DOESN'T GET TO END.
            shell_printf("error: poke cannot convert '%s'\n", argv[1]);
            return 1;
        } else if (*endstrval != '\0') { //  IF ENDPTR DOES NOT READ END.
            shell_printf("error: poke cannot convert '%s'\n", argv[2]);
            return 1;
        } else if ((address % 4) != 0) { // ALIGNMENT CHECK.
            shell_printf("error: poke address must be 4-byte aligned\n");
            return 1;
        } else {
            *(unsigned int *)address = value;
            return 0;
        }
    }
    return 0;
}


void shell_init(formatted_fn_t print_fn)
{
    shell_printf = print_fn;
}

void shell_bell(void)
{
    uart_putchar('\a');
}

/*
 * shell_readline
 * ==============
 *
 * Reads a single line of input from the user. Store the characters typed on
 * the keyboard and stores them into a buffer `buf` of size `bufsize`. Reading
 * stops when the user types enter (\n). Support for backspace.
 */
void shell_readline(char buf[], int bufsize)
{
    //keyboard_init();
    char *ch = buf;
    int counter = 0;
    while (bufsize > (counter-1)) {
        unsigned char input = keyboard_read_next();
        if (input == '\n') { break;}
        if (input != '\b') { // if this is anything other than backspace.
            *ch = input;
            ch++;
            counter++;
            shell_printf("%c",input);
        } else { // BACKSPACE CHECK.
            if (counter == 0) {
                shell_bell();
                continue;
            }
            ch--;
            counter--;
            shell_printf("\b \b");
        }
    }
    *ch = '\0';
    counter++;

}

/*
 * Helper Function: Returns a pointer to a new null-terminated string containing
 * at most `n` bytes copied from the string pointed to by `src`.
 * Example: strndup("cs107e", 4) == "cs10"
 */
static char *strndup(const char *src, int n)
{
    char *dup = malloc(n+1);
    // print the first four
    for (int i = 0; i < n; i++) {
	    dup[i] = src[i];
    }
    dup[n] = '\0';
    return dup;
}

/* checks for a space */
static int isspace(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

/* tokenizes a line into individual "words" based on space separation */
static int tokenize(const char *line, char *array[],  int max)
{
    int ntokens = 0;

    while (*line != '\0' && ntokens < max) {
        while (isspace(*line)) line++;  // skip past spaces
        if (*line == '\0') break; // no more non-white chars
        const char *start = line;
        while (*line != '\0' && !isspace(*line)) line++; // advance to next space/null
        int nchars = line - start;
        array[ntokens++] = strndup(start, nchars);   // make heap-copy, add to array
    }
    return ntokens;
}

/*
 * shell_evaluate - parses and evaluates.
 * ==============
 */
int shell_evaluate(const char *line)
{
    if (*line != '\0') {
        const char *words[strlen(line)];
        int wordsLength = tokenize(line, (char **)words, strlen(line));
        if (strcmp(words[0],"echo") == 0) { // CHECK COMMANDS.
            shell_printf("\n");
            cmd_echo(wordsLength, words);
            return 0;
        } else if (strcmp(words[0],"help") == 0) {
            shell_printf("\n");
            cmd_help(wordsLength, words);
            return 0;
        } else if (strcmp(words[0],"reboot") == 0) {
            shell_printf("\n");
            cmd_reboot(wordsLength, words);
            return 0;
        } else if (strcmp(words[0],"peek") == 0) {
            shell_printf("\n");
            cmd_peek(wordsLength, words);
            return 0;
        } else if (strcmp(words[0],"poke") == 0) {
            shell_printf("\n");
            cmd_poke(wordsLength, words);
            return 0;
        } else {
            shell_printf("\n");
            shell_printf("error: no such command `%s`\n", words[0]);
            return 1;
        }
        for (int i = 0; i < wordsLength; i++) {
            free((char **)words[i]);
        }
    }
    return 0;

}

void shell_run(void)
{
    shell_printf("Welcome to the CS107E shell. Remember to type on your PS/2 keyboard!\n");
    while (1)
    {
        char line[LINE_LEN];

        shell_printf("Pi> ");
        shell_readline(line, sizeof(line));
        shell_evaluate(line);
    }
}

// Acknowledgements:
// tokenize, isSpace, and dup functions are from lab4 work and Github.
// spoke about work with Abla Ghaleb, Pierce, Lillian, Mary, as well as received
// help from TA's.
// Helped Holden and Elisa think through a few concepts in the lab.
// Overall: proud of this work, pulled it together, and learned a lot.
