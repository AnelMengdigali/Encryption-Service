#include <stdio.h>
#include <string.h>

//The function that fill character arrays in two ways
void fillkeysarrays(FILE *keyfile, char keysEnc[], char keysDec[]) {

	int index = 0; //Declaration with initialization since 0 here it is starting point

	//Reading the first character from a file to start while loop (subsequently, the same operation will do this kind of code)
	char keychar = fgetc(keyfile);

	//Working with the file until the end of it (subsequently, the same operation will do this kind of code)
	while (keychar != EOF) {

		//arrays of char taking values that are characters from the key.254 and their indexes are integers rising from 0 to 253
		keysEnc[index] = keychar;

		//arrays of char taking inversely characters from the key.254 as its indexes and values that are those above indexes
		keysDec[(unsigned char) keychar] = index;

		//Integers rise by one each iteration
		index++;

		//Similarly, the characters from the file goes one by one (subsequently, the same operation will do this kind of code)
		keychar = fgetc(keyfile);

	}

}

//The function that does encryption process
void encrypt(FILE *fin, FILE *fout, char subarr[], char pass[]) {

	int passindex = 0; //Declaration with initialization so that it will operate a condition in if statement
	int index;

	int passlength = strlen(pass);

	char filechar = fgetc(fin);

	while (filechar != EOF) {

		//As the end of the password is reached, it will start over
		if (passindex >= passlength) {

			passindex = 0;

		}

		/*
		 * Getting the character from the provided file and
		 * adding it by order to the corresponding character from the password,
		 * then taking modulo 254 (which is the number of characters in key.254)
		 */
		index = (filechar + pass[passindex]) % 254;

		/*
		 *  Using the above obtained number as an index for char arrays,
		 *  that was filled before for encryption process in fillkeysarrays function,
		 *  in order to find the encrypted character,
		 *  then the gained character is printed into the encrypted file
		 */
		fputc(subarr[(unsigned char) index], fout);

		passindex++;

		filechar = fgetc(fin);

	}

}

//The function that does decryption process
void decrypt(FILE *fin, FILE *fout, char subarr[], char pass[]) {

	//Below used codes are play the same role as they do in the encryption process (encrypt function)

	int passindex = 0;
	int index;

	int passlength = strlen(pass);

	char filechar = fgetc(fin);

	while (filechar != EOF) {

		if (passindex >= passlength) {

			passindex = 0;

		}

		index = (subarr[(unsigned char) filechar] - pass[passindex]) % 254;

		fputc(index, fout);

		passindex++;

		filechar = fgetc(fin);

	}

}

//Starting point of the program
int main() {

	setvbuf(stdout, NULL, _IONBF, 0);

	char password[256];
	char command[256];
	char encryptfile[256];
	char encryptedfile[256];
	char decryptedfile[256];

	char keysEnc[254];
	char keysDec[254];

	int index;

	//Opening and checking for validity a file (subsequently, the same operation will do this kind of code)
	FILE *keyfile;

	keyfile = fopen("key.254", "r");

	if (keyfile == NULL) {

		printf("CANNOT open key.254\n");

		return 1;

	}

	/*
	 * Calling a function that reads the key.254 file and fills two arrays of characters in different ways,
	 * by taking characters from it,
	 * for encryption and then decryption operations respectively
	 */
	fillkeysarrays(keyfile, keysEnc, keysDec);

	//Closing the file as the work is done (subsequently, the same operation will do this kind of code)
	fclose(keyfile);

	//A beginning of a service
	printf("WELCOME to the encryption and decryption service.\n");

	//Reading the input and saving it for further use (subsequently, the same operation will do this kind of code)
	printf("ENTER your password\n");
	fgets(password, 256, stdin);

	//Identifying the length of the input, here it is password, until a null character (subsequently, the same operation will do this kind of code)
	index = strlen(password);

	//This code gets rid of a new line, so that it will not affect a following operations (subsequently, the same operation will do this kind of code)
	if (password[index - 1] == '\n') {

		password[index - 1] = '\0';

	}

	//The main body of encryption-decryption system

	do {
		printf("MENU:<e>ncode, <d>ecode, or <q>uit\n");
		fgets(command, 256, stdin);

		//Looking at only the first character of the input to identify the command to operate
		if (command[0] == 'e') {

			printf("ENTER a file to encrypt\n");
			fgets(encryptfile, 256, stdin);

			index = strlen(encryptfile);

			if (encryptfile[index - 1] == '\n') {

				encryptfile[index - 1] = '\0';

			}

			FILE *refile;

			refile = fopen(encryptfile, "r");

			if (refile == NULL) {

				printf("CANNOT open %s\n", encryptfile);

				continue;

			}

			printf("Enter a filename for the encrypted file.\n");
			fgets(encryptedfile, 256, stdin);

			index = strlen(encryptedfile);

			if (encryptedfile[index - 1] == '\n') {

				encryptedfile[index - 1] = '\0';

			}

			FILE *wefile;

			wefile = fopen(encryptedfile, "w");

			if (wefile == NULL) {

				printf("CANNOT open %s\n", encryptedfile);

				continue;

			}

			//Calling a function that does encryption process
			encrypt(refile, wefile, keysEnc, password);

			fclose(refile);
			fclose(wefile);

		}

		if (command[0] == 'd') {

			printf("ENTER a file to decrypt\n");
			fgets(encryptedfile, 256, stdin);

			index = strlen(encryptedfile);

			if (encryptedfile[index - 1] == '\n') {

				encryptedfile[index - 1] = '\0';

			}

			FILE *rdfile;

			rdfile = fopen(encryptedfile, "r");

			if (rdfile == NULL) {

				printf("CANNOT open %s\n", encryptedfile);

				continue;

			}

			printf("Enter a filename for the decrypted file.\n");
			fgets(decryptedfile, 256, stdin);

			index = strlen(decryptedfile);

			if (decryptedfile[index - 1] == '\n') {

				decryptedfile[index - 1] = '\0';

			}

			FILE *wdfile;

			wdfile = fopen(decryptedfile, "w");

			if (wdfile == NULL) {

				printf("CANNOT open %s\n", decryptedfile);

				continue;

			}

			//Calling a function that does decryption process
			decrypt(rdfile, wdfile, keysDec, password);

			fclose(rdfile);
			fclose(wdfile);

		}

		//When the user uses other commands that are not included in the menu
		if (command[0] != 'e' && command[0] != 'd' && command[0] != 'q') {

			printf("UNRECOGNIZED %c\n", command[0]);

		}

	} while (command[0] != 'q');

	//Operated as soon as the command 'q' is entered
	printf("BYE!\n");

	return 0;
}

