
import re
import string
import random

def gen_random_str(N):
    return ''.join(
        random.choice(
            string.punctuation +
            string.ascii_uppercase +
            string.ascii_lowercase
        )
        for _ in range(N)
    )

def filter_chars(s):
    s = s.replace("'", '')
    s = s.replace('"', '')
    s = s.replace('\\', '')
    s = s.replace(' ', '')
    s = s.replace('#', '')
    return s


cpp_file = None
code_file = None

syntaxes = []
variables = []

with open('interpreter.cpp', 'r') as fi:
    cpp_file = fi.read()
    syntaxes = list(re.findall(r'#define (.+?) ', cpp_file))
    syntaxes.sort(key = lambda s: len(s), reverse=True)

with open('code.txt', 'r') as fi:
    code_file = fi.read()
    code_file = re.sub(r'\/\/.*\n', '', code_file)
    code_file = re.sub(r'\n{2,}', '\n', code_file)
    variables = list(set(re.findall(r'\s([a-z_]+(\d*)[a-z_]*)', code_file)))
    variables = [x[0] for x in variables]
    variables.sort(key = lambda s: len(s), reverse=True)

random_syntaxes = [gen_random_str(random.randint(20,30)) for _ in range(len(syntaxes))]
random_syntaxes = [filter_chars(x) for x in random_syntaxes]
random_syntaxes = list(zip(syntaxes, random_syntaxes))

random_variables = [gen_random_str(random.randint(20,30)) for _ in range(len(variables))]
random_variables = [filter_chars(x) for x in random_variables]
random_variables = list(zip(variables, random_variables))

for x in random_syntaxes:
    before = '#define {} "{}"'.format(x[0], x[0])
    after = '#define {} "{}"'.format(x[0], x[1])
    cpp_file = cpp_file.replace(before, after)
    code_file = code_file.replace(x[0], x[1])

for x in random_variables:
    code_file = code_file.replace(x[0], x[1])

with open('interpreter_obfuscated.cpp', 'w') as fo:
    fo.write(cpp_file)

with open('code_obfuscated.txt', 'w') as fo:
    fo.write(code_file)