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
    call_make('clean_io')
    call_make('test_io.out')
    print("test_io ReCompilation Done")
    

    # Create a temporary file to use for output
    with tempfile.NamedTemporaryFile(delete=False) as tmp_out:
        output_filename = tmp_out.name
    
    # Run the compiled program with the temporary file
    test_proc = subprocess.run(['./test_io.out', output_filename], text=True, capture_output=True)
    
    # Check results
    if test_proc.returncode == 0:
        print("IO Test Passed: Tape data validated successfully.")
    else:
        print(f"IO Test Failed with exit code {test_proc.returncode}:\n  {test_proc.stdout} sterr:{test_proc.stderr}")

    print("")
    # Clean up the temporary file
    os.unlink(output_filename)

def run_test_turing():
    call_make('clean_turing')
    call_make('test_turing.out')
    print("test_turing ReCompilation Done")

    test_proc = subprocess.run(['./test_turing.out'], text=True, capture_output=True)
    
    # Check results
    if test_proc.returncode == 0:
        print("IO Test Passed: Tape data validated successfully.")
    else:
        print(f"IO Test Failed with exit code {test_proc.returncode}:\n  {test_proc.stdout} sterr:{test_proc.stderr}")

    print("")
    

if __name__ == '__main__':
    run_test_io()
    run_test_turing()
