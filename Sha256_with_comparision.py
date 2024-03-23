import hashlib
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import binascii

secret_key = b'ThisIsASecretKey'
code_s = b'randomcode'

def encrypt_message(message):
    cipher = AES.new(secret_key, AES.MODE_CBC)
    encrypted_message = cipher.encrypt(pad(message.encode(), AES.block_size))
    return cipher.iv + encrypted_message

def decrypt_message(encrypted_message):
    iv = encrypted_message[:AES.block_size]
    cipher = AES.new(secret_key, AES.MODE_CBC, iv)
    decrypted_message = unpad(cipher.decrypt(encrypted_message[AES.block_size:]), AES.block_size)
    return decrypted_message.decode()

def generate_hash(message):
    hash_object = hashlib.sha256()
    hash_object.update(message)
    return hash_object.digest()

def verify_hash(original_hash, received_hash):
    return original_hash == received_hash

def main():
    message = input("Enter your message: ")

    # Bob's operations
    hashed_message = generate_hash(message.encode() + code_s)  # Appending the code_s with message
    encrypted_message = encrypt_message(message)

    print("\nBob (Source A) sends to Alice (Destination B):")
    print("Encrypted message:", binascii.hexlify(encrypted_message))
    print("Hash of message concatenated with code (H(M||s)):", binascii.hexlify(hashed_message))


    # Alice's operations
    decrypted_message = decrypt_message(encrypted_message)
    received_hash = generate_hash(decrypted_message.encode() + code_s)  # Appending the code_s with received message

    print("\nAlice (Destination B) receives from Bob (Source A):")
    print("Encrypted message:", binascii.hexlify(encrypted_message))
    print("Hash of message concatenated with code (H(M||s)):", binascii.hexlify(hashed_message))

    # Verifying the hash
    if verify_hash(hashed_message, received_hash):
        print("\nMessage integrity verified. Original message:", decrypted_message)
    else:
        print("\nMessage integrity compromised. Original message might have been altered.")

if __name__ == "__main__":
    main()
