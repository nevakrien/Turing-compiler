import random
import os
from os.path import join
from time import time_ns
import gc
from concurrent.futures import ProcessPoolExecutor
import subprocess
import shutil

def time_run_turing_no_stop(task):
        # Highly sensitive timing
        gc.disable()
        start_time_ns = time_ns()

        # Run the command using subprocess
        compile_proc = subprocess.run(['./../bin/run_turing', join(task, 'code.t'), join(task, 'input.tape'), '/dev/null'], text=True, capture_output=True)

        end_time_ns = time_ns()
        gc.enable()

        # Calculate elapsed time in nanoseconds
        return end_time_ns - start_time_ns, compile_proc

def time_run_turing(task):
        # Highly sensitive timing
        gc.disable()
        start_time_ns = time_ns()

        # Run the command using subprocess
        compile_proc = subprocess.run(['./../bin/run_turing', join(task, 'code.t'), join(task, 'input.tape'),  '/dev/null', '10000'], text=True, capture_output=True)

        end_time_ns = time_ns()
        gc.enable()

        # Calculate elapsed time in nanoseconds
        return end_time_ns - start_time_ns, compile_proc


def time_tmc0(task):
        gc.disable()
        start_time_ns = time_ns()

        compile_proc = subprocess.run([join(task, 'tmc0.out'), join(task, 'input.tape'), '/dev/null'], text=True, capture_output=True)

        end_time_ns = time_ns()
        gc.enable()

        return end_time_ns - start_time_ns, compile_proc

def time_tmc1(task):
        gc.disable()
        start_time_ns = time_ns()

        compile_proc = subprocess.run([join(task, 'tmc1.out'), join(task, 'input.tape'), '/dev/null'], text=True, capture_output=True)

        end_time_ns = time_ns()
        gc.enable()

        return end_time_ns - start_time_ns, compile_proc


def measure(d):
        return d['program'](d['task'])

def main_timing(tasks):
    tasks.sort()
    print("\nstarting timers...\n")

    #timers={'run_turing_no_stop':time_run_turing_no_stop,'run_turing':time_run_turing,'tmc0':time_tmc0}#,'tmc1':time_tmc1}
    timers={'tmc0':time_tmc0,'tmc1':time_tmc1}


    jobs=[{'task':t,'name':k,'program':p,'id':i} for t in tasks for k,p in timers.items() for i in range(1000)]
    random.shuffle(jobs)

    with ProcessPoolExecutor(os.cpu_count() - 1) as ex:
        results=list(ex.map(measure,jobs))
    
    for j,r in zip(jobs,results):
        j['time']=r


    tally={task:{i:{name:None for name in timers.keys()} for i in range(1000)} for task in tasks}
    for j in jobs:
        tally[j['task']][j['id']][j['name']]=j['time']


    # Calculate scores
    scores = {}
    for task, ids in tally.items():
        task_scores = {name: 0 for name in timers.keys()}  # Initialize scores for each program
        valid_counts = 0  # To count only IDs with valid timings

        for id, name_times in ids.items():
            # Filter out None times and find the fastest program
            filtered_times = {name: time for name, time in name_times.items() if time is not None}
            if filtered_times:
                fastest_program = min(filtered_times, key=filtered_times.get)
                task_scores[fastest_program] += 1
                valid_counts += 1

        # Calculate the fraction of wins for each program
        if valid_counts > 0:
            for name in task_scores:
                task_scores[name] /= valid_counts

        scores[task] = task_scores

    # Print scores in a readable format
    for task, program_scores in scores.items():
        print(f"Task: {task}")
        for program, score in program_scores.items():
            print(f"{program}: {score:.2%}")
        print("")
    
    #avrage
    # Calculate weights
    weights = {task: 2 if 'tasks/dead' in task else 1 for task in scores.keys()}
    # Compute weighted average scores for each program
    total_weights = sum(weights.values())
    weighted_scores = {program: 0 for program in next(iter(scores.values())).keys()}  # Initialize scores based on programs

    for task, program_scores in scores.items():
        for program, score in program_scores.items():
            weighted_scores[program] += score * weights[task]

    # Normalize the scores by the total weights
    for program in weighted_scores:
        weighted_scores[program] /= total_weights

        # Print weighted average scores
    print("Weighted Average Scores Across All Tasks:")
    for program, score in weighted_scores.items():
        print(f"{program}: {score:.2%}")

def main():
    tasks=[join('tasks', d) for d in os.listdir('tasks')]
    main_timing(tasks)

# def setup_tmpfs(mount_point, size='128M'):
#     # Ensure the mount point exists within the current working directory
#     if not os.path.exists(mount_point):
#         os.makedirs(mount_point)
#     # Mount the tmpfs
#     subprocess.run(['sudo', 'mount', '-t', 'tmpfs', '-o', f'size={size}', 'tmpfs', mount_point], check=True)

# def cleanup_tmpfs(mount_point):
#     # Unmount the tmpfs
#     subprocess.run(['sudo', 'umount', mount_point], check=True)
#     # Remove the mount point directory to clean up
#     if os.path.exists(mount_point):
#         os.rmdir(mount_point)

# def copy_tasks_to_mem(src_directory, dest_directory):
#     # Copy all files from the source to the destination directory
#     for item in os.listdir(src_directory):
#         s = os.path.join(src_directory, item)
#         d = os.path.join(dest_directory, item)
#         if os.path.isdir(s):
#             shutil.copytree(s, d, dirs_exist_ok=True)
#         else:
#             shutil.copy2(s, d)

# #we are doing our best by memory maping the files.
# def main_memory_maped():
#     # Setup paths
#     src_directory = 'tasks'  # Source tasks directory
#     mem_tasks_directory = 'mem_tasks'  # Destination directory in tmpfs

#     # Setup the tmpfs at the specified mount point
#     setup_tmpfs(mem_tasks_directory)

#     try:
#         # Copy tasks to the memory-mapped directory
#         copy_tasks_to_mem(src_directory, mem_tasks_directory)
#         tasks = [join('mem_tasks', d) for d in os.listdir('mem_tasks')]
#         main_timing(tasks)

#     finally:
#         # Ensure cleanup happens regardless of previous errors
#         cleanup_tmpfs(mem_tasks_directory)



if __name__ == "__main__":
    main()