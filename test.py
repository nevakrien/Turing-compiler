import subprocess
import tempfile
import os

def call_make(target):
    compile_proc = subprocess.run(['make', target], text=True, capture_output=True)
    if compile_proc.returncode != 0:
        print("Compilation Failed:")
        print(compile_proc.stderr)
        return

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

    test_proc = subprocess.run(['./bin/test_parser','syntax_examples/first.t'], text=True, capture_output=True)

    # Check results
    if test_proc.returncode == 0:
        #print(f"PRINT TEST:\n{test_proc.stdout}")
        print("Parser Test Passed")
    else:
        print(f"\n!!!Parser Test Failed with exit code {test_proc.returncode}:\n\n{test_proc.stdout} sterr:{test_proc.stderr}\n")


if __name__ == '__main__':
    print("starting tests...\n")
    run_test_io()
    print("")
    
    run_test_turing()
    print("")

    run_test_parser()
    print("")

    print("all tests are done!")
