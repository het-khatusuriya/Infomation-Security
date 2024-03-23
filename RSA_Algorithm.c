#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_PRIME 250

int public_key;
int private_key;
int n;
int primes[MAX_PRIME];
int primes_count = 0;
int prime1, prime2;

void primefiller() {
    bool seive[MAX_PRIME];
    memset(seive, true, sizeof(seive));
    seive[0] = false;
    seive[1] = false;

    for (int i = 2; i < MAX_PRIME; i++) {
        if (seive[i]) {
            primes[primes_count++] = i;
            for (int j = i * 2; j < MAX_PRIME; j += i) {
                seive[j] = false;
            }
        }
    }
}

int pickrandomprime() {
    int k = rand() % primes_count;
    return primes[k];
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void setkeys() {
    prime1 = pickrandomprime();
    prime2 = pickrandomprime();
    n = prime1 * prime2;
    int fi = (prime1 - 1) * (prime2 - 1);
    int e = 2;
    while (1) {
        if (gcd(e, fi) == 1)
            break;
        e++;
    }
    public_key = e;
    int d = 2;
    while (1) {
        if ((d * e) % fi == 1)
            break;
        d++;
    }
    private_key = d;

    printf("p (prime1): %d\n", prime1);
    printf("q (prime2): %d\n", prime2);
    printf("n (public key parameter): %d\n", n);
    printf("Public Key (e): %d\n", public_key);
    printf("Private Key (d): %d\n\n", private_key);
}


long long int encrypt(int message) {
    int e = public_key;
    long long int encrypted_text = 1;
    while (e--) {
        encrypted_text *= message;
        encrypted_text %= n;
    }
    return encrypted_text;
}

long long int decrypt(int encrypted_text) {
    int d = private_key;
    long long int decrypted = 1;
    while (d--) {
        decrypted *= encrypted_text;
        decrypted %= n;
    }
    return decrypted;
}

void encoder(char *message, int *form) {
    int len = strlen(message);
    for (int i = 0; i < len; i++) {
        form[i] = encrypt((int)message[i]);
    }
}

void decoder(int *encoded, char *decoded, int len) {
    for (int i = 0; i < len; i++) {
        decoded[i] = (char)decrypt(encoded[i]);
    }
    decoded[len] = '\0';
}

int main() {
    srand(time(NULL));
    primefiller();
    setkeys();
    char message[100];
    printf("Enter the message: ");
    fgets(message, 100, stdin);
    message[strcspn(message, "\n")] = 0;

    int form[strlen(message)];
    encoder(message, form);
    printf("\nInitial message:\n%s\n\n", message);
    printf("The encoded message(encrypted by public key):\n");
    for (int i = 0; i < strlen(message); i++)
        printf("%d", form[i]);
    printf("\n\n");

    char decoded[strlen(message) + 1];
    decoder(form, decoded, strlen(message));
    printf("The decoded message(decrypted by private key):\n%s\n", decoded);

    return 0;
}
