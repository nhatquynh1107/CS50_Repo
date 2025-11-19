def main():
    while True:
        try:
            x = float(input("Change: "))
            if x > 0:
                break
        except ValueError:
            continue
    n = round(x * 100)
    cnt = 0
    a = [25, 10, 5, 1]
    for i in a:
        cnt += n // i
        n = n % i
    print(cnt)
main()

