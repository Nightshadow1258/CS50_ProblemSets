coins = [0.25, 0.1, 0.05, 0.01]

change = 0.00


def main():
    while True:
        change = float(input("Change: "))
        if change > 0:
            break
    
    counter = 0
    for coin in coins:
        change,counter_new = check_change(change, coin)
        counter += counter_new


    print(counter)

def check_change(change: int, coin_value):
    counter = 0
    while change >= coin_value:
        #print(change)
        change = round(change - coin_value,ndigits=2)
        counter += 1
    return change, counter

main()