import os
import subprocess

def long_chain(write,direction,l,pre,end_state):
	return [f'{pre}{i},{r}: {write(i)} {direction(i)} {pre+str(i+1) if i!=l-1 else end_state}' for r in range(2) for i in range(l)]

def make_tape(target,left_limit, right_limit, left_init, rigt_init):
    compile_proc = subprocess.run(['./../bin/tape_tool','new', target,str(left_limit), str(right_limit), str(left_init), str(rigt_init)], text=True, capture_output=True)
    if compile_proc.returncode != 0:
        print("tape making Failed:")
        print(compile_proc.stderr)
        return

if __name__=="__main__":
	#os.mkdir("tasks")

	os.makedirs("tasks/100")
	with open('tasks/100/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R',100,'S','halt')))
	make_tape('tasks/100/input.tape',-200,200,-200,200)

	os.makedirs("tasks/69")
	with open('tasks/69/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R',69,'S','halt')))
	make_tape('tasks/69/input.tape',-100,100,-10,10)

	os.makedirs("tasks/42")
	with open('tasks/42/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R' if i%3 else 'S',42,'S','halt')))
	make_tape('tasks/42/input.tape',-100,100,-10,10)

	os.makedirs("tasks/1000")
	with open('tasks/1000/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R' if i%2 else 'L',1000,'S','halt')))
	make_tape('tasks/1000/input.tape',-10,10,-10,10)