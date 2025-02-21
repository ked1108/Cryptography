import math

def modInv(e, phiN):
    for d in range(2, phiN):
        if (e * d) % phiN == 1:
            return d

    return -1

def findE(phiN):
    for e in range(2, phiN):
        if math.gcd(e, phiN) == 1:
            return e

    return -1


if __name__ == "__main__":
    p = int(input("Enter p:\t"))
    q = int(input("Enter q:\t"))
    n = p*q
    phiN = (p-1) * (q-1)

    e = findE(phiN)
    assert(e != -1)

    d = modInv(e, phiN)
    assert(d != -1)

    # M = str(input("Enter the Message:\t"))
    M = int(input("Enter the number:\t"))
    # asciiM = list(map(ord, M))
    # print("ASCII Message:\t", asciiM)
    
    C = pow(M, e, n) 
    # C = list(map(lambda X: pow(X, e, n), asciiM))
    print("Encrypted Message:\t", C)

    D = pow(C, d, n)
    # D = list(map(lambda X: pow(X, d, n), C))
    print("Decrypted Message:\t", D)




    pass