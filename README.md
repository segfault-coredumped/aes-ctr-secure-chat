# AES-256-CTR Secure Chat

## Overview
This project implements a simple secure chat encryption and decryption system using AES-256 in CTR mode via the OpenSSL EVP API.

It demonstrates practical symmetric-key cryptography, including nonce generation, stream encryption, and correct usage of OpenSSL’s EVP interface.

## Features
- AES-256 encryption in CTR mode
- Secure random nonce generation
- Stream encryption (no padding required)
- OpenSSL EVP API usage
- Message format with embedded nonce

## Message Format
```
[12-byte nonce | ciphertext]
```

The nonce is stored at the beginning of the ciphertext and reused during decryption to reconstruct the IV.

## Algorithm Details
- Cipher: AES-256
- Mode: CTR (stream cipher mode)
- Key size: 32 bytes
- IV: 12-byte nonce + 4-byte zero counter
- Library: OpenSSL EVP API

CTR mode enables encryption of variable-length messages without padding.

## Functions

### chat_encrypt(k, pt, ptlen, ct)
- Generates a 12-byte random nonce using `RAND_bytes`
- Constructs IV = nonce || counter (0)
- Encrypts plaintext using `EVP_EncryptUpdate`
- Prepends nonce to ciphertext output

### chat_decrypt(k, ct, ctlen, pt)
- Extracts nonce from ciphertext
- Reconstructs IV
- Decrypts ciphertext using `EVP_DecryptUpdate`

## Concepts Used
- AES symmetric encryption
- CTR mode operation
- Secure random nonce generation
- OpenSSL EVP interface
- IV construction and management

## Notes
- Each encryption uses a unique random nonce
- CTR mode does not require padding
- No authentication layer (not AES-GCM)
- Educational implementation for cryptography practice
