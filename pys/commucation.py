from enum import Enum


class TYPE(Enum):
	Ariticle = 0
	Definition = 1
	Property = 2
	Example = 3
	Exercise = 4


class CommProtocol(object):
	
	def __init__(self, type_or_raw, content = None):
		if content is None:
			self.type = int(type_or_raw[0])
			self.content = type_or_raw[1:]
		else:
			self.type = type_or_raw
			self.content = content

	def to_string(self):
		return str(self.type) + str(self.content)
