#include <stdio.h>        //This one's necessary
#include <string.h>       //Used here for manipulating file names
#include <stdlib.h>       //Used here for generating pseudo random integers
#include <time.h>         //Helps to make random integers more random, by
                          // using system time as parameter

int get_int(char *, int, char);  //Information of this function is at line 387
void hill_cipher_encrypter(FILE *input_file, FILE *encrypted_file,
		FILE *key_file);
                                 //Information of this function is at line 145
void hill_cipher_encrypter_key(FILE *input_file, FILE *encrypted_file,
		char *key);
                                 //Information of this function is at line 273

/*
 * The main function, the driver of this program.
 */
int main(int argc, char *argv[]) {
	FILE *before_encryption;
	FILE *after_encryption;
	FILE *key_file;
	int key_file_open = 1;
	int error = 0;
	char before_encryption_filename[80] = {0};
	char after_encryption_filename[80] = {0};
	char key_filename[80] = {0};
	int i;
	char key_input[4];


	strcat(after_encryption_filename, "encryption_of_");
	strcat(key_filename, "key_of_");

	switch (argc) {
		case 1:	before_encryption = fopen("input.txt", "r");
				after_encryption = fopen("encryption_of_input.txt", "w");
				key_file = fopen("key_of_input.txt", "w");
				if (before_encryption == NULL) {
					error = 1;
					goto failed;
				}
				hill_cipher_encrypter(before_encryption, after_encryption, key_file);
				break;              //Correct

		case 2: strcat(before_encryption_filename, argv[1]);
				strcat(after_encryption_filename, before_encryption_filename);
				strcat(key_filename, before_encryption_filename);
				before_encryption = fopen(before_encryption_filename, "r");
				if (before_encryption == NULL) {
					error = 1;
					printf("\nCannot open %s", before_encryption_filename);
					goto failed;
				}
				after_encryption = fopen(after_encryption_filename, "w");
				key_file = fopen(key_filename, "w");
				hill_cipher_encrypter(before_encryption, after_encryption, key_file);
				break;

		case 3: if (argv[1][0] == '-' && argv[1][1] == 'i') {
					strcat(before_encryption_filename, argv[2]);
					strcat(after_encryption_filename, before_encryption_filename);
					strcat(key_filename, before_encryption_filename);
					before_encryption = fopen(before_encryption_filename, "r");
					if (before_encryption == NULL) {
					error = 1;
					printf("\nCannot open %s", before_encryption_filename);
					goto failed;
					}
					after_encryption = fopen(after_encryption_filename, "w");
					key_file = fopen(key_filename, "w");
					hill_cipher_encrypter(before_encryption, after_encryption, key_file);
					break;
				} else if (argv[1][0] == '-' && argv[1][1] == 'k') {
					before_encryption = fopen("input.txt", "r");
					if (strlen(argv[2]) < 4 || before_encryption == NULL) {
						error = 1;
						printf("\nCannot open %s or the length key_input was less than 4.", before_encryption_filename);
						goto failed;
					}
					after_encryption = fopen("encryption_of_input.txt", "w");
					key_file_open = 0;
					for (i = 0; i < 4; i++)
						key_input[i] = argv[2][i];
					hill_cipher_encrypter_key(before_encryption, after_encryption, key_input);
					break;
				} else {
					error = 1;
					goto failed;
				}

		case 5: if (argv[1][0] == '-' && argv[1][1] == 'i' && argv[3][0] == '-' && argv[3][1] == 'k') {
					strcat(before_encryption_filename, argv[2]);
					strcat(after_encryption_filename, before_encryption_filename);
					before_encryption = fopen(before_encryption_filename, "r");
					if ((strlen(argv[4]) < 4) || before_encryption == NULL) {
					error = 1;
					printf("\nCannot open %s or the length key_input was less than 4.", before_encryption_filename);
					goto failed;
					}
					after_encryption = fopen(after_encryption_filename, "w");
					key_file_open = 0;
					for (i = 0; i < 4; i++)
						key_input[i] = argv[4][i];
					hill_cipher_encrypter_key(before_encryption, after_encryption, key_input);
					break;
				} else if (argv[1][0] == '-' && argv[1][1] == 'k' && argv[3][0] == '-' && argv[3][1] == 'i') {
					strcat(before_encryption_filename, argv[4]);
					strcat(after_encryption_filename, before_encryption_filename);
					before_encryption = fopen(before_encryption_filename, "r");
					if ((strlen(argv[2]) < 4) || before_encryption == NULL) {
					error = 1;
					printf("\nCannot open %s or the length key_input was less than 4.", before_encryption_filename);
					goto failed;
					}
					after_encryption = fopen(after_encryption_filename, "w");
					key_file_open = 0;
					for (i = 0; i < 4; i++)
						key_input[i] = argv[2][i];
					hill_cipher_encrypter_key(before_encryption, after_encryption, key_input);
					break;
				} else {
					error = 1;
					goto failed;
				}

		default: error = 1;
				 goto failed;
	}
	
	if (key_file_open) {
		fclose(key_file);
	}
	fclose(after_encryption);
	fclose(before_encryption);

	if (error) {
		failed:
			printf("\nSomething went wrong, terminating");
			return 1;
	}
	
	return 0;
}

/*
 * Called from (function) -
 * ------------------------
 *  1. main()
 *
 * Parameters taken by this function -
 * -----------------------------------
 *  1. input_file - is the file which needs encryption
 *  2. encrypted_file - is the file where encryption will be saved
 *  3. key_file - is the file where key will be saved
 *
 * Description -
 * -------------
 *  This function encrypts the input_file.
 *
 * Returns -
 * ---------
 *  Nothing (void)
 *
 * No. of User Defined Functions called by this function -
 * -------------------------------------------------------
 *  1. get_int()
 */
void hill_cipher_encrypter(FILE *input_file, FILE *encrypted_file,
		FILE *key_file) {
	/*
	 * Description of variables used -
	 * -------------------------------
	 *  1. c - (to store each char read from the input_file)
	 *  2. key[4] - (The password)
	 *  3. in_buffer[2] - holds the integer value corresponding to dictionary' 
	 *                    of char read from file
	 *  4. op_buffer[2] - holds the matrix multiple of in_buffer and matrix
	 *  5. ch_buffer[2] - holds the characters <=> of in_buffer or op_buffer
	 *  6. matrix[2][2] - holds the key in matrix form
	 *  7. i, j - loop helpers
	 *  8. random_number - it defines itself :)
	 *  9. error = (if (there_is_error_processing_the_file) ? 1 : 0)
	 */
	int c = 'c';                  //Initializing it for a purpose.
	char key[4];
	int in_buffer[2];
	int op_buffer[2];
	char ch_buffer[2];
	int matrix[2][2];
	int i, j;
	int random_number;
	int error = 0;

	/*
	 * dictionary[97]
	 * --------------
	 *  A char array consisting of most commonly used alphabets
	 *  the first three values being ' ' '\n' & '\t'
	 *  and 94 other printable characters.
	 *  This makes total of 97 characters and that's good
	 *  because 97 is prime and any Matrix combination would work.
	 */
	char dictionary[97];
	int dict_length = 97;
	dictionary[0] = ' ';
	dictionary[1] = '\n';
	dictionary[2] = '\t';
	for (i = 3, j = 33; i < 97; i++, j++) {
		dictionary[i] = (char) j;
	}

	/*
	 * Random Key generator of 4 figure
	 * --------------------------------
	 *  It is done here by calling srand() with proper
	 *  arguments to utilize system 'time' to generate
	 *  pseudo random integers between 3 & 96 and then
	 *  use them to stitch a key of length 4.
	 */
	srand((unsigned) time(0));
	for (i = 0; i < 4; i++) {
		random_number = (rand() % 94) + 3;
		key[i] = dictionary[random_number];
		matrix[i / 2][i % 2] = random_number;
	}
	
	/*
	 * The engine of hill_encrypter.c
	 * -------------------------
	 *  The characters read from the input_file is stored in ch_buffer[] (char)
	 *  Then in_buffer[] stores the characters in ch_buffer[] by their 
	 *   offset in dictionary[] (int)
	 *  op_buffer[] stores the "(matrix multiplication) mod dict_length" (int)
	 *  The values in op_buffer[] is then converted into char
	 *   and stored into ch_buffer[]
	 *  ch_buffer[] is then printed to output_file.
	 *  That's how I write codes, seems a puzzle? Come on, you can do it ;)
	 */
	while (c != EOF && (c = fgetc(input_file)) != EOF) {
		ch_buffer[0] = c;
		ch_buffer[1] = (((c = fgetc(input_file)) != EOF) ? c : ' ');
		in_buffer[0] = get_int(dictionary, dict_length, ch_buffer[0]);
		in_buffer[1] = get_int(dictionary, dict_length, ch_buffer[1]);
		if (in_buffer[0] == -1 || in_buffer[1] == -1) {
			error = 1;
			goto die;   //Sometimes goto statement is quiet helpful.
		}
		op_buffer[0] = ((matrix[0][0] * in_buffer[0])
				+ (matrix[0][1] * in_buffer[1])) % dict_length;
		op_buffer[1] = ((matrix[1][0] * in_buffer[0])
				+ (matrix[1][1] * in_buffer[1])) % dict_length;
		ch_buffer[0] = dictionary[op_buffer[0]];
		ch_buffer[1] = dictionary[op_buffer[1]];
		fprintf(encrypted_file, "%c%c", ch_buffer[0], ch_buffer[1]);
		               //Encryption being written in encrypted_file
	}
	
	/*
	 * Print key[] in key_file
	 */
	for (i = 0; i < 4; i++)
		fprintf(key_file, "%c", key[i]);     //Key being written in key_file
	
	/*
	 * Just precautionary, I know it will never happen.
	 */
	if (error) {
		die:
			printf("\nError encrypting!!!");
	}
}

/*
 * Called from (function) -

 * ------------------------
 *  1. main()
 *
 * Parameters taken by this function -
 * -----------------------------------
 *  1. input_file - is the file which needs encryption
 *  2. encrypted_file - is the file where encryption will be saved
 *  3. *key - is the key provided by user for encryption
 *
 * Description -
 * -------------
 *  This function encrypts the input_file.
 *
 * Returns -
 * ---------
 *  Nothing (void)
 *
 * No. of User Defined Functions called by this function -
 * -------------------------------------------------------

 *  1. get_int()
 */
void hill_cipher_encrypter_key(FILE *input_file, FILE *encrypted_file,
		char *key) {
	/*
	 * Description of variables used -
	 * -------------------------------
	 *  1. c - (to store each char read from the input_file)
	 *  2. key_length - (The length of password)
	 *  3. in_buffer[2] - holds the integer value corresponding to dictionary' 
	 *                    of char read from file
	 *  4. op_buffer[2] - holds the matrix multiple of in_buffer and matrix
	 *  5. ch_buffer[2] - holds the characters <=> of in_buffer or op_buffer
	 *  6. matrix[2][2] - holds the key in matrix form
	 *  7. i, j - loop helpers
	 *  8. error = (if (there_is_error_processing_the_file) ? 1 : 0)
	 */
	int c = 'c';                  //Initializing it for a purpose.
	int key_length = 4;
	int in_buffer[2];
	int op_buffer[2];
	char ch_buffer[2];
	int matrix[2][2];
	int i, j;
	int error = 0;

	/*
	 * dictionary[97]
	 * --------------
	 *  A char array consisting of most commonly used alphabets
	 *  the first three values being ' ' '\n' & '\t'
	 *  and 94 other printable characters.
	 *  This makes total of 97 characters and that's good
	 *  because 97 is prime and any Matrix combination would work.
	 */
	char dictionary[97];
	int dict_length = 97;
	dictionary[0] = ' ';
	dictionary[1] = '\n';
	dictionary[2] = '\t';
	for (i = 3, j = 33; i < 97; i++, j++) {
		dictionary[i] = (char) j;
	}

	/*
	 * Convert key[4] into matrix[2][2] with corresponding integers.
	 */
	for (i = 0; i < key_length; i++) {
		matrix[i / 2][i % 2] = get_int(dictionary, dict_length, key[i]);
	}
	
	/*
	 * The engine of hill_encrypter.c
	 * -------------------------
	 *  The characters read from the input_file is stored in ch_buffer[] (char)
	 *  Then in_buffer[] stores the characters in ch_buffer[] by their 
	 *   offset in dictionary[] (int)
	 *  op_buffer[] stores the "(matrix multiplication) mod dict_length" (int)
	 *  The values in op_buffer[] is then converted into char
	 *   and stored into ch_buffer[]
	 *  ch_buffer[] is then printed to output_file.
	 *  That's how I write codes, seems a puzzle? Come on, you can do it ;)
	 */
	while (c != EOF && (c = fgetc(input_file)) != EOF) {
		ch_buffer[0] = c;
		ch_buffer[1] = (((c = fgetc(input_file)) != EOF) ? c : ' ');
		in_buffer[0] = get_int(dictionary, dict_length, ch_buffer[0]);
		in_buffer[1] = get_int(dictionary, dict_length, ch_buffer[1]);
		if (in_buffer[0] == -1 || in_buffer[1] == -1) {
			error = 1;
			goto die;   //Sometimes goto statement is quiet helpful.
		}
		op_buffer[0] = ((matrix[0][0] * in_buffer[0])
				+ (matrix[0][1] * in_buffer[1])) % dict_length;
		op_buffer[1] = ((matrix[1][0] * in_buffer[0])
				+ (matrix[1][1] * in_buffer[1])) % dict_length;
		ch_buffer[0] = dictionary[op_buffer[0]];
		ch_buffer[1] = dictionary[op_buffer[1]];
		fprintf(encrypted_file, "%c%c", ch_buffer[0], ch_buffer[1]);
		               //Encryption being written in encrypted_file
	}
	
	/*
	 * Just precautionary, I know it will never happen.
	 */
	if (error) {
		die:
			printf("\nError encrypting!!!");
	}
}

/*
 * Called from (function) -
 * ------------------------
 *  1. hill_cipher_encrypter()
 *
 * Parameters taken by this function -
 * -----------------------------------
 *  1. dict_pointer - holds the pointer of 'dictionary[]'.
 *  2. length - specifies the 'length of dictionary'.
 *  3. x - the 'char' whose offset in 'dictionary[]' is needed
 *         to the calling function.
 *
 * Description -
 * -------------
 *  This function returns the 'offset' of 'char x' by
 *  searching it sequentially in dictionary[].
 *  Just in case if the 'char x' is not found, it returns -1 and weeps
 *  because calling function is 'going to die'.
 *
 * Returns -
 * ---------
 *  'i' (int) i.e. the offset of x.
 */
int get_int(char *dict_pointer, int length, char x) {
	int i;
	/*
	 * Sequential Search
	 */
	for (i = 0; i < length; i++)
		if (dict_pointer[i] == x)
			return i;
	return -1;   //Oh shit!!!
}
