#include <stdio.h>               //This one's necessary
#include <string.h>              //Used here for manipulating file names

int calculate_mod(int, int);     //Information of this function is at line 542
int get_int(char *, int, char);  //Information of this function is at line 570
void hill_cipher_decrypter(FILE *, FILE *, FILE *);
                                 //Information of this function is at line 142
void hill_cipher_decrypter_key(FILE *, char *, FILE *);
                                 //Information of this function is at line 343

/*
 * The main function, the driver of this program.
 */
int main(int argc, char *argv[]) {
	FILE *before_decryption;
	FILE *after_decryption;
	FILE *key_file;
	int key_file_open = 1;
	int error = 0;
	char before_decryption_filename[80] = {0};
	char after_decryption_filename[80] = {0};
	char key_filename[80] = {0};
	int i;
	char key_input[4];


	strcat(after_decryption_filename, "decryption_of_");

	switch (argc) {
		case 1:	before_decryption = fopen("encryption_of_input.txt", "r");
				key_file = fopen("key_of_input.txt", "r");
				if (before_decryption == NULL || key_file == NULL) {
					error = 1;
					goto failed;
				}
				after_decryption = fopen("decryption_of_input.txt", "w");
				hill_cipher_decrypter(before_decryption, key_file, after_decryption);
				break;

		case 3: strcat(before_decryption_filename, argv[1]);
				strcat(after_decryption_filename, before_decryption_filename);
				strcat(key_filename, argv[2]);
				before_decryption = fopen(before_decryption_filename, "r");
				key_file = fopen(key_filename, "r");
				if (before_decryption == NULL || key_file == NULL) {
					error = 1;
					printf("\nCannot open %s or %s", before_decryption_filename, key_filename);
					goto failed;
				}
				after_decryption = fopen(after_decryption_filename, "w");
				hill_cipher_decrypter(before_decryption, key_file, after_decryption);
				break;

		case 4: if (argv[2][0] == '-' && argv[2][1] == 'k') {
					strcat(before_decryption_filename, argv[1]);
					strcat(after_decryption_filename, before_decryption_filename);
					before_decryption = fopen(before_decryption_filename, "r");
					key_file_open = 0;
					if ((strlen(argv[3]) < 4) || before_decryption == NULL) {
						error = 1;
						printf("\nCannot open %s or the length of key entered is less than 4", before_decryption_filename);
						goto failed;
					}
					after_decryption = fopen(after_decryption_filename, "w");
					for (i = 0; i < 4; i++)
						key_input[i] = argv[3][i];
					hill_cipher_decrypter_key(before_decryption, key_input, after_decryption);
					break;
				} else if(argv[1][0] == '-' && argv[1][1] == 'i') {
					strcat(before_decryption_filename, argv[2]);
					strcat(after_decryption_filename, before_decryption_filename);
					strcat(key_filename, argv[3]);
					before_decryption = fopen(before_decryption_filename, "r");
					key_file = fopen(key_filename, "r");
					if (before_decryption == NULL || key_file == NULL) {
						error = 1;
						printf("\nCannot open %s or %s", before_decryption_filename, key_filename);
						goto failed;
					}
					after_decryption = fopen(after_decryption_filename, "w");
					hill_cipher_decrypter(before_decryption, key_file, after_decryption);
					break;
				} else {
					error = 1;
					goto failed;
				}
		
		case 5: if (argv[1][0] == '-' && argv[1][1] == 'i' && argv[3][0] == '-' && argv[3][1] == 'k') {
					strcat(before_decryption_filename, argv[2]);
					strcat(after_decryption_filename, before_decryption_filename);
					before_decryption = fopen(before_decryption_filename, "r");
					key_file_open = 0;
					if ((strlen(argv[4]) < 4) || before_decryption == NULL) {
						error = 1;
						printf("\nCannot open %s or the length of key entered is less than 4", before_decryption_filename);
						goto failed;
					}
					after_decryption = fopen(after_decryption_filename, "w");
					for (i = 0; i < 4; i++)
						key_input[i] = argv[4][i];
					hill_cipher_decrypter_key(before_decryption, key_input, after_decryption);
					break;
				} else if (argv[1][0] == '-' && argv[1][1] == 'k' && argv[3][0] == '-' && argv[3][1] == 'i') {
					strcat(before_decryption_filename, argv[4]);
					strcat(after_decryption_filename, before_decryption_filename);
					before_decryption = fopen(before_decryption_filename, "r");
					key_file_open = 0;
					if ((strlen(argv[2]) < 4) || before_decryption == NULL) {
						error = 1;
						printf("\nCannot open %s or the length of key entered is less than 4", before_decryption_filename);
						goto failed;
					}
					after_decryption = fopen(after_decryption_filename, "w");
					for (i = 0; i < 4; i++)
						key_input[i] = argv[2][i];
					hill_cipher_decrypter_key(before_decryption, key_input, after_decryption);
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
	fclose(after_decryption);
	fclose(before_decryption);

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
 *  1. encrypted_file - is the file which needs decryption
 *  2. key_file - is the file where key of the encrypted_file is saved
 *  3. decrypted_file - is the file where decryption will be saved
 *                      or original file will be recovered
 *
 * Description -
 * -------------
 *  This function decrypts the encrypted_file.
 *
 * Returns -
 * ---------
 *  Nothing (void)
 *
 * No. of User Defined Functions called by this function -
 * -------------------------------------------------------
 *  1. calculate_mod()
 *  2. get_int()
 */
void hill_cipher_decrypter(FILE *encrypted_file, FILE *key_file,
		FILE *decrypted_file) {
	/*
	 * Description of variables used -
	 * -------------------------------
	 *  1. c - (to store each char read from encrypted_file)
	 *  2. i, j - loop helpers
	 *  3. matrix[2][2] - (to store char from key_file in integer format)
	 *  4. inverse_matrix[2][2] - stores the 'inverse' of matrix[][]
	 *  5. determinant_of_matrix - it defines itself :)
	 *  6. mod_dict_length_of_inverse_matrix[2][2] - stores mod97 
	 *                                               of inverse_matrix[][]
	 *  7. modular_inverse_of_determinant - it defines itself :)
	 *  8. decryption_matrix[2][2] - stores the decryption_matrix
	 *  9. key[4] - (the password read from key_file)
	 * 10. ch_buffer[2] - holds the characters <=> of in_buffer or op_buffer
	 * 11. in_buffer[2] - holds the integer value corresponding to dictionary' 
	 *                    of char read from file
	 * 12. op_buffer[2] - holds the matrix multiple of in_buffer and matrix
	 * 13. error = (if (there_is_error_processing_the_file) ? 1 : 0)
	 */
	int c;
	int i, j;
	int matrix[2][2];
	int inverse_matrix[2][2];
	int determinant_of_matrix;
	int mod_dict_length_of_inverse_matrix[2][2];
	int modular_inverse_of_determinant;
	int decryption_matrix[2][2];
	char key[4];
	char ch_buffer[2];
	int in_buffer[2];
	int op_buffer[2];
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
	 * Read the key_file ,parse the key into matrix form
	 * & store it in key[4].
	 */
	for (i = 0; i < 4; i++) {
		key[i] = fgetc(key_file);
		matrix[i / 2][i % 2] = get_int(dictionary, dict_length, key[i]);
		if (matrix[i / 2][i % 2] == -1) {
			error = 1;
			goto die;
		}
	}

	/*
	 * Calculating inverse of matrix[][] and storing it into
	 * inverse_matrix[][]
	 */
	inverse_matrix[0][0] = matrix[1][1];
	inverse_matrix[0][1] = -1 * matrix[0][1];
	inverse_matrix[1][0] = -1 * matrix[1][0];
	inverse_matrix[1][1] = matrix[0][0];

	/*
	 * Calculating the determinant of matrix[][] and storing it into
	 * determinant_of_matrix
	 */
	determinant_of_matrix = (matrix[0][0] * matrix[1][1])
			- (matrix[0][1] * matrix[1][0]);

	/*
	 * Calculate (modular dict_length) i.e. mod97 of inverse_matrix
	 * and storing the result to mod_dict_length_of_inverse_matrix[][]
	 */
	for (i = 0; i < 4; i++) {
		mod_dict_length_of_inverse_matrix[0][0] = calculate_mod(
				inverse_matrix[0][0], dict_length);
		mod_dict_length_of_inverse_matrix[0][1] = calculate_mod(
				inverse_matrix[0][1], dict_length);
		mod_dict_length_of_inverse_matrix[1][0] = calculate_mod(
				inverse_matrix[1][0], dict_length);
		mod_dict_length_of_inverse_matrix[1][1] = calculate_mod(
				inverse_matrix[1][1], dict_length);
	}

	/*
	 * Calculate modular inverse of determinant_of_matrix
	 */
	if (determinant_of_matrix >= 0) {
		for (i = 1; i < dict_length; i++) {
			if ((determinant_of_matrix * i) % dict_length == 1) {
				modular_inverse_of_determinant = i;
				break;
			}
		}
	}
	else {
		for (i = 1; i < dict_length; i++) {
			if ((-1 * determinant_of_matrix * i) % dict_length == 1) {
				modular_inverse_of_determinant = (-1 * i);
				break;
			}
		}
	}
	modular_inverse_of_determinant = calculate_mod(modular_inverse_of_determinant, dict_length);
	
	/*
	 * Creating decryption_matrix[][]
	 * decryption_matrix[][] = ( (modular_inverse_of_determinant)
	 *                          *(mod_dict_length_of_inverse_matrix[][])
	 *                         ) mod dict_length
	 */
	decryption_matrix[0][0] = calculate_mod(
			modular_inverse_of_determinant
					* mod_dict_length_of_inverse_matrix[0][0], dict_length);
	decryption_matrix[0][1] = calculate_mod(
			modular_inverse_of_determinant
					* mod_dict_length_of_inverse_matrix[0][1], dict_length);
	decryption_matrix[1][0] = calculate_mod(
			modular_inverse_of_determinant
					* mod_dict_length_of_inverse_matrix[1][0], dict_length);
	decryption_matrix[1][1] = calculate_mod(
			modular_inverse_of_determinant
					* mod_dict_length_of_inverse_matrix[1][1], dict_length);

	/*
	 * The engine of hill_decrypter.c
	 * -------------------------
	 *  The characters read from the input_file is stored in ch_buffer[] (char)
	 *  Then in_buffer[] stores the characters in ch_buffer[] by their
	 *   offset in dictionary[] (int)
	 *  op_buffer[] stores the "(matrix multiplication) mod dict_length" (int)
	 *  The values in op_buffer[] is then converted into char
	 *   and stored into ch_buffer[]
	 *  ch_buffer[] is then printed to decrypted_file.
	 *  Tired? This too will pass ;)
	 */
	while ((c = fgetc(encrypted_file)) != EOF) {
		ch_buffer[0] = c;
		ch_buffer[1] = fgetc(encrypted_file);
		in_buffer[0] = get_int(dictionary, dict_length, ch_buffer[0]);
		in_buffer[1] = get_int(dictionary, dict_length, ch_buffer[1]);
		if (in_buffer[0] == -1 || in_buffer[1] == -1) {
			error = 1;
			goto die;   //Sometimes goto statement is quiet helpful. Deja vu?
		}
		op_buffer[0] = ((decryption_matrix[0][0] * in_buffer[0])
				+ (decryption_matrix[0][1] * in_buffer[1])) % dict_length;
		op_buffer[1] = ((decryption_matrix[1][0] * in_buffer[0])
				+ (decryption_matrix[1][1] * in_buffer[1])) % dict_length;
		ch_buffer[0] = dictionary[op_buffer[0]];
		ch_buffer[1] = dictionary[op_buffer[1]];
		fprintf(decrypted_file, "%c%c", ch_buffer[0], ch_buffer[1]);
	}
	
	/*
	 * Just precautionary, I know it will never happen here too.
	 */
	if (error) {
		die:
			printf("\nError decrypting!!!");
	}
}

/*
 * Called from (function) -
 * ------------------------
 *  1. main()
 *
 * Parameters taken by this function -
 * -----------------------------------
 *  1. encrypted_file - is the file which needs decryption
 *  2. *key - is the key input by user for decryption
 *  3. decrypted_file - is the file where decryption will be saved
 *                      or original file will be recovered
 *
 * Description -
 * -------------
 *  This function decrypts the encrypted_file.
 *
 * Returns -
 * ---------
 *  Nothing (void)
 *
 * No. of User Defined Functions called by this function -
 * -------------------------------------------------------
 *  1. calculate_mod()
 *  2. get_int()
 */
void hill_cipher_decrypter_key(FILE *encrypted_file, char *key,
		FILE *decrypted_file) {
	/*
	 * Description of variables used -
	 * -------------------------------
	 *  1. c - (to store each char read from encrypted_file)
	 *  2. i, j - loop helpers
	 *  3. matrix[2][2] - (to store char from key_file in integer format)
	 *  4. inverse_matrix[2][2] - stores the 'inverse' of matrix[][]
	 *  5. determinant_of_matrix - it defines itself :)
	 *  6. mod_dict_length_of_inverse_matrix[2][2] - stores mod97 
	 *                                               of inverse_matrix[][]
	 *  7. modular_inverse_of_determinant - it defines itself :)
	 *  8. decryption_matrix[2][2] - stores the decryption_matrix
	 *  9. key_length - (the length password)
	 * 10. ch_buffer[2] - holds the characters <=> of in_buffer or op_buffer
	 * 11. in_buffer[2] - holds the integer value corresponding to dictionary' 
	 *                    of char read from file
	 * 12. op_buffer[2] - holds the matrix multiple of in_buffer and matrix
	 * 13. error = (if (there_is_error_processing_the_file) ? 1 : 0)
	 */
	int c;
	int i, j;
	int matrix[2][2];
	int inverse_matrix[2][2];
	int determinant_of_matrix;
	int mod_dict_length_of_inverse_matrix[2][2];
	int modular_inverse_of_determinant;
	int decryption_matrix[2][2];
	int key_length = 4;
	char ch_buffer[2];
	int in_buffer[2];
	int op_buffer[2];
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
	 * Parse the key[4] into matrix[2][2]
	 */
	for (i = 0; i < key_length; i++) {
		matrix[i / 2][i % 2] = get_int(dictionary, dict_length, key[i]);
		if (matrix[i / 2][i % 2] == -1) {
			error = 1;
			goto die;
		}
	}

	/*
	 * Calculating inverse of matrix[][] and storing it into
	 * inverse_matrix[][]
	 */
	inverse_matrix[0][0] = matrix[1][1];
	inverse_matrix[0][1] = -1 * matrix[0][1];
	inverse_matrix[1][0] = -1 * matrix[1][0];
	inverse_matrix[1][1] = matrix[0][0];

	/*
	 * Calculating the determinant of matrix[][] and storing it into
	 * determinant_of_matrix
	 */
	determinant_of_matrix = (matrix[0][0] * matrix[1][1])
			- (matrix[0][1] * matrix[1][0]);

	/*
	 * Calculate (modular dict_length) i.e. mod97 of inverse_matrix
	 * and storing the result to mod_dict_length_of_inverse_matrix[][]
	 */
	for (i = 0; i < 4; i++) {
		mod_dict_length_of_inverse_matrix[0][0] = calculate_mod(
				inverse_matrix[0][0], dict_length);
		mod_dict_length_of_inverse_matrix[0][1] = calculate_mod(
				inverse_matrix[0][1], dict_length);
		mod_dict_length_of_inverse_matrix[1][0] = calculate_mod(
				inverse_matrix[1][0], dict_length);
		mod_dict_length_of_inverse_matrix[1][1] = calculate_mod(
				inverse_matrix[1][1], dict_length);
	}

	/*
	 * Calculate modular inverse of determinant_of_matrix
	 */
	if (determinant_of_matrix >= 0) {
		for (i = 1; i < dict_length; i++) {
			if ((determinant_of_matrix * i) % dict_length == 1) {
				modular_inverse_of_determinant = i;
				break;
			}
		}
	}
	else {
		for (i = 1; i < dict_length; i++) {
			if ((-1 * determinant_of_matrix * i) % dict_length == 1) {
				modular_inverse_of_determinant = (-1 * i);
				break;
			}
		}
	}
	modular_inverse_of_determinant = calculate_mod(modular_inverse_of_determinant, dict_length);
	
	/*
	 * Creating decryption_matrix[][]
	 * decryption_matrix[][] = ( (modular_inverse_of_determinant)
	 *                          *(mod_dict_length_of_inverse_matrix[][])
	 *                         ) mod dict_length
	 */
	decryption_matrix[0][0] = calculate_mod(
			modular_inverse_of_determinant
					* mod_dict_length_of_inverse_matrix[0][0], dict_length);
	decryption_matrix[0][1] = calculate_mod(
			modular_inverse_of_determinant
					* mod_dict_length_of_inverse_matrix[0][1], dict_length);
	decryption_matrix[1][0] = calculate_mod(
			modular_inverse_of_determinant
					* mod_dict_length_of_inverse_matrix[1][0], dict_length);
	decryption_matrix[1][1] = calculate_mod(
			modular_inverse_of_determinant
					* mod_dict_length_of_inverse_matrix[1][1], dict_length);

	/*
	 * The engine of hill_decrypter.c
	 * -------------------------
	 *  The characters read from the input_file is stored in ch_buffer[] (char)
	 *  Then in_buffer[] stores the characters in ch_buffer[] by their
	 *   offset in dictionary[] (int)
	 *  op_buffer[] stores the "(matrix multiplication) mod dict_length" (int)
	 *  The values in op_buffer[] is then converted into char
	 *   and stored into ch_buffer[]
	 *  ch_buffer[] is then printed to decrypted_file.
	 *  Tired? This too will pass ;)
	 */
	while ((c = fgetc(encrypted_file)) != EOF) {
		ch_buffer[0] = c;
		ch_buffer[1] = fgetc(encrypted_file);
		in_buffer[0] = get_int(dictionary, dict_length, ch_buffer[0]);
		in_buffer[1] = get_int(dictionary, dict_length, ch_buffer[1]);
		if (in_buffer[0] == -1 || in_buffer[1] == -1) {
			error = 1;
			goto die;   //Sometimes goto statement is quiet helpful. Deja vu?
		}
		op_buffer[0] = ((decryption_matrix[0][0] * in_buffer[0])
				+ (decryption_matrix[0][1] * in_buffer[1])) % dict_length;
		op_buffer[1] = ((decryption_matrix[1][0] * in_buffer[0])
				+ (decryption_matrix[1][1] * in_buffer[1])) % dict_length;
		ch_buffer[0] = dictionary[op_buffer[0]];
		ch_buffer[1] = dictionary[op_buffer[1]];
		fprintf(decrypted_file, "%c%c", ch_buffer[0], ch_buffer[1]);
	}
	
	/*
	 * Just precautionary, I know it will never happen here too.
	 */
	if (error) {
		die:
			printf("\nError decrypting!!!");
	}
}

/*
 * Called from (function) -
 * ------------------------
 *  1. hill_cipher_decrypter()
 *
 * Parameters taken by this function -
 * -----------------------------------
 *  1. x
 *  2. base
 *
 * Description -
 * -------------
 *  This function returns the arithmetic remainder of x / base
 *
 * Returns -
 * ---------
 *  mod of x / base
 */
int calculate_mod(int x, int base) {
	int mod;
	if (x >= 0) {
		mod = x % base;
	} else {
		mod = base + x;        //A little hack.
	}
	return mod;
}

/*
 * Called from (function) -
 * ------------------------
 *  1. hill_cipher_decrypter()
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
