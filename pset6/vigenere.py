import sys

def printCipher(text, key):
    encryptedString = '';
    k = 0;
    # for each character check the of upper or lowercase
    for i in range(len(text)):
        # only encrypt letters, other characters remain unchanged
        if text[i].isalpha():
            if text[i].isupper():
                startIndex = ord('A')
            else:
                startIndex = ord('a')
            # cipher is (text[i] - k) mod 26
            encryptedString += chr(startIndex + (ord(text[i]) - startIndex + ord( key[k % len(key)].lower()) - ord('a')) % 26)
        else:
            encryptedString += text[i]
        k += 1
    return encryptedString
        
def main():
    # check if number of arguments is correct
    if len(sys.argv) != 2:
        print("Usage: python vigenere.py k")
        exit(1)
    
    # check if the key only contains characters
    key = sys.argv[1]
    if key.isalpha() != True:
        print("Usage: python vigenere.py k")
        exit(1)
    
    # request the plaintext
    plain = input("plaintext: ")
    
    # print the cipher
    print("ciphertext: ", printCipher(plain, key))
    exit(0)

if __name__ == "__main__":
    main()