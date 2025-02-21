
if __name__ == "__main__":
    alpha = int(input("Enter ɑ:\t"))
    q = int(input("Enter q:\t"))

    Xa = int(input("Enter public key of A:"))
    Xb = int(input("Enter public key of B:"))

    Ya = pow(alpha, Xa, q)
    Yb = pow(alpha, Xb, q)

    print("Ya:\t", Ya)
    print("Yb:\t", Yb)

    Ka = pow(Yb, Xa, q)
    Kb = pow(Ya, Xb, q)


    print("Ka:\t", Ka)
    print("Kb:\t", Kb)

    assert(Kb == Ka)
    print("Match!")

