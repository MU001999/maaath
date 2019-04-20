import socket

def test_ConceptMap(s):
	sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
	sock.connect("/tmp/datastructureexpt.socket")
	sock.sendall(s.encode('utf-8'))
	recv = sock.recv(4096)
	sock.close()
	return recv.decode('utf-8')


if __name__ == "__main__":
	print(test_ConceptMap("5记者读报道的来源是呱呱食堂"))
	print(test_ConceptMap("5要长大，要可爱，但不要变成各种奇奇怪怪的不可爱的人"))