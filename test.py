import subprocess
import tempfile
import os
import time


def call_make(target):
    compile_proc = subprocess.run(['make', target], text=True, capture_output=True)
    if compile_proc.returncode != 0:
        print("Compilation Failed:")
        print(compile_proc.stderr)
        raise Exception(f"failed to compile {target}") 
    

def run_test_io():
    #call_make('clean_io')
    call_make('bin/test_io')
    print("test_io Compilation Done")
    

    # Create a temporary file to use for output
    with tempfile.NamedTemporaryFile(delete=False) as tmp_out:
        output_filename = tmp_out.name
    
    # Run the compiled program with the temporary file
    test_proc = subprocess.run(['./bin/test_io', output_filename], text=True, capture_output=True)
    
    # Check results
    if test_proc.returncode == 0:
        print("IO Test Passed")
    else:
        print(f"\n!!!IO Test Failed with exit code {test_proc.returncode}:\n  {test_proc.stdout} sterr:{test_proc.stderr}\n")

    # Clean up the temporary file
    os.unlink(output_filename)

def run_test_turing():
    #call_make('clean_turing')
    call_make('bin/test_turing')
    print("test_turing Compilation Done")

    test_proc = subprocess.run(['./bin/test_turing'], text=True, capture_output=True)
    
    # Check results
    if test_proc.returncode == 0:
        print("Turing Test Passed")
    else:
        print(f"\n!!!Turing Test Failed with exit code {test_proc.returncode}:\n\n{test_proc.stdout} sterr:{test_proc.stderr}\n")

    

def run_test_parser():
    call_make('bin/test_parser')
    print("test_parser Compilation Done")

    # Directories containing syntax examples
    valid_dir = 'tests/code_samples/valid'
    invalid_dir = 'tests/code_samples/invalid'

    def run_tests(directory, expected_return_code):
        for filename in os.listdir(directory):
            if filename.endswith('.t'):
                file_path = os.path.join(directory, filename)
                test_proc = subprocess.run(['./bin/test_parser', file_path], text=True, capture_output=True)
                
                # Check results
                if test_proc.returncode == expected_return_code:
                    print(f"Parser Test ({filename}) Passed")
                else:
                    print(f"\n!!!Parser Test ({filename}) Failed with exit code {test_proc.returncode} (expected {expected_return_code}):\n\n{test_proc.stdout} stderr:{test_proc.stderr}\n")

    # Run tests for valid syntax examples
    run_tests(valid_dir, expected_return_code=0)

    # Run tests for invalid syntax examples
    run_tests(invalid_dir, expected_return_code=1)

def run_test_compiler():
    call_make('bin/test_compiler')
    print("test_compiler Compilation Done")

    test_proc = subprocess.run(['./bin/test_compiler'], text=True, capture_output=True)
    
    # Check results
    if test_proc.returncode == 0:
        print("nasm ld Test Passed")
    else:
        print(f"\n!!!nasm ld Failed with exit code {test_proc.returncode}:\n\n{test_proc.stdout} sterr:{test_proc.stderr}\n")


def run_comp_test():
    call_make('all')
    print("full Compilation Done")
    # Save the current working directory
    original_cwd = os.getcwd()
    
    try:
        # Change to the code_tests directory
        os.chdir('code_tests')
        
        # Run the comp_test.py script
        test_proc=subprocess.run(['python3', 'comp_test.py'], stderr=subprocess.STDOUT, text=True)
        print('tmc0 test done')
        #print(test_proc.stderr)
    except:
        print('tmc0 test Failed')

    finally:
        # Change back to the original working directory
        os.chdir(original_cwd)
    

    # # Check results
    # if test_proc.returncode == 0:
    #     print("tmc0 comperison Test Passed")
    # else:
    #     print(f"\n!!!tmc0 comperison Test Failed with exit code {test_proc.returncode}:\n\n{test_proc.stdout} sterr:{test_proc.stderr}\n")

if __name__ == '__main__':
    start_time = time.time()
    
    print("starting tests...\n")

    run_test_io()
    print("")
    
    run_test_turing()
    print("")
    
    run_test_parser()
    print("")
    
    run_test_compiler()
    print("")
    
    run_comp_test()
    print("")
    
    end_time = time.time()
    elapsed_time = end_time - start_time
    
    print(f"all tests are done! Time elapsed: {elapsed_time:.2f} seconds")
