#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXBUF 1024*1024
#define MAXKEY 256

// GLOBAL FLAGS AND SRC DEST
char *src  = NULL; // source file
char *dest = NULL; // destination file

int b_encrypt = 0; // encrypt flag -e, -i, --encrypt
int b_decrypt = 0; // decrypt flag -d, -o, --decrypt
int b_silent  = 0; // silent flag  -s, --silent
int b_interm  = 0; // input from the terminal, -it
int b_outerm  = 0; // output to the terminal only, ot
int b_help	  = 0; // flag for help text

// FLAG DEFINITION STRUCT
typedef struct {
    const char *short_name;
    const char *long_name;
    int *flag;
} FlagDef;

// BUFFERS AND KEYS
static char input_buffer [MAXBUF];
static char output_buffer[MAXBUF];
char keyed_alphabet[26] = {0};
char viginere_table[26][26];
char alphabet_key[26]      = "KRYPTOS";
char keyword[MAXKEY]	   = "HIDDEN";


// Generate the alphabet that has been keyed
void generate_keyed_alphabet() 
{
	int key_len = strlen(alphabet_key);
	char a_ctr = 0;
	char c;

	if (key_len > 26 ) {
		printf("[  ERROR  ] : Alphabet key longer than alphabet (max 26)\n");
		return;
	}  

	// Make alphabet key uppercase 
	for (int i = 0; i < key_len; i++) {
		if (alphabet_key[i] >= 'a' && alphabet_key[i] <= 'z') {
			alphabet_key[i] -= 'a' - 'A';  
		}
	}
	
	// Check if the alphabet key has repetitions
	int seen[26] = {0};  
	for (int i = 0; i < key_len; i++) {
		int index = alphabet_key[i] - 'A';
        if (index < 0 || index >= 26) {
            printf("[  ERROR  ] : Invalid character in A-key\n");
			exit(1);
        }
		if (seen[index]) {
			printf("[  ERROR  ] : Alphabet key has repetitions, choose another key\n");
			exit(1);
		}
		seen[index] = 1;
	}

	// Fill the alphabet with the key
	for (int i = 0; i < key_len; i++) {
		keyed_alphabet[i] = alphabet_key[i];
	}

	// Fill the rest of the alphabet with letters not in the key
	int pos = key_len;
	for (int i = 0; i < 26; i++) {
		char c = 'A' + i;
		int already_used = 0;
		
		// Check if this letter is already in the key
		for (int j = 0; j < key_len; j++) {
			if (keyed_alphabet[j] == c) {
				already_used = 1;
				break;
			}
		}
		
		if (!already_used) {
			keyed_alphabet[pos++] = c;
		}
	}
}

// function for shifting the alphabet
void shift_alphabet(char* src, char* dest,int shift) 
{
	for (int i = 0; i < 26; i++) {
		dest[i] = src[(i + shift)%26];
	}
}

// Generates the viginere table
void generate_table() 
{
	// Shift the alphabet by 'shift'
	for (int i = 0; i < 26; i++) {
		shift_alphabet(
			keyed_alphabet,
			viginere_table[i], 
			i
		);
	}
} 

// parse input from the terminal
void parse_input (char* buf) 
{
	char c;
	int b_ctr = 0;
	// Read until EOF or newline
	for (int i = 0; (c = getchar()) != EOF && c != '\n'; i++) {
		// check if buffer exceeded
		if (i >= MAXBUF - 1) {	
			printf("[ ERROR ] : File too long, %d chars max\n", MAXBUF);
			exit(1);
		}
		
		// make it uppercase
		if (c >= 'a' && c <= 'z') {
			c -= 'a' - 'A';
		}
		
		// change period into 'X'
		if (c == '.') {
			c = 'X';
		}
		
		// check if the character is within the alphabet
		if (c >= 'A' && c <= 'Z') {
			buf[b_ctr] = c;
			b_ctr++;
		}
	}
	buf[b_ctr] = '\0';  // Null-terminate the buffer
}


// generate the cipher text for encryption
void generate_ciphertext(char* in_buf, char* out_buf)
{
	int buf_len = strlen(in_buf);
	int key_len = strlen(keyword);
	char c, h;
	int  x, y;

	// encryption loop
	for (int i = 0; i < buf_len; i++) {

		// checking the character fron input and key stream
		c = in_buf[i];
		h = keyword[i % key_len];

		// looking for character index
		for (int j = 0; j < 26; j++) {
			if (c == keyed_alphabet[j]) {
				x = j;
				break;
			}
		}
		for (int j = 0; j < 26; j++) {
			if (h == keyed_alphabet[j]) {
				y = j;
				break;
			}
		}

		// saving the table value to the output buffer 
		out_buf[i] = viginere_table[x][y];
	}
	out_buf[buf_len] = '\0';
}

// generate the plaintext for decryption
void generate_plaintext(char* in_buf, char* out_buf)
{  
	int buf_len = strlen(in_buf);
	int key_len = strlen(keyword);
	char shift_alpha[26];
	char c, h;
	int  x, y;

	// decyption loop
	for (int i = 0; i < buf_len; i++) {
		c = in_buf[i];
		h = keyword[i % key_len];

		// looking for character idx
		for (int j = 0; j < 26; j++) {
			if ( h == keyed_alphabet[j]) {
				x = j;
				break;
			}
		}
		// shift alphabet to get there
		shift_alphabet(keyed_alphabet, shift_alpha, x);

		// looking for c idx
		for (int j = 0; j < 26; j++) {
			if ( c == shift_alpha[j]) {
				y = j;
				break;
			}
		}
		// save to output buffer
		out_buf[i] = keyed_alphabet[y];
	} 
	out_buf[buf_len] = '\0';
}

// Encyption pipeline
void encrypt() 
{
    generate_keyed_alphabet();
    generate_table();
    generate_ciphertext(input_buffer, output_buffer);
}

// Decryption pipeline
void decrypt() 
{
    generate_keyed_alphabet();
    generate_plaintext(input_buffer, output_buffer);
}

void print_help() 
{
    printf("Usage: %s [OPTIONS] [SOURCE] [DEST]\n", "program");
    printf("\nOptions:\n");
    printf("  -e,  --encrypt, -i  Encrypt mode\n");
    printf("  -d,  --decrypt, -o  Decrypt mode\n");
    printf("  -s,  --silent       Suppress output messages\n");
    printf("  -h,  --help         Show this help\n");
    printf("  -it, --interm       Input from terminal (stdin)\n");
    printf("  -ot, --outerm       Output to terminal (stdout)\n");
    printf("  -keyword=KEY        Encryption keyword\n");
    printf("  -akey=KEY           Alphabet substitution key\n");
    printf("\nExamples:\n");
    printf("  %s -e -keyword=secret input.txt output.txt\n", "program");
    printf("  %s -d -keyword=secret encrypted.txt decrypted.txt\n", "program");
}

// Fix file reading function
void read_file(const char* filename, char* buffer) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[  ERROR  ] : Cannot open file: %s\n", filename);
        exit(1);
    }
    
    int c;
    int i = 0;
    while ((c = fgetc(file)) != EOF && i < MAXBUF - 1) {
        // Process character (uppercase, replace '.', etc.)
        if (c >= 'a' && c <= 'z') {
            c -= 'a' - 'A';
        }
        if (c == '.') {
            c = 'X';
        }
        if (c >= 'A' && c <= 'Z') {
            buffer[i++] = c;
        }
    }
	if (i == MAXBUF - 1) {
		printf("[ WARNING ] : Maximum buffer size reached\n");
	}
    buffer[i] = '\0';
    fclose(file);
}

void write_file(const char* filename, const char* buffer) 
{
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("[  ERROR  ]: Cannot write to file: %s\n", filename);
        exit(1);
    }
    fprintf(file, "%s", buffer);
    fclose(file);
}


static void add_positional_arg(char *arg)
{
    char **first;
    char **second;

    if (b_interm) {
        first  = &dest;
        second = &src;
    } else {
        first  = &src;
        second = &dest;
    }

    if (*first == NULL)
        *first = arg;
    else if (*second == NULL)
        *second = arg;
    else
        printf("[ WARNING ] : Extra argument ignored: %s\n", arg);
}

static void parse_keyword(const char *value)
{
    strncpy(keyword, value, MAXKEY - 1);
    keyword[MAXKEY - 1] = '\0';

    for (int i = 0; keyword[i]; i++)
        keyword[i] = toupper((unsigned char)keyword[i]);
}

static void parse_alphabet_key(const char *value)
{
    strncpy(alphabet_key, value, 26);
    alphabet_key[26] = '\0';
}

void parse_command(int argc, char *argv[])
{
    static FlagDef flags[] = {
        { "-e",  "--encrypt", &b_encrypt },
        { "-i",  NULL,        &b_encrypt },

        { "-d",  "--decrypt", &b_decrypt },
        { "-o",  NULL,        &b_decrypt },

        { "-s",  "--silent",  &b_silent  },

        { "-it", "--interm",  &b_interm  },
        { "-ot", "--outerm",  &b_outerm  },

        { NULL,  NULL, NULL }
    };

    for (int i = 1; i < argc; i++) {

        char *arg = argv[i];

        /* positional argument */
        if (arg[0] != '-') {
            add_positional_arg(arg);
            continue;
        }

        /* help */
        if (!strcmp(arg, "-h") ||
            !strcmp(arg, "--help"))
        {
            print_help();
            exit(EXIT_SUCCESS);
        }

        /* keyword=value */
        if (!strncmp(arg, "-keyword=", 9)) {
            parse_keyword(arg + 9);
            continue;
        }

        /* akey=value */
        if (!strncmp(arg, "-akey=", 6)) {
            parse_alphabet_key(arg + 6);
            continue;
        }

        /* simple flags */
        int found = 0;

        for (int j = 0; flags[j].flag != NULL; j++) {

            if (!strcmp(arg, flags[j].short_name) ||
                (flags[j].long_name &&
                 !strcmp(arg, flags[j].long_name)))
            {
                *flags[j].flag = 1;
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("[ WARNING ] : Unknown flag ignored: %s\n", arg);
        }
    }
}

void process_command(void)
{
    // Acquire input from file or stdin 
    if (src) {
        read_file(src, input_buffer);
    }
    else if (b_interm) {
        parse_input(input_buffer);
    }
    else {
        printf("[  ERROR  ] : No input source specified\n");
        exit(EXIT_FAILURE);
    }

	// encrypt and decrypt conflict
	if ( b_encrypt == 1 && b_decrypt == 1) {
		printf("[  ERROR  ] : Encrypt and decrypt flags in conflict. Choose one\n");
		exit(1);
	}


	// Check if encrypt or decrypt is specified
    if (!b_encrypt && !b_decrypt) {
        printf("[  ERROR  ] : Must specify encrypt or decrypt flags\n");
        exit(1);
    }

    if (b_outerm && b_silent) {
        printf("[ WARNING ] : Terminal output and silent mode both enabled. No output will be shown\n");
    }
	
	char* output_state;

    // Perform operation
	if (b_encrypt) {
        encrypt();
		output_state = "CIPHERTEXT";
    } else if (b_decrypt) {
        decrypt();
		output_state = "PLAINTEXT";
    }

    // Save output if a destination file was provided 
    if (dest && !b_outerm) {
        write_file(dest, output_buffer);
    }
	
    // Display output unless silent mode is enabled 
    if (!b_silent) {
        if (b_outerm || !dest) {
            printf("\n[ %s ]\n\n%s\n",output_state, output_buffer);
        }
        else {
            printf("\n[ %s ]\n\n%s\n",output_state, output_buffer);
        }
    }
}

int main ( int argc, char* argv[])
{
	parse_command( argc, argv );
	process_command();
	return 0;
}
