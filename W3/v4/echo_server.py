import socket
import sys

def str_to_upper(s):
    return s.upper()

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <server_ip>")
        sys.exit(1)

    server_ip = sys.argv[1]

    try:
        # Create a socket
        server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Bind the socket to the address
        server_address = (server_ip, 5000)
        server_sock.bind(server_address)

        # Listen for incoming connections
        server_sock.listen(5)
        print("Server waiting.")

        while True:
            # Accept a connection
            client_sock, client_address = server_sock.accept()
            try:
                # Receive the message
                message = client_sock.recv(1024).decode('utf-8')
                print(f"Server received: {message}")

                # Convert the message to uppercase
                response = str_to_upper(message)

                # Send the response back to the client
                client_sock.sendall(response.encode('utf-8'))
            finally:
                client_sock.close()

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
    finally:
        server_sock.close()

if __name__ == "__main__":
    main()
