import os
import sys

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <filename> <message>")
        sys.exit(1)

    filename = sys.argv[1]
    message = sys.argv[2]

    try:
        with open(filename, 'w') as file:
            file.write(message)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
