ignores=[';',',','"','\'','\t',' ','(',')','[',']','{','}']

class result:
	def __init__(self,ans,good=False):
		self.ans=ans
		self.good=good

def make_res(t):
	parts=t.split(':')
	if(len(parts)!=2):
		return result(f"wrong sep count{len(parts)}"+t)

	left=[x for x in parts[0].split(' ') if x]
	if(len(left)!=2):
		return result(f"wrong left count{len(left)}"+t)
	right=[x for x in parts[1].split(' ') if x]
	if(len(right)!=3):
		return result(f"wrong right count{len(right)}"+t)

	return(result([left,right],True))

def split_parts(text:str):
	for char in ignores:
		text = text.replace(char, ' ')
	lines=[l.split('#')[0] for l in text.split('\n')]
	return[(i+1,make_res(t)) for i,t in enumerate(lines) if t]


if __name__=="__main__":
	with open("syntax_examples/first.t") as f:
		text=f.read()
	for i,t in split_parts(text):
		if(not t.good):
			pre='ERROR'
		else:
			pre='YAY'
		print(f'{pre} at line[{i}]: {t.ans}')
