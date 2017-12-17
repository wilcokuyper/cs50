def main():
    # query tower height, request again if not in range [0,23]
    h = -1
    while h < 0 or h > 23:
        try:
            h = int(input("Height:"))
        except ValueError:
            pass

    printPyramide(h)
    exit(0)
    
def printPyramide(height):
    for r in range (1, height+1):
        printLine(r, height)
        
def printLine(row, height):
    # calculate number of spaces for this row and print the lines: spaces - hashes - two spaces - hashes - spaces
    spaces = height - row
    printChar(" ", spaces)
    printChar("#", row)
    printChar(" ", 2)
    printChar("#", row)
    print("\r")

def printChar(c, count):
    #helper function for printing a character multiple times
    for i in range(count):
        print(c, end='')

if __name__ == "__main__":
    main()