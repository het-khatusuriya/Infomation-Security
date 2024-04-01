from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.PublicKey import RSA
from Crypto.Random import get_random_bytes
import hashlib

class KeySharingServer:
    def __init__(self):
        self.private_key = RSA.generate(2048)
        self.public_key = self.private_key.publickey()

    def generate_session_key(self):
        return get_random_bytes(32)  # AES-256 bit key size

    def encrypt_session_key(self, session_key, recipient_public_key):
        cipher_rsa = PKCS1_OAEP.new(recipient_public_key)
        encrypted_session_key = cipher_rsa.encrypt(session_key)
        return encrypted_session_key

    def decrypt_session_key(self, encrypted_session_key):
        cipher_rsa = PKCS1_OAEP.new(self.private_key)
        session_key = cipher_rsa.decrypt(encrypted_session_key)
        return session_key

class Node:
    def __init__(self, name, key_sharing_server):
        self.name = name
        self.key_sharing_server = key_sharing_server
        self.session_key = None

    def receive_session_key(self, encrypted_session_key):
        decrypted_session_key = self.key_sharing_server.decrypt_session_key(encrypted_session_key)
        self.session_key = decrypted_session_key

    def encrypt_message(self, message):
        cipher_aes = AES.new(self.session_key, AES.MODE_EAX)
        ciphertext, tag = cipher_aes.encrypt_and_digest(message.encode('utf-8'))
        return ciphertext, cipher_aes.nonce, tag

    def decrypt_message(self, ciphertext, nonce, tag):
        cipher_aes = AES.new(self.session_key, AES.MODE_EAX, nonce=nonce)
        decrypted_message = cipher_aes.decrypt_and_verify(ciphertext, tag)
        return decrypted_message.decode('utf-8')

# Scenario setup
key_sharing_server = KeySharingServer()
alice = Node("Alice", key_sharing_server)
bob = Node("Bob", key_sharing_server)

# Key sharing
session_key = key_sharing_server.generate_session_key()
encrypted_session_key_for_alice = key_sharing_server.encrypt_session_key(session_key, alice.key_sharing_server.public_key)
encrypted_session_key_for_bob = key_sharing_server.encrypt_session_key(session_key, bob.key_sharing_server.public_key)

alice.receive_session_key(encrypted_session_key_for_alice)
bob.receive_session_key(encrypted_session_key_for_bob)

# Communication
message_to_bob = "Hello Bob, this is Alice!"
ciphertext, nonce, tag = alice.encrypt_message(message_to_bob)
decrypted_message = bob.decrypt_message(ciphertext, nonce, tag)

print(f"Alice sent to Bob: {message_to_bob}")
print(f"Bob received from Alice: {decrypted_message}")
