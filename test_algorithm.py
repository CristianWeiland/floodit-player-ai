#!/usr/bin/env python3
# Tests executable $1 with tests in json file $2.
# The test executable should output ONLY the number of moves it took to finish the game and a \n. (only one printf with ("%d\n").
# The format from the JSON should be equal to database.json.example:
# - The key to the JSON is the seed;
# - The object contains some properties: 'nlinhas', 'ncolunas', 'ncores', 'njogadas' and an array 'jogadas'.

import sys, subprocess, json
from pathlib import Path

if len(sys.argv) != 3:
    print('Usage: ' + sys.argv[0] + ' <trabalho_executavel> <arquivo_json>')
    sys.exit()

cmd = sys.argv[1]
arquivo = sys.argv[2]

file_exists = Path(arquivo)
if not file_exists.is_file():
    print('Arquivo inexistente.')
    sys.exit()

with open(arquivo) as data_file:
    data = json.load(data_file)

worse = 0
equal = 0
better = 0
total = 0

for key in data:
    value = data[key]
    seed = key
    ncores = value['ncores']
    nlinhas = value['nlinhas']
    ncolunas = value['ncolunas']
    njogadas = value['njogadas']

    result = subprocess.run([cmd, nlinhas, ncolunas, ncores, str(seed)], stdout=subprocess.PIPE)
    output = result.stdout.decode('utf-8')
    result = int(output)

    total += result

    if(result < njogadas):
        better += 1
    elif(result == njogadas):
        equal += 1
    else:
        worse += 1

print('From ' + str(better+equal+worse) + ' tests:')
print(str(better) + ' were better, ' + str(equal) + ' were equal and ' + str(worse) + ' were worse.')
print('You took the total amount of ' + str(total) + ' moves. Average: ' + str(total/(worse+better+equal)) )
