import subprocess
import os
import json
from os.path import join
from time import time_ns
import gc
from concurrent.futures import ProcessPoolExecutor

def run_turing(task):
    def run_timing():
        # Highly sensitive timing
        gc.disable()
        start_time_ns = time_ns()

        # Run the command using subprocess
        compile_proc = subprocess.run(['./../bin/run_turing', join(task, 'code.t'), join(task, 'input.tape'), join(task, 'run_turing.tape')], text=True, capture_output=True)

        end_time_ns = time_ns()
        gc.enable()

        # Calculate elapsed time in nanoseconds
        return end_time_ns - start_time_ns, compile_proc

    trials = [run_timing() for _ in range(1000)]
    elapsed_times_ns = [trial[0] for trial in trials]
    average_time_ns = sum(elapsed_times_ns) / len(elapsed_times_ns)
    
    outputs = [trial[1].stdout for trial in trials]
    errors = [trial[1].stderr for trial in trials if trial[1].stderr]
    returncodes = [trial[1].returncode for trial in trials]

    if any(returncode != 0 for returncode in returncodes):
        print("run_turing Failed:")
        for error in errors:
            print(error)
        return

    # Prepare the result information
    result_info = {
        "timing": {
            "average_ns": average_time_ns,
            "all_times_ns": elapsed_times_ns
        },
        "output": outputs,  # Include all outputs
        "errors": errors  # Include all errors
    }

    # Save the result to a JSON file
    result_file_path = join(task, 'run_turing.json')
    with open(result_file_path, 'w') as result_file:
        json.dump(result_info, result_file, indent=4)

    print(f"Results saved to {result_file_path}")
    return elapsed_times_ns

def run_turing_counted(task):
    def run_timing():
        # Highly sensitive timing
        gc.disable()
        start_time_ns = time_ns()

        # Run the command using subprocess
        compile_proc = subprocess.run(['./../bin/run_turing', join(task, 'code.t'), join(task, 'input.tape'), join(task, 'run_turing_counted.tape'), '10000'], text=True, capture_output=True)

        end_time_ns = time_ns()
        gc.enable()

        # Calculate elapsed time in nanoseconds
        return end_time_ns - start_time_ns, compile_proc

    trials = [run_timing() for _ in range(1000)]
    elapsed_times_ns = [trial[0] for trial in trials]
    average_time_ns = sum(elapsed_times_ns) / len(elapsed_times_ns)
    
    outputs = [trial[1].stdout for trial in trials]
    errors = [trial[1].stderr for trial in trials if trial[1].stderr]
    returncodes = [trial[1].returncode for trial in trials]

    if any(returncode != 0 for returncode in returncodes):
        print("run_turing_counted Failed:")
        for error in errors:
            print(error)
        return

    # Prepare the result information
    result_info = {
        "timing": {
            "average_ns": average_time_ns,
            "all_times_ns": elapsed_times_ns
        },
        "output": outputs,  # Include all outputs
        "errors": errors  # Include all errors
    }

    # Save the result to a JSON file
    result_file_path = join(task, 'run_turing_counted.json')
    with open(result_file_path, 'w') as result_file:
        json.dump(result_info, result_file, indent=4)

    print(f"Results saved to {result_file_path}")
    return elapsed_times_ns

def lazy_flatten(iterable):
    for item in iterable:
        if isinstance(item, (list, tuple)):  # Check if item is a list or tuple
            yield from lazy_flatten(item)   # Recursively flatten
        else:
            yield item

if __name__ == "__main__":
    tasks = [join('tasks', d) for d in os.listdir('tasks')]

    with ProcessPoolExecutor() as executor:
        futures_turing = executor.map(run_turing, tasks)
        futures_turing_counted = executor.map(run_turing_counted, tasks)
        
        results = list(zip(lazy_flatten(futures_turing), lazy_flatten(futures_turing_counted)))

    #print([f'no_stop {a[0]} stop {a[1]}' for a in results])

    interpeter_result=[a[0]>a[1] for a in results]
    interpeter_result=sum(interpeter_result)/len(interpeter_result)
    print(f'run_turing: {interpeter_result} run_turing_no_stop {1-interpeter_result}')

