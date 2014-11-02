def reduzir_regra(n_term, regra):
	delimitadores = [' ', '"', '(', ')', '{', '}', '[', ']']
	index = 0
	while regra.find(n_term["n_term"], index) != -1:
		index = regra.find(n_term["n_term"], index)
		if regra[:index].count('"') % 2 == 0:
			if index or regra[index - 1] in delimitadores:
				if regra[index + len(n_term["n_term"])] in delimitadores:
					regra = regra[:index] + "(" + n_term["regra"][:-1] + ")" + regra[(index + len(n_term["n_term"])):]

def is_recursivo(n_term):
	is_recursivo = False
	delimitadores = [' ', '"', '(', ')', '{', '}', '[', ']']
	index = n_term["regra"].find(term["n_term"])
	if index >= 0:
		if n_term["regra"][index - 1] in delimitadores and n_term["regra"][index + len(term["n_term"])] in delimitadores:
			is_recursivo = True
	return is_recursivo

def reduzir(gram):
	for i in range(len(gram)):
	current = gram[-1]
	for n_term in gram:
		if(n_term != current):
			reduzir_regra(current, n_term["regra"])
	#TODO: verificar se current eh auto-recursivo
	return gram

def parse_regra_wirth(line):
	n_term = {"n_term": None, "regra": None}
	equal_index = line.find("=")
	n_term["n_term"] = line[:equal_index].strip()
	n_term["regra"] = line[equal_index + 1:].strip()
	return n_term

def nao_terminal_essencial():
	gram = []
	for line in open("wirth.txt").readlines():
		gram.append(parse_regra_wirth(line))
	for n_term in gram:
		print n_term["n_term"] + "    ->     " + n_term["regra"]
	return gram

def main(argv=None):
	gram = nao_terminal_essencial()
	reduzir(gram)

if __name__ =='__main__':main() 
