import socket

def InitSockets(port):
    host = socket.gethostname()
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s_ack = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((host, port))
        s_ack.connect((host, port + 1))
    except Exception as e:
        raise e
        return
    return s, s_ack

def GetDataFromPort(s):
    data = s.recv(1024)
    return data

def SendAck(s_ack):
    ack = bytes([0x97, 0x98, 0x99, 0x42])
    s_ack.send(ack)


