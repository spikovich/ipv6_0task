import socket
import sys

def str_to_upper(s):
    return s.upper()

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <socket_name>")
        sys.exit(1)

    socket_name = sys.argv[1]

    try:
        # Create a socket
        server_sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

        # Remove old socket if it exists
        try:
            os.unlink(socket_name)
        except OSError:
            if os.path.exists(socket_name):
                raise

        # Bind the socket to the address
        server_sock.bind(socket_name)

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
