def get_char_sets():
	digits = range(ord('0'), ord('9'))
	_lambda = []
	for c in range(ord('a'), ord('z')):
		_lambda.append(c)
	for c in range(ord('A'), ord('Z')):
		_lambda.append(c)
	_lambda.append(ord('_'))
	Lambda = []
	for c in _lambda:
		Lambda.append(c)
	for c in digits:
		_lambda.append(c)
	_phi = [ord(' '), ord('('), ord(')'), ord('{'), ord('}'), ord('['), ord(']'), ord('\t'), ord('\n')]
	Phi = [ord('='), ord('<'), ord('>'), ord('+'), ord('-'), ord('/'), ord('*')]
	for c in _phi:
		Phi.append(c)
	Psi = [ord('!'), ord('?'), ord('.'), ord(','), ord(';'), ord(':'), ord('#'), ord('%'), ord('@')]
	return digits, _lambda, Lambda, _phi, Phi, Psi

def generate_state_machine():
	digits, _lambda, Lambda, _phi, Phi, Psi = get_char_sets() 
	state_machine = []
	for state in range(23):
		transitions = []
		for c in range(128):
			if state == 0:
				if c == ord('-'):
					transitions.append(1)
				elif c in digits:
					transitions.append(2)
				elif c == ord('('):
					transitions.append(5)
				elif c == ord(')'):
					transitions.append(6)
				elif c == ord('{'):
					transitions.append(7)
				elif c == ord('}'):
					transitions.append(8)
				elif c == ord('['):
					transitions.append(9)
				elif c == ord(']'):
					transitions.append(10)
				elif c == ord('"'):
					transitions.append(11)
				elif c in _lambda:
					transitions.append(13)
				elif c == ord('+'):
					transitions.append(14)
				elif c == ord('*'):
					transitions.append(16)
				elif c == ord('/'):
					transitions.append(17)
				elif c == ord('<'):
					transitions.append(18)
				elif c == ord('='):
					transitions.append(20)
				elif c == ord('>'):
					transitions.append(22)
				else:
					transitions.append(0)
			elif state == 1:
				if c in digits:
					transitions.append(2)
				elif c == ord('-'):
					transitions.append(4)
				else:
					transitions.append(0)
			elif state == 2:
				if c in digits:
					transitions.append(2)
				elif c == ord('.'):
					transitions.append(3)
				else:
					transitions.append(0)
			elif state == 3:
				if c in digits:
					transitions.append(3)
				else:
					transitions.append(0)
			elif state == 11:
				if c in Lambda + Phi + Psi:
					transitions.append(11)
				elif c == ord('"'):
					transitions.append(12)
				else:
					transitions.append(0)
			elif state == 13:
				if c in Lambda:
					transitions.append(13)
				else:
					transitions.append(0)
			elif state == 14:
				if c == ord('+'):
					transitions.append(15)
				else:
					transitions.append(0)
			elif state == 18:
				if c == ord('='):
					transitions.append(19)
				else:
					transitions.append(0)
			elif state == 20:
				if c == ord('>'):
					transitions.append(21)
				else:
					transitions.append(0)
			elif state == 22:
				if c == ord('='):
					transitions.append(23)
				else:
					transitions.append(0)
			else:
				transitions.append(0)
		state_machine.append(transitions)
	return state_machine

def get_tokens_final_states():
	tokens = []
	for state in range(23):
		if state == 1:
			tokens.append(("T_OPERADOR", "SUBTRACAO"))
		elif state == 2:
			tokens.append(("T_INT", None))
		elif state == 3:
			tokens.append(("T_REAL", None))
		elif state == 4:
			tokens.append(("T_OPERADOR", "DECREMENTO"))
		elif state == 5:
			tokens.append(("T_DELIMITADOR", "PARENTESES_ABRE"))
		elif state == 6:
			tokens.append(("T_DELIMITADOR", "PARENTESES_FECHA"))
		elif state == 7:
			tokens.append(("T_DELIMITADOR", "CHAVES_ABRE"))
		elif state == 8:
			tokens.append(("T_DELIMITADOR", "CHAVES_FECHA"))
		elif state == 9:
			tokens.append(("T_DELIMITADOR", "COLCHETES_ABRE"))
		elif state == 10:
			tokens.append(("T_DELIMITADOR", "COLCHETES_FECHA"))
		elif state == 12:
			tokens.append(("T_TEXT", None))
		elif state == 13:
			tokens.append(("T_RESERVADO", None))
		elif state == 14:
			tokens.append(("T_OPERADOR", "SOMA"))
		elif state == 15:
			tokens.append(("T_OPERADOR", "INCREMENTO"))
		elif state == 16:
			tokens.append(("T_OPERADOR", "MULTIPLICACAO"))
		elif state == 17:
			tokens.append(("T_OPERADOR", "DIVISAO"))
		elif state == 18:
			tokens.append(("T_COMPARACAO", "MENOR"))
		elif state == 19:
			tokens.append(("T_COMPARACAO", "MENOR_OU_IGUAL"))
		elif state == 20:
			tokens.append(("T_COMPARACAO", "IGUAL"))
		elif state == 21:
			tokens.append(("T_ATRIBUICAO", None))
		elif state == 22:
			tokens.append(("T_COMPARACAO", "MAIOR"))
		elif state == 23:
			tokens.append(("T_COMPARACAO", "MAIOR_OU_IGUAL"))
		else:
			tokens.append(("T_NONE", None))
	return tokens

def generate_header_file_cvs(fname):
	fin = open(fname).readlines()[1:]
	output = "int state_machine[" + str(len(fin)) + "][128] = {\n"
	state = 0
	for line in fin:
		cnt = 0
		output += "\t{"
		for next_state in line.split(',')[1:]:
			cnt += 1
			if not next_state or next_state == '\n':
				next_state = str(cnt);
			output += next_state + ", "
		output = output[:-3] + "},\n"
		state += 1
	output = output[:-2] + "\n};"
	fout = open('state_machine.h', 'w')
	fout.write(output)
	fout.close()

def generate_header_file(state_machine, tokens_states):
	output = "#ifndef __MACHINE_STATE_\n"
	output += "#define __MACHINE_STATE_\n\n"
	output += "#include \"lexico.h\"\n\n"
	output += "int state_machine[" + str(len(state_machine)) + "][128] = {\n"
	for state in state_machine:
		output += "\t{"
		for transition in state:
			output += str(transition) + ", "
		output = output[:-2] + "},\n"
	output = output[:-2] + "\n};\n\n"
	output += "int token_tipo[" + str(len(tokens_states)) + "]  = {\n"
	for tipo in [token[0] for token in tokens_states]:
		output += "\t" + tipo + ",\n"
	output = output[:-2] + "\n};\n\n"
	output += "int token_valor[" + str(len(tokens_states)) + "]  = {\n"
	for valor in [token[1] for token in tokens_states]:
		if valor:
			output += "\t" + valor + ",\n"
		else:
			output += "\t0,\n"
	output = output[:-2] + "\n};\n\n"
	output += "#endif // __MACHINE_STATE_"
	fout = open('state_machine.h', 'w')
	fout.write(output)
	fout.close()

def main(argv=None):
	state_machine = generate_state_machine()
	tokens_states = get_tokens_final_states()
	generate_header_file(state_machine, tokens_states)

if __name__ =='__main__':main()

