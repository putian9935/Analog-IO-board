import argparse

from backend import sweep, servo
from csv_reader import get_wfm 

def help_action(args):
    subparsers.choices[args.cmd].print_help()


def sweep_action(args):
    sweep(args.lower, args.upper)

def servo_action(args):
    try:
        open(args.fname)
    except FileNotFoundError as e:
        print(e)
    servo(args.f_I, args.g, get_wfm(args.fname))
    

parser = argparse.ArgumentParser('', description="Intesnity servo terminal", add_help=False, epilog="Use 'help command name' to see how to use each command.", exit_on_error=False)
subparsers = parser.add_subparsers()


help_parser = subparsers.add_parser("help", 
description='Display help for commands', add_help=False, exit_on_error=False)
help_parser.add_argument('cmd', type=str, choices=['sweep', 'servo', 'help'], help='command name')
help_parser.set_defaults(func=help_action)

sweep_parser = subparsers.add_parser("sweep", description="Start sweep", add_help=False, exit_on_error=False)
sweep_parser.add_argument('--lower', type=int, required=False, default=0, help='Lower limit of DAC number. Default is 0. ')
sweep_parser.add_argument('--upper', type=int, required=False, default=1500, help='Upper limit of DAC number. Default is 1500')
sweep_parser.set_defaults(func=sweep_action)


servo_parser = subparsers.add_parser("servo", description="Start PI servo", add_help=False, exit_on_error=False)
servo_parser.add_argument('f_I', metavar='f_I', type=str, help='I corner in the unit of Nyquist frequency')
servo_parser.add_argument('G', metavar='G', type=float, help='Overall gain')
servo_parser.add_argument('fname', metavar='fname', type=str, help='Reference waveform file')
servo_parser.set_defaults(func=servo_action)

parser.print_help()

prompt = '>>> '
while True:
    commands = input(prompt).split()
    try:
        a = parser.parse_args(commands)
        a.func(a)
    except argparse.ArgumentError as e:
        print(e)
    except SystemExit as e:
        pass