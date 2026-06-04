// AES-256-CTR encryption/decryption using OpenSSL EVP API


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// OpenSSL Libraries
#include <openssl/evp.h>
#include <openssl/rand.h>

void chat_encrypt(void *k, void *pt, int ptlen, void *ct) {
    EVP_CIPHER_CTX *ctx; // will hold the ciphertext when generated
    uint8_t nonce[12]; // 12 byte nonce, uint8 = 1 byte of data -> array of 12 bytes
    uint32_t ctr = 0; // 4 byte counter, uint32 = 32 bits, 8 bits per byte
    uint8_t iv[16]; // 16 byte iv, [nonce||ctr]
    int len = 0; // track actual number of bytes written to ct
    int finallen; // Used for API correctness

    // fill nonce with random bytes
    if (1 != (RAND_bytes(nonce, 12))) {
        return;
    }

    // create iv
    // build iv = [nonce||ctr], ctr = 0
    memcpy(iv, nonce,12);
    // append 4 bytes of ctr after 12 bytes of nonce
    memcpy(iv+12, &ctr, 4);

    // store nonce at beginning of ct: during decryption extract 12 bytes from start of ct
    memcpy(ct, nonce, 12);

    // create and initialise context
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        return;
    }
    // initialise encryption operation - aes256 ctr
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, k, iv)) {
        // prevent mem leak
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    // provide message to be encrypted and obtain the encrypted output
    // first 12 bytes are reserved for nonce
    // update len to indicate how many bytes were written
    if (1 != EVP_EncryptUpdate(ctx, ct+12, &len, pt, ptlen)) {
        // prevent mem leak
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    // finalize encryption. further ciphertext bytes may be written at this stage
    // CTR wont output further ciphertext bytes i.e no padding used
    EVP_EncryptFinal_ex(ctx, NULL, &finallen);

    // clean up context
    EVP_CIPHER_CTX_free(ctx);
}

void chat_decrypt(void *k, void *ct, int ctlen, void *pt) {
    EVP_CIPHER_CTX *ctx;
    uint8_t iv[16]; // 16 byte iv, [nonce||ctr]
    uint32_t ctr = 0;
    int len = 0;
    int finallen;

    // rebuild iv used in decryption
    // get the nonce from first 12 bytes of ct and copy it into start of iv
    memcpy(iv, ct, 12);
    // ctr is 0, next 4 bytes after 12 bytes of nonce: [nonce||ctr]
    memcpy(iv+12, &ctr, 4);

    // create/initialise context
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        return;
    }

    // Initialise decryption operation
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, k, iv)) {
        // prevent mem leak
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    // provide message to be decrypted and obtain the decrypted output
    // first 12 bytes are reserved for nonce
    // update len to indicate how many bytes were written
    // skip the nonce when decrypting
    if(1 != EVP_DecryptUpdate(ctx, pt, &len, ct+12, ctlen-12)) {
        // prevent mem leak
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    EVP_DecryptFinal_ex(ctx, NULL, &finallen);

    // clean up context
    EVP_CIPHER_CTX_free(ctx);
}

/*
Test harness (disabled for library-style build)
Enable with: #if 1
*/

#if 0
#include <stdio.h> 
int main() {
    // Write test from your function here
    
    // build 32 byte key
    unsigned char k[32] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                           0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
                           0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
                           0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31
                         
                        };
    // create pt message to be encrypted and decrypted
    unsigned char pt[] = "Testing message for AES256 CTR encryption";
    int ptlen = strlen((char*)pt);

    // create ct buffer
    unsigned char ct[ptlen+12];
    // create buffer to hold decrypted ct message
    unsigned char decrypted_ct[ptlen+1]; // room for null terminator

    // encrypt message into ciphertext
    chat_encrypt(k, pt, ptlen, ct);
    // decrypt ciphertext into message
    chat_decrypt(k, ct, ptlen + 12, decrypted_ct);
    decrypted_ct[ptlen] = '\0'; // add null terminator to the end

    // Original message
    printf(" Original: %s\n", pt);
    // check ciphertext
    printf("Ciphertext: ");
    for (int i = 0; i < ptlen + 12; i++) {
        printf("%02x", ((unsigned char *)ct)[i]);
    }
    printf("\n");
    // check decrypted ciphertext
    printf("Decrypted: %s\n", decrypted_ct);

    return 0;
}
#endif