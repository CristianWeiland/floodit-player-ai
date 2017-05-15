#!/usr/bin/env python3

import sys, subprocess, json
from pathlib import Path
from random import randint

if len(sys.argv) != 6:
    print('Usage: ' + sys.argv[0] + ' <nlinhas> <ncolunas> <ncores> <ntestes> <arquivo>')
    print('Ex: ' + sys.argv[0] + ' 5 5 5 100 ./database_5_5_5.json')
    sys.exit()

def run_command(seed):
    result = subprocess.run(['./backtrack', nlinhas, ncolunas, ncores, str(seed)], stdout=subprocess.PIPE)
    output = result.stdout.decode('utf-8')
    output = output.lstrip().rstrip().splitlines()
    return output

nlinhas = sys.argv[1]
ncolunas = sys.argv[2]
ncores = sys.argv[3]
ntestes = int(sys.argv[4])
arquivo = sys.argv[5]

file_exists = Path(arquivo)
if file_exists.is_file():
    with open(arquivo) as data_file:
        data = json.load(data_file)
else:
    data = {}

max_seed = 9999999

for x in range(0, ntestes):
    #seed = str(randint(1,max_seed))
    seed = x
    if seed not in data:
        ret = run_command(seed)
        seed = ret[0]
        njogadas = int(ret[1])
        jogadas = []
        for i in range(2, int(njogadas)+2):
            jogadas.append(int(ret[i]))
        data[seed] = {
            "njogadas": njogadas
            , "jogadas": jogadas
            , "nlinhas": nlinhas
            , "ncolunas": ncolunas
            , "ncores": ncores
        }

with open(arquivo, 'w') as outfile:
    json.dump(data, outfile, indent=4, sort_keys=True)
