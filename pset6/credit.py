def sumOfDigits(number):
    sum = 0
    length = len(number)
    val = int(number)
    # creditcard is only valid if it contains 13, 15 or 16 digits
    if length == 13 or length == 15 or length == 16:
        # calculate the sum of the digits by adding the individual numbers to each other, every other number should also be doubled
        for i in range(1,length+1):
            # find the current digits
            currentDigit = int(val % 10);
            # nth mod 2 digits should be doubled
            if i % 2 == 0:
                currentDigit *= 2;
                if currentDigit > 9:
                    sum += int(currentDigit % 10) + 1
                else:
                    sum += currentDigit
            else:
                sum += currentDigit
            val = int(val / 10)
    return sum;
    
def typeOfCard(number):
    lastTwoDigits = int(int(number) / (pow(10.0, len(number)-2) ))
    if lastTwoDigits == 34 or lastTwoDigits == 37:
        return "AMEX"
    elif (lastTwoDigits == 51 or
            lastTwoDigits == 52 or 
            lastTwoDigits == 53 or 
            lastTwoDigits == 54 or 
            lastTwoDigits == 55):
        return "MASTERCARD"
    elif int(lastTwoDigits / 10) == 4:
        return "VISA"
    else:
        return "INVALID"

def printCardType(number):
    # calculatie the sum of the creditcard digits, if it is positive and divisible by 10 it is valid
    sum = sumOfDigits(number)
    if sum > 0 and sum % 10 == 0:
        print(typeOfCard(number))
    else:
        print("INVALID")

def main():
    try:
        number = input("Number: ")
        printCardType(number)
        exit(0)
    except ValueError:
        print("INVALID")
        exit(1)

if __name__ == "__main__":
    main()