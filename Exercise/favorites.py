import csv
with open(" ", "r") as file:
    reader = csv.DictReader(file)
    counts = {}
    for row in reader:
        favorite = row("language")
        if favorite in counts:
            counts[favorite] += 1
        else:
            counts[favorite] = 1
for favorite in sorted(counts, key = counts.get, reverse= True): # large to small if sont have reverse it will small to large
    print(f"{favorite}") # sorted usually by alphabet but key = counts.get mean the varible inside the counts
