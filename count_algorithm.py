#!/usr/bin/env python3
# Tests executable $1 with tests in json file $2.
# The test executable should output ONLY the number of moves it took to finish the game and a \n. (only one printf with ("%d\n").
# The format from the JSON should be equal to database.json.example:
# - The key to the JSON is the seed;
# - The object contains some properties: 'nlinhas', 'ncolunas', 'ncores', 'njogadas' and an array 'jogadas'.

import sys, subprocess, json
from pathlib import Path

if len(sys.argv) != 2:
    print('Usage: ' + sys.argv[0] + ' <trabalho_executavel>')
    sys.exit()

cmd = sys.argv[1]

file_exists = Path(cmd)
if not file_exists.is_file():
    print('Arquivo inexistente.')
    sys.exit()

algorithms = []
for i in range(0,10):
    algorithms.append(0)

nlinhas = 100
ncolunas = 100
ncores = 10

for key in range(0,1000):
    seed = key

#    result = subprocess.run([cmd, nlinhas, ncolunas, ncores, str(seed)], stdout=subprocess.PIPE)
#    output = result.stdout.decode('utf-8')
#    result = int(output)

    result = subprocess.check_output([cmd, str(nlinhas), str(ncolunas), str(ncores), str(seed)])
    output = str(result.decode('utf-8'))
    algorithms[int(output)] += 1

    print('Winner: ' + output + '!')

#print('Result: ' + algorithms)
i = 0
for elem in algorithms:
    print('Algorithm ' + str(i) + ' got ' + str(elem) + 'points.')
    i += 1
