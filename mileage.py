# Empty list mileage, that will contain all daily miles ran
mileage = []
# tot_mileage ran per day count
tot_mileage = 0

# This takes in input from the user for how many miles they ran each day
daily_miles = input("Enter the number of miles you ran today: " + "If finished inputting, type done.")
# The while loop is used to keep the code within it going until the user inputs done
while daily_miles != 'done':
    # Will append each daily_miles intot he mileage list
    mileage.append(float(daily_miles))
    # Keeps count of the total miles ran
    tot_mileage += float(daily_miles)
    # Prints daily updates on total mileage ran thus far
    print("Your total mileage so far is " + str(tot_mileage))
    # Will ask user for input again at end of loop, and the while loop will check if 'done' was entered to exit the loop
    daily_miles = input("Enter the number of miles you ran today: " + "If finished inputting, type done.")

# While loop exited now, print statement for the next line
print("Here is the amount you ran each day: ")
# for loop will iterate over every entry in the list mileage and print the day num with the mileage ran that day
for i in range(len(mileage) - 1):
        print("Day " + (str(i+1)) + ": " + str(mileage[i]) + " miles")
# Last line prints message with your total mileage ran in the last i days
print("Congrats! Your final total is " + str(tot_mileage) + " miles!")
