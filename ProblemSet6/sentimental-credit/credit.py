def main():
    cc_number = int(input("Number: "))
    check_cc_number(cc_number)




def check_cc_number(number : int):

    #check if number length is okay
    if number < 0 or number > 9999999999999999:
        print("INVALID")
        return
    
    #apply Luhn's algorithm
    # get every second digit of the number
    even = 0
    temp = 0
    for i in range(0,len(str(number)),2):
        temp = get_digit(number,i)
        even += temp


    odd = 0

    for i in range(0,len(str(number)),2):
        temp = 2* get_digit(number/10,i)
        if temp >= 10:
            odd += get_digit(temp,0)
            odd += get_digit(temp,1)
            temp = 0
        odd += temp

    if (odd+even)%10 == 0:
        if get_digit(number, 14)  == 3:
            print("AMEX")
            return
        if get_digit(number, 15) == 5:
            print("MASTERCARD")
            return
        if get_digit(number, 12) == 4 or get_digit(number, 15) == 4:
            print("VISA")
            return

    print("INVALID")

def get_digit(number, n):
    return number // 10**n % 10


main()