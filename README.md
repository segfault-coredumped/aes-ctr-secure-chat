# AES-256-CTR Secure Chat

This project implements a simple secure chat encryption and decryption system using AES-256 in CTR mode via the OpenSSL EVP API.

It demonstrates practical symmetric-key cryptography, including nonce generation, stream encryption, and proper use of OpenSSL’s EVP interface.
___

### Overview

Provides two functions:

- `chat_encrypt()` – Encrypts plaintext using AES-256-CTR  
- `chat_decrypt()` – Decrypts ciphertext back into plaintext  

Each message uses a 12-byte random nonce combined with a 4-byte zero counter to form the IV.

___

### Encryption Format

[12-byte nonce | ciphertext]

The nonce is stored at the beginning of the ciphertext so it can be extracted during decryption.

___

### Algorithm Details

- Cipher: AES-256
- Mode: CTR (stream cipher mode)
- Key size: 32 bytes
- IV: 12-byte nonce + 4-byte counter (0)
- Library: OpenSSL EVP API

CTR mode allows encryption of arbitrary-length messages without padding.

### Functions

### chat_encrypt(k, pt, ptlen, ct)
- Generates a 12-byte random nonce using RAND_bytes
- Builds IV = nonce || counter
- Encrypts plaintext using EVP_EncryptUpdate
- Prepends nonce to ciphertext output

### chat_decrypt(k, ct, ctlen, pt)
- Extracts nonce from ciphertext
- Rebuilds IV
- Decrypts using EVP_DecryptUpdate

___

### Concepts Demonstrated

- AES symmetric encryption
- CTR mode operation
- Secure random nonce generation
- OpenSSL EVP API usage
- IV construction and reuse

___

### Notes

- Each encryption uses a unique random nonce
- CTR mode does not require padding
- No authentication layer (not AES-GCM)
- Educational implementation for cryptography practice