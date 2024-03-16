//implemeted playfair cipher with encryption and decryption with a cryptanalytics 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIZE 5

char* generate_playfair_key(char* key) {

    char* key_matrix = (char*)malloc(25 * sizeof(char));
    int index = 0;
    int i, j;
    
    for (i = 0; i < strlen(key); i++) {
        if (key[i] != ' ' && strchr(key_matrix, key[i]) == NULL) {
            if (key[i] != 'J')
                key_matrix[index++] = key[i];
        }
    }
    
    for (i = 'A'; i <= 'Z'; i++) {
        if (i != 'J' && strchr(key_matrix, i) == NULL) {
            key_matrix[index++] = i;
        }
    }
    
    return key_matrix;
}

void find_position(char* matrix, char target, int* row, int* col) {
    int i;
    for (i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
        if (matrix[i] == target) {
            *row = i / MATRIX_SIZE;
            *col = i % MATRIX_SIZE;
            break;
        }
    }
}

void encrypt(char* plaintext, char* key_matrix, char* ciphertext) {
 
    int i, a_row, a_col, b_row, b_col;
    for (i = 0; i < strlen(plaintext); i += 2) {
        char a = plaintext[i];
        char b = plaintext[i + 1];
        find_position(key_matrix, a, &a_row, &a_col);
        find_position(key_matrix, b, &b_row, &b_col);
        if (a_row == b_row) {  
            ciphertext[i] = key_matrix[a_row * MATRIX_SIZE + (a_col + 1) % MATRIX_SIZE];
            ciphertext[i + 1] = key_matrix[b_row * MATRIX_SIZE + (b_col + 1) % MATRIX_SIZE];
        } else if (a_col == b_col) {  
            ciphertext[i] = key_matrix[((a_row + 1) % MATRIX_SIZE) * MATRIX_SIZE + a_col];
            ciphertext[i + 1] = key_matrix[((b_row + 1) % MATRIX_SIZE) * MATRIX_SIZE + b_col];
        } else { 
            ciphertext[i] = key_matrix[a_row * MATRIX_SIZE + b_col];
            ciphertext[i + 1] = key_matrix[b_row * MATRIX_SIZE + a_col];
        }
    }
    ciphertext[i] = '\0';
}

void decrypt(char* ciphertext, char* key_matrix, char* plaintext) {
    
    int i, a_row, a_col, b_row, b_col;
    for (i = 0; i < strlen(ciphertext); i += 2) {
        char a = ciphertext[i];
        char b = ciphertext[i + 1];
        find_position(key_matrix, a, &a_row, &a_col);
        find_position(key_matrix, b, &b_row, &b_col);
        if (a_row == b_row) {  
            plaintext[i] = key_matrix[a_row * MATRIX_SIZE + (a_col - 1 + MATRIX_SIZE) % MATRIX_SIZE];
            plaintext[i + 1] = key_matrix[b_row * MATRIX_SIZE + (b_col - 1 + MATRIX_SIZE) % MATRIX_SIZE];
        } else if (a_col == b_col) {  
            plaintext[i] = key_matrix[((a_row - 1 + MATRIX_SIZE) % MATRIX_SIZE) * MATRIX_SIZE + a_col];
            plaintext[i + 1] = key_matrix[((b_row - 1 + MATRIX_SIZE) % MATRIX_SIZE) * MATRIX_SIZE + b_col];
        } else {  
            plaintext[i] = key_matrix[a_row * MATRIX_SIZE + b_col];
            plaintext[i + 1] = key_matrix[b_row * MATRIX_SIZE + a_col];
        }
    }
    plaintext[i] = '\0';
}

void cryptanalysis(char* ciphertext, char* key_matrix, char* plaintext) {
   
    char temp_key_matrix[25];
    int i, j, k = 0;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            if (key_matrix[i * MATRIX_SIZE + j] != 'I' && key_matrix[i * MATRIX_SIZE + j] != 'J') {
                memcpy(temp_key_matrix, key_matrix, 25 * sizeof(char));
                temp_key_matrix[i * MATRIX_SIZE + j] = 'I';   with 'I'
                decrypt(ciphertext, temp_key_matrix, plaintext);
                printf("Key %d: %s\n", ++k, plaintext);
            }
        }
    }
}

int main() {
    char key[] = "PLAYFAIR EXAMPLE";
    char plaintext[] = "HELLO WORLD";
    char* key_matrix = generate_playfair_key(key);
    char ciphertext[strlen(plaintext) * 2];
    char decrypted_text[strlen(plaintext)];
    
    printf("Key Matrix:\n");
    int i, j;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%c ", key_matrix[i * MATRIX_SIZE + j]);
        }
        printf("\n");
    }
    
    encrypt(plaintext, key_matrix, ciphertext);
    printf("\nCiphertext: %s\n", ciphertext);
    
    decrypt(ciphertext, key_matrix, decrypted_text);
    printf("Decrypted Text: %s\n", decrypted_text);
    
    printf("\nCryptanalysis Attack Results:\n");
    char possible_plaintexts[strlen(plaintext)];
    cryptanalysis(ciphertext, key_matrix, possible_plaintexts);
    
    free(key_matrix);
    return 0;
}
