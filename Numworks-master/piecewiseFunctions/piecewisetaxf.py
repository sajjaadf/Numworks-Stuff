def taxRateFinder(taxPer,bracket):
    taxRate = []
    for number in range(len(bracket)) :
      taxRate.append((bracket[number] - bracket[number - 1]) * taxPer[number])
      if taxRate[number] != 0 :
        taxRate[number] = taxRate[number] + taxRate[number - 1]
    return taxRate

print("==========")
print("1. Input Income")
print("2. Input Table")
choice = input(":")
if choice == "1" :
    print("What is your income?")
    income = float(input(":"))
    bracket = [0,9525,38700,82500,157500,200000,500000]
    taxPer = [0,.10,.12,.22,.24,.32,.35,.37]
    taxRate = []
    taxRate = taxRateFinder(taxPer,bracket)
    tax = 0
    counter = 0
    while tax == 0 :
      if counter <= 5 :
        if income >= bracket[counter] and income <= bracket[counter+1]:
          tax = (taxPer[counter + 1] * (income - bracket[counter])) + taxRate[counter]
      else:
        tax = (.37 * (income - 500000) + 150689.5)
      counter = counter + 1
    print("Tax:",tax)
    print((tax/income)*100,"%")
if choice == "2" :
    print("a * (x - b) + c")
    print("Multiplier? *a*")
    taxPer = [float(x) for x in input(":").split(",")]
    if taxPer[0] != 0 :
        taxPer.insert(0,0)
    print("Bracket? *b*")
    bracket = [float(x) for x in input(":").split(",")]
    taxRate = taxRateFinder(taxPer,bracket)
    print(taxPer[1],"x   ", "0 < x <",bracket[1])
    bracket.append("Infinity")
    for number in range(len(taxPer)-2) :
        print(taxPer[number+2],"* (x -",bracket[number+1],"+",taxRate[number+1],") |",bracket[number + 1],"< x <",bracket[number + 2])
