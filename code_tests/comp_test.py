import subprocess
import os
from os.path import join
import json
import filecmp

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

    print(f"Results saved to {result_file_path}")
    return 0



def compile_and_run_turing(task):
    # Step 1: Compile the code using tmc0
    compile_result = subprocess.run(['./../bin/tmc0', join(task, 'code.t'), join(task, 'tmc0')], text=True, capture_output=True)

    # Check the return code for the compilation step
    if compile_result.returncode != 0:
        print("tmc0 Compilation Failed:")
        if compile_result.stderr:
            print(compile_result.stderr)
        return 1

    # Step 2: Run the compiled output with the input and output tapes
    run_result = subprocess.run([join(task, 'tmc0.out'), join(task, 'input.tape'), join(task, 'tmc0_run.tape')], text=True, capture_output=True)

    # Check the return code for the run step
    if run_result.returncode != 0:
        print("tmc0.out Execution Failed:")
        if run_result.stderr:
            print(run_result.stderr)
        return 1

    # Prepare the result information
    result_info = {
        "output": run_result.stdout,
        "errors": run_result.stderr
    }

    # Save the result to a JSON file
    result_file_path = join(task, 'compile_run_turing.json')
    with open(result_file_path, 'w') as result_file:
        json.dump(result_info, result_file, indent=4)

    print(f"Results saved to {result_file_path}")
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
        print("Passed the tapes are identical.")
    else:
        print("ERROR: The tapes are different.")

    return 0

if __name__=="__main__":
    tasks = [join('tasks', d) for d in os.listdir('tasks')]
    for task in tasks:
        run_and_compare(task)


