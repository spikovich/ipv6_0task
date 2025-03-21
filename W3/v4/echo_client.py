import socket
import sys

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <server_ip> <message>")
        sys.exit(1)

    server_ip = sys.argv[1]
    message = sys.argv[2]

    try:
        # Create a socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Connect to the server
        server_address = (server_ip, 5000)
        sock.connect(server_address)

        # Send the message
        print(f"client sending message: {message}")
        sock.sendall(message.encode('utf-8'))

        # Receive the response
        response = sock.recv(1024).decode('utf-8')
        print(f"client received reply from server: {response}")

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
    finally:
        sock.close()

if __name__ == "__main__":
    main()
