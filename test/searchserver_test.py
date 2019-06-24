import socket
from collections import defaultdict


def test(s):
	sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
	sock.connect("/tmp/datastructureexpt.socket")
	sock.sendall(s.encode('utf-8'))
	recv = sock.recv(8192)
	sock.close()
	recv = recv.decode('utf-8').split('#')
	res = defaultdict(list)
	for i in range(0, len(recv) - 1, 2):
		res[recv[i]].append(recv[i+1])
	return res


if __name__ == "__main__":
	# print(test("5数轴有理数绝对值"))
	# print(test("0数轴有理数绝对值"))
	print(test("0$x-3x+9r=-1701$"));

	print(test("1数轴有理数绝对值"))
	print(test("2数轴有理数绝对值"))
	print(test("3数轴有理数绝对值"))
	print(test("4数轴有理数绝对值"))

	print(test("5数轴有理数绝对值"))
	# print(test("5记者读报道的来源是呱呱食堂"))
	# print(test("5要长大，要可爱，但不要变成各种奇奇怪怪的不可爱的人"))
