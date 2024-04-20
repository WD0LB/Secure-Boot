from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization
import base64

def generate_keys():
    # Generate a private RSA key of 2048 size.
    private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048,
        backend=default_backend()
    )
    print(private_key.private_bytes)
    
    # Serialize the private key to PEM format
    pem = private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption()
    )
    
    # write the private key to a file
    with open('./keys/private_key.pem', 'wb') as f:
        f.write(base64.b64encode(pem))

    # serialize and write the public key
    public_key = private_key.public_key()
    print(public_key.public_bytes)
    pem_public = public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    
    with open('./keys/public_key.pem', 'wb') as f:
        f.write(base64.b64encode(pem_public))
    
if __name__ == "__main__":
    generate_keys()
