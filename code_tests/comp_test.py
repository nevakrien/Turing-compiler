import subprocess
import os
from os.path import join
import json
import filecmp
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



def compile_and_run_turing(task,compiler):
    # Step 1: Compile the code using tmc0
    start_compile_time = time.time()
    compile_result = subprocess.run([f'./../bin/{compiler}', join(task, 'code.t'), join(task, compiler)], text=True, capture_output=True)
    compile_duration = time.time() - start_compile_time

    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        print(f"{compiler} Compilation Failed:")
        if compile_result.stderr:
            print(compile_result.stderr)
        return 1

    # Step 2: Run the compiled output with the input and output tapes
    start_run_time = time.time()
    run_result = subprocess.run([join(task, f'{compiler}.out'), join(task, 'input.tape'), join(task, f'{compiler}_run.tape')], text=True, capture_output=True)
    run_duration = time.time() - start_run_time

    # Check the return code for the run step
    if run_result.returncode != 0:
        print(f"{compiler} .out Execution Failed:")
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

def run_and_compare(task,compiler):
    
    if(run_turing(task) or compile_and_run_turing(task,compiler)):
        return 1

    # Compare the output tapes
    tape1 = join(task, 'run_turing.tape')
    tape2 = join(task, f'{compiler}_run.tape')

    if compare_tapes(tape1, tape2):
        print(f"[{task}] the tapes are identical. PASS")
    else:
        print(f"[{task}] The tapes are different. FAIL")

    return 0

def test_no_halt(task,compiler):
    # Step 1: Compile the code using tmc0
    compile_result = subprocess.run([f'./../bin/{compiler}', join(task, 'code.t'), join(task, compiler)], text=True, capture_output=True)
    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        print("tmc0 Compilation Failed:")
        if compile_result.stderr:
            print(compile_result.stderr)
        return ""

    try:
        # Step 2: Run the compiled output with the input and output tapes
        process = subprocess.Popen([join(task, f'{compiler}.out'), join(task, 'input.tape'), join(task, f'{compiler}_run.tape')])
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

def test_out_of_tape(task,compiler):
    # Step 1: Compile the code using tmc0
    compile_result = subprocess.run([f'./../bin/{compiler}', join(task, 'code.t'), join(task, compiler)], text=True, capture_output=True)
    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        print("tmc0 Compilation Failed:")
        if compile_result.stderr:
            print(compile_result.stderr)
        return 

    try:
        # Step 2: Run the compiled output with the input and output tapes
        run_result = subprocess.run([join(task, f'{compiler}.out'), join(task, 'input.tape'), join(task, f'{compiler}_run.tape')],capture_output=True)
        
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


    code_gen.main()

    futures={}

    for compiler in ['tmc0','tmc1']:
        print(f'testing {compiler}')
        with ThreadPoolExecutor() as ex:
            futures[compiler]=[ex.submit(test_no_halt,join('no_halt', d),compiler) for d in os.listdir('no_halt')]

        tasks = [join('tasks', d) for d in os.listdir('tasks')]
        for task in tasks:
            run_and_compare(task,compiler)

        tasks = [join('out_of_tape', d) for d in os.listdir('out_of_tape')]
        for task in tasks:
            test_out_of_tape(task,compiler)

    for compiler in ['tmc0','tmc1']:
        print(f'no_hault {compiler}')
        for f in futures[compiler]:
            print(f.result())