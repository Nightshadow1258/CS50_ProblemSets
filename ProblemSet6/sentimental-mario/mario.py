def main():
    while True:
        height = int(input("Height: "))
        if (height > 0 and height < 9):
            break

    for i in range(height):
        print(" " * (height-1-i) + "#" * (1+i))


main()