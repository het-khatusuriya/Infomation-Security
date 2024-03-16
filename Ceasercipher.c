//implemented Ceaser Cipher that performs encryption and descryption with Brutefore and frequency analysis attacks 
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void caesar_encrypt(char plaintext[], int shift) {
    int i;
    for (i = 0; i < strlen(plaintext); i++) {
        if (isalpha(plaintext[i])) {
            if (isupper(plaintext[i])) {
                plaintext[i] = ((plaintext[i] - 'A' + shift) % 26) + 'A';
            } else {
                plaintext[i] = ((plaintext[i] - 'a' + shift) % 26) + 'a';
            }
        }
    }
}

void caesar_decrypt(char ciphertext[], int shift) {
    int i;
    for (i = 0; i < strlen(ciphertext); i++) {
        if (isalpha(ciphertext[i])) {
            if (isupper(ciphertext[i])) {
                ciphertext[i] = ((ciphertext[i] - 'A' - shift + 26) % 26) + 'A';
            } else {
                ciphertext[i] = ((ciphertext[i] - 'a' - shift + 26) % 26) + 'a';
            }
        }
    }
}

void frequency_analysis(char ciphertext[]) {
    int frequencies[26] = {0};
    int i, total_chars = 0;

    for (i = 0; i < strlen(ciphertext); i++) {
        if (isalpha(ciphertext[i])) {
            total_chars++;
            if (isupper(ciphertext[i])) {
                frequencies[ciphertext[i] - 'A']++;
            } else {
                frequencies[ciphertext[i] - 'a']++;
            }
        }
    }

    printf("Frequency Analysis:\n");
    for (i = 0; i < 26; i++) {
        if (frequencies[i] > 0) {
            printf("%c: %.2f%%\n", 'A' + i, (float)frequencies[i] / total_chars * 100);
        }
    }
}

void brute_force_attack(char ciphertext[]) {
    int shift;
    printf("Brute Force Attack:\n");
    for (shift = 1; shift < 26; shift++) {
        printf("Shift %d: ", shift);
        caesar_decrypt(ciphertext, shift);
        printf("%s\n", ciphertext);
    }
}

int main() {
    char plaintext[100], ciphertext[100];
    int shift;

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // remove newline character

    printf("Enter shift: ");
    scanf("%d", &shift);

    strcpy(ciphertext, plaintext);
    caesar_encrypt(ciphertext, shift);
    printf("Encrypted: %s\n", ciphertext);

    printf("Decrypted: ");
    caesar_decrypt(ciphertext, shift);
    printf("%s\n", ciphertext);

    frequency_analysis(ciphertext);

    brute_force_attack(ciphertext);

    return 0;
}
