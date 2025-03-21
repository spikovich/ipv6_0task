import os
import random
import sys

def custom_rand(seed):
    a = 1664525
    c = 1013904223
    m = 4294967296  # 2^32
    seed = (a * seed + c) % m
    return seed

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <filename> <message>")
        sys.exit(1)

    filename = sys.argv[1]
    message = sys.argv[2]

    try:
        with open(filename, 'a') as file:
            file_size = os.path.getsize(filename)
            seed = os.getpid()
            random_value = custom_rand(seed) % 1000 + 1
            if file_size > 0:
                file.write(f"\n{random_value} {message}")
            else:
                file.write(f"{random_value} {message}")
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
