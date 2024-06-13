import subprocess
import os
from os.path import join
import json
import filecmp
import shutil
import time

from concurrent.futures import ThreadPoolExecutor

def run_turing(task):
    # Run the command using subprocess
    result = subprocess.run(['./../bin/run_turing', join(task, 'code.t'), join(task, 'input.tape'), join(task, 'run_turing.tape')], text=True, capture_output=True)

    # Check the return code
    if result.returncode != 0:
        print("run_turing Failed:")
        if result.stderr:
            print(result.stderr)
        return 1

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



def compile_and_run_turing(task):
    # Step 1: Compile the code using tmc0
    start_compile_time = time.time()
    compile_result = subprocess.run(['./../bin/tmc0', join(task, 'code.t'), join(task, 'tmc0')], text=True, capture_output=True)
    compile_duration = time.time() - start_compile_time

    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        print("tmc0 Compilation Failed:")
        if compile_result.stderr:
            print(compile_result.stderr)
        return 1

    # Step 2: Run the compiled output with the input and output tapes
    start_run_time = time.time()
    run_result = subprocess.run([join(task, 'tmc0.out'), join(task, 'input.tape'), join(task, 'tmc0_run.tape')], text=True, capture_output=True)
    run_duration = time.time() - start_run_time

    # Check the return code for the run step
    if run_result.returncode != 0:
        print("tmc0.out Execution Failed:")
        if run_result.stderr:
            print(run_result.stderr)
        return 1

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
    return filecmp.cmp(file1, file2, shallow=False)

def run_and_compare(task):
    
    if(run_turing(task) or compile_and_run_turing(task)):
        return 1

    # Compare the output tapes
    tape1 = join(task, 'run_turing.tape')
    tape2 = join(task, 'tmc0_run.tape')

    if compare_tapes(tape1, tape2):
        print(f"[{task}] the tapes are identical. PASS")
    else:
        print(f"[{task}] The tapes are different. FAIL")

    return 0

def test_no_halt(task):
    # Step 1: Compile the code using tmc0
    compile_result = subprocess.run(['./../bin/tmc0', join(task, 'code.t'), join(task, 'tmc0')], text=True, capture_output=True)
    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        print("tmc0 Compilation Failed:")
        if compile_result.stderr:
            print(compile_result.stderr)
        return ""

    try:
        # Step 2: Run the compiled output with the input and output tapes
        process = subprocess.Popen([join(task, 'tmc0.out'), join(task, 'input.tape'), join(task, 'tmc0_run.tape')])
        time.sleep(0.15)
        if process.poll() is None:
            process.kill()
            return f"[{task}] didnt halt. PASS"

        

        # Check the return code for the run step
        if process.returncode == 1: #1 is TIME_OUT in turing.h
            return f"[{task}] gave a TIME_OUT code PASS"

        return f"[{task}] didnt halt gave a wrong return code FAIL"

    except Exception as e:
        print(f"[{task}] raised an Exception FAIL")

        if process is not None and process.returncode is None:
            process.kill()
        raise e

def test_out_of_tape(task):
    # Step 1: Compile the code using tmc0
    compile_result = subprocess.run(['./../bin/tmc0', join(task, 'code.t'), join(task, 'tmc0')], text=True, capture_output=True)
    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        print("tmc0 Compilation Failed:")
        if compile_result.stderr:
            print(compile_result.stderr)
        return 

    try:
        # Step 2: Run the compiled output with the input and output tapes
        run_result = subprocess.run([join(task, 'tmc0.out'), join(task, 'input.tape'), join(task, 'tmc0_run.tape')],capture_output=True)
        
        # Check the return code for the run step
        if run_result.returncode == 2: #2 is OUT_OF_TAPE in turing.h
            print(f"[{task}] gave an OUT_OF_TAPE code PASS")
            return

        print(f"[{task}] gave a wrong return code FAIL")
        return

    except Exception as e:
        print(f"[{task}] raised an Exception FAIL")
        raise e

if __name__=="__main__":
    import code_gen

    if(os.path.exists('no_halt')):
        shutil.rmtree('no_halt')

    if(os.path.exists('tasks')):
        shutil.rmtree('tasks')

    if(os.path.exists('out_of_tape')):
        shutil.rmtree('out_of_tape')

    code_gen.main()

    with ThreadPoolExecutor() as ex:
        futures=[ex.submit(test_no_halt,join('no_halt', d)) for d in os.listdir('no_halt')]

    tasks = [join('tasks', d) for d in os.listdir('tasks')]
    for task in tasks:
        run_and_compare(task)

    tasks = [join('out_of_tape', d) for d in os.listdir('out_of_tape')]
    for task in tasks:
        test_out_of_tape(task)

    for f in futures:
        print(f.result())


