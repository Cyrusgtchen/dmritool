#!/usr/bin/env python
"""
Extract bash codes from a rst file
"""
import argparse, re

def main():

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input_rst', nargs=1, help='input rst file')
    parser.add_argument('output_sh', nargs=1, help='output sh file')

    args = parser.parse_args()
    rstfile = args.input_rst[0]
    shfile = args.output_sh[0]

    f = open(rstfile,"r")
    lines = f.readlines()
    f.close()

    f = open(shfile,"w")

    shstartcheck = re.compile(r'^.. code-block:: shell[ ]*$')
    shendcheck = re.compile(r'^[^\s]')
    shstart = False
    shlines = []
    for line in lines:
        if shstartcheck.match(line):
            shstart = True
            continue
        if shstart and line and shendcheck.match(line):
            shstart = False
        if shstart :
            shlines.append(line.strip())

    f.write('\n'.join(shlines))

    f.close()


if __name__ == '__main__':
    main()