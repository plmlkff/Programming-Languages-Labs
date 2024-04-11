import subprocess as sb
from subprocess import PIPE

INPUTS = "test_data/input.txt"
OUTPUTS = "test_data/output.txt"
EXECUTABLE_FILE = "./main"

inputs_file = open(INPUTS, 'r')
outputs_file = open(OUTPUTS, 'r')
result = True

print("----------------------------------------")

def print_res(input_string, expected_value, received_value):
    print(
        "|  Test with input" + " [" + input_string + "] Result: " + ("SUCCESS" if expected_value == received_value else "FAILED"))
    print("|  Expected value: [" + expected_value + "]")
    print("|  Got value: [" + received_value + "]")
    print("----------------------------------------")


for input_str, output_str in zip(inputs_file, outputs_file):
    exp_value = output_str.strip()
    input_str = input_str.strip()
    proc = sb.Popen(EXECUTABLE_FILE, universal_newlines=True, stdin=PIPE, stdout=PIPE, stderr=PIPE)
    res = proc.communicate(input_str.strip())
    out = res[0].strip()
    err = res[1].strip()
    result = result if (out == exp_value) or (err == exp_value) else False
    print_res(input_str, exp_value, out if out != "" else err)
if result:
    print("-----------------------")
    print("|All tests are passed!|")
    print("-----------------------")

