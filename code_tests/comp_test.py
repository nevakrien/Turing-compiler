import subprocess
import os
from os.path import join
import json
#import filecmp
import time

from concurrent.futures import ProcessPoolExecutor

def run_turing(task):
    # Run the command using subprocess
    result = subprocess.run(['./../bin/run_turing', join(task, 'code.t'), join(task, 'input.tape'), join(task, 'run_turing.tape')], text=True, capture_output=True)

    # Check the return code
    if result.returncode != 0:
        raise Exception(f"run_turing [{task}] \033[91mFailed\033[0m:\n{result.stderr}")

    # Prepare the result information
    result_info = {
        "output": result.stdout,
        "errors": result.stderr
    }

    # Save the result to a JSON file
    result_file_path = join(task, 'run_turing.json')
    with open(result_file_path, 'w') as result_file:
        json.dump(result_info, result_file, indent=4)

    #print(f"Results saved to {result_file_path}")
    return 0


def maybe_qemu(command, use_qemu=False, **kwargs):
    """
    Execute a command, optionally using qemu-arm.
    
    Args:
        command (list): The command to execute.
        use_qemu (bool): Whether to prepend qemu-arm to the command.
        kwargs: Additional keyword arguments for subprocess functions.
        
    Returns:
        CompletedProcess: The result of the subprocess.run call.
    """
    if use_qemu:
        command = ['qemu-arm'] + command
    return subprocess.run(command, **kwargs)

def maybe_qemu_popen(command, use_qemu=False, **kwargs):
    """
    Execute a command with Popen, optionally using qemu-arm.
    
    Args:
        command (list): The command to execute.
        use_qemu (bool): Whether to prepend qemu-arm to the command.
        kwargs: Additional keyword arguments for subprocess functions.
        
    Returns:
        Popen: The Popen process object.
    """
    if use_qemu:
        command = ['qemu-arm'] + command
    return subprocess.Popen(command, **kwargs)

def compile_and_run_turing(task,compiler):
    # Step 1: Compile the code using compiler
    start_compile_time = time.time()
    compile_result = subprocess.run([f'./../bin/{compiler}', join(task, 'code.t'), join(task, compiler)], text=True, capture_output=True)
    compile_duration = time.time() - start_compile_time

    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        return Exception(f"{compiler} Compilation [{task}] \033[91mFailed\033[0m:\n{compile_result.stderr}")

    use_qemu = 'arm' in compiler

    # Step 2: Run the compiled output with the input and output tapes
    start_run_time = time.time()
    run_result = maybe_qemu([join(task, f'{compiler}.out'), join(task, 'input.tape'), join(task, f'{compiler}_run.tape')],use_qemu=use_qemu, text=True, capture_output=True)
    run_duration = time.time() - start_run_time

    # Check the return code for the run step
    if run_result.returncode != 0:
        raise Exception(f"{compiler}.out [{task}] Execution \033[91mFailed\033[0m:\n{run_result.stderr}\nwith code {run_result.returncode}")

    # Prepare the result information
    result_info = {
        "output": run_result.stdout,
        "errors": run_result.stderr,
        "compile_duration": compile_duration,
        "run_duration": run_duration
    }

    # Save the result to a JSON file
    result_file_path = join(task, 'compile_run_turing.json')
    with open(result_file_path, 'w') as result_file:
        json.dump(result_info, result_file, indent=4)

    # print(f"Results saved to {result_file_path}")
    # print(f"Compilation Time: {compile_duration:.4f} seconds")
    # print(f"Execution Time: {run_duration:.4f} seconds")
    
    return 0



def compare_tapes(file1, file2):
    #return filecmp.cmp(file1, file2, shallow=False)
    result = subprocess.run([f'./../bin/tape_tool','cmp', file1, file2], text=True, capture_output=True)
    if(result.returncode!=0):
        raise Exception(f"tape_tool \033[91mFailed\033[0m on files {file1} {file2}")

    if(result.stdout.strip()=='yes'):
        return True
    if(result.stdout.strip()=='no'):
        return False

    raise Exception(f"tape_tool gave weird answer on files {file1} {file2}")

    
def run_and_compare(task,compiler):
    #run_turing(task)
    compile_and_run_turing(task,compiler)

    # Compare the output tapes
    tape1 = join(task, 'run_turing.tape')
    tape2 = join(task, f'{compiler}_run.tape')

    if compare_tapes(tape1, tape2):
        return f"[{task}] the tapes are identical. \033[92mPASS\033[0m"
    
    return f"[{task}] The tapes are different. \033[91mFAIL\033[0m"


def test_no_halt(task,compiler):
    # Step 1: Compile the code using tmc0
    compile_result = subprocess.run([f'./../bin/{compiler}', join(task, 'code.t'), join(task, compiler)], text=True, capture_output=True)
    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        raise Exception(f"{compiler}  [{task}]Compilation \033[91mFailed\033[0m:\n{compile_result.stderr}")

    try:
        use_qemu = 'arm' in compiler
        # Step 2: Run the compiled output with the input and output tapes
        process = maybe_qemu_popen([join(task, f'{compiler}.out'), join(task, 'input.tape'), join(task, f'{compiler}_run.tape')],use_qemu)
        time.sleep(0.15)
        if process.poll() is None:
            process.kill()
            return f"[{task}] didnt halt. \033[92mPASS\033[0m"

        

        # Check the return code for the run step
        if process.returncode == 1: #1 is TIME_OUT in turing.h
            return f"[{task}] gave a TIME_OUT code \033[92mPASS\033[0m"

        return f"[{task}] didnt halt gave a wrong return code \033[91mFAIL\033[0m"

    except Exception as e:
        if process is not None and process.returncode is None:
            process.kill()
        return f"[{task}] raised an Exception \033[91mFAIL\033[0m\n{e}"

def test_out_of_tape(task,compiler):
    # Step 1: Compile the code using tmc0
    compile_result = subprocess.run([f'./../bin/{compiler}', join(task, 'code.t'), join(task, compiler)], text=True, capture_output=True)
    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        return f"{compiler} Compilation \033[91mFailed\033[0m:\n{compile_result.stderr}"
         

    try:
        use_qemu = 'arm' in compiler

        # Step 2: Run the compiled output with the input and output tapes
        run_result = maybe_qemu([join(task, f'{compiler}.out'), join(task, 'input.tape'), join(task, f'{compiler}_run.tape')],use_qemu=use_qemu,capture_output=True)
        
        # Check the return code for the run step
        if run_result.returncode == 2: #2 is OUT_OF_TAPE in turing.h
            return f"[{task}] gave an OUT_OF_TAPE code \033[92mPASS\033[0m"
            

        return f"[{task}] gave a wrong return code \033[91mFAIL\033[0m"
        

    except Exception as e:
        return f"[{task}] raised an Exception \033[91mFAIL\033[0m\n{e}"

if __name__=="__main__":
    import code_gen

    compilers=['tmc0','tmc1','treemc','tmc2','arm',"arm_treemc"]#['tmc1_bad_hop','tmc1']

    code_gen.main()

    futures={}
    with ProcessPoolExecutor() as ex:
        for compiler in compilers:
            tasks = [join('no_halt', d) for d in os.listdir('no_halt')]
            futures[compiler]=[ex.submit(test_no_halt,task,compiler) for task in tasks]

            tasks = [join('tasks', d) for d in os.listdir('tasks')]
            list(ex.map(run_turing,tasks))
            futures[compiler].extend([ex.submit(run_and_compare,task,compiler) for task in tasks])

            tasks = [join('out_of_tape', d) for d in os.listdir('out_of_tape')]
            futures[compiler].extend([ex.submit(test_out_of_tape,task,compiler) for task in tasks])


    for compiler in compilers:
        print(f'results {compiler}:')
        for f in futures[compiler]:
            print(f.result())