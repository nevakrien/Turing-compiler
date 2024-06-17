import os
import subprocess
import shutil

def long_chain(write,direction,l,pre,end_state):
	return [f'{pre}{i},{r}: {write(i)} {direction(i)} {pre+str(i+1) if i!=l-1 else end_state}'  for i in range(l) for r in range(2)]

def make_tape(target,left_limit, right_limit, left_init, rigt_init):
    compile_proc = subprocess.run(['./../bin/tape_tool','new', target,str(left_limit), str(right_limit), str(left_init), str(rigt_init)], text=True, capture_output=True)
    if compile_proc.returncode != 0:
        print("tape making Failed:")
        print(compile_proc.stderr)
        return

def main():
	#os.mkdir("tasks")

	if(os.path.exists('no_halt')):
	    shutil.rmtree('no_halt')

	if(os.path.exists('tasks')):
	    shutil.rmtree('tasks')

	if(os.path.exists('out_of_tape')):
	    shutil.rmtree('out_of_tape')

	os.makedirs("tasks/dead")
	with open('tasks/dead/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R',30,'S','Y0')+long_chain(lambda i: i%2,lambda i: 'R',10,'DEAD','halt')+long_chain(lambda i: i%2,lambda i: 'L',50,'Y','halt')))
	make_tape('tasks/dead/input.tape',-200,200,-200,200)
		

	os.makedirs("tasks/100")
	with open('tasks/100/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R',100,'S','halt')))
	make_tape('tasks/100/input.tape',-100,100,-1,10)

	os.makedirs("tasks/69")
	with open('tasks/69/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'L',69,'S','halt')))
	make_tape('tasks/69/input.tape',-100,100,-10,1)

	os.makedirs("tasks/42")
	with open('tasks/42/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R' if i%3 else 'S',42,'S','halt')))
	make_tape('tasks/42/input.tape',-100,100,-1,10)

	os.makedirs("tasks/1000")
	with open('tasks/1000/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R' if i%2 else 'L',1000,'S','halt')))
	make_tape('tasks/1000/input.tape',-10,10,-10,10)

	os.makedirs("tasks/2000")
	with open('tasks/2000/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R' if (i//100)%2 else 'L',2000,'S','halt')))
	make_tape('tasks/2000/input.tape',-1000,1000,-10,10)


	os.makedirs("out_of_tape/33")
	with open('out_of_tape/33/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'R' if i%2 else 'S',33,'S','halt')))
	make_tape('out_of_tape/33/input.tape',-10,10,-10,10)

	os.makedirs("no_halt/35")
	with open('no_halt/35/code.t','w') as f:
		f.write('\n'.join(long_chain(lambda i: i%2,lambda i: 'S',35,'S','S3')))
	make_tape('no_halt/35/input.tape',-10,10,-10,10)

if __name__=="__main__":
	main()