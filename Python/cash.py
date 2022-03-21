import cs50
import sys

while True:
    change = cs50.get_float("change owed: ")
    if change > 0: 
        break
change *= 100
#compute quarters 
coins = 0 

if change >= 25:
    quarters = int(change / 25)
    change = change % 25 
    coins += quarters


if change >= 10:
    coins += int(change / 10)
    change %= 10


if change >= 5: 
    coins += 1
    change -= 5

coins += change 
print(int(coins))


