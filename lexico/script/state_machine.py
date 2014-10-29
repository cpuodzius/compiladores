def main(argv=None):
	fin = open('state_machine.csv').readlines()[1:]
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
	fout = open('include/state_machine.h', 'w')
	fout.write(output)
	fout.close()

if __name__ =='__main__':main()

