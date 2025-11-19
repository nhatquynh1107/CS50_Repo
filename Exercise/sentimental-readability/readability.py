from cs50 import get_string

def main():
    text = get_string("Text: ")
    lter = 0
    stence = 0
    word = 1

    for c in text:
        if c.isalpha():
            lter += 1
        if c in [".", "?", "!"]:
            stence += 1
        if c == " ":
            word += 1

    L = (lter / word) * 100
    S = (stence / word) * 100
    idx = 0.0588 * L - 0.296 * S - 15.8
    ans = round(idx)
    
    if ans < 1:
        print("Before Grade 1")
    elif ans >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {ans}")

main()

















    