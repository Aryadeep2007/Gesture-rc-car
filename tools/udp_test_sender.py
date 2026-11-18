import socket

CAR_IP = "192.168.1.236"   # change if your board's IP differs
CAR_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(2)

for msg in ["hello", "F,150", "S,0"]:
    sock.sendto(msg.encode(), (CAR_IP, CAR_PORT))
    print("Sent:", msg)
    try:
        data, _ = sock.recvfrom(64)
        print("Reply:", data.decode())
    except socket.timeout:
        print("No reply (timeout)")