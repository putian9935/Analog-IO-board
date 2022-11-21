import argparse

from backend import sweep, servo, stop, channel
from csv_reader import get_wfm 

def help_action(args):
    subparsers.choices[args.cmd].print_help()


def sweep_action(args):
    sweep(args.ch, args.lower, args.upper)

def servo_action(args):
    # non-exising file
    try:
        open(args.fname)
    except FileNotFoundError as e:
        print('%s: %s' %(e.__class__.__name__,  e))
        return 
    
    # wrong format f_I 
    try:
        f = float(eval(args.f_I.strip('"')))
    except (SyntaxError, NameError, TypeError) as e:
        print('%s: %s' %(e.__class__.__name__,  e))
        return 
    if f > 0:
        f = -f 
    servo(args.ch, f, args.G, get_wfm(args.fname))

def channel_action(args):
    if args.on:
        channel(args.ch, True)
    else:
        channel(args.ch, False)


def exit_action(args):
    exit(0)    

parser = argparse.ArgumentParser('', description="Intesnity servo terminal", add_help=False, epilog="Use 'help <command>' to see how to use each command.", exit_on_error=False)
subparsers = parser.add_subparsers()

sweep_parser = subparsers.add_parser("sweep", description="Start sweep", add_help=False, exit_on_error=False)
sweep_parser.add_argument('ch', type=int, choices=[0,1,2,3], help='Channel number')
sweep_parser.add_argument('--lower', type=int, required=False, default=0, help='Lower limit of DAC number. Default is 0. ')
sweep_parser.add_argument('--upper', type=int, required=False, default=1500, help='Upper limit of DAC number. Default is 1500')
sweep_parser.set_defaults(func=sweep_action)


servo_parser = subparsers.add_parser("servo", description="Start PI servo", add_help=False, exit_on_error=False)
servo_parser.add_argument('ch', type=int, choices=[0,1,2,3], help='Channel number')
servo_parser.add_argument('f_I', metavar='f_I', type=str, help='I corner in the unit of Nyquist frequency.')
servo_parser.add_argument('G', metavar='G', type=float, help='Overall gain')
servo_parser.add_argument('fname', metavar='fname', type=str, help='Reference waveform file')
servo_parser.set_defaults(func=servo_action)

channel_parser = subparsers.add_parser("channel", description="Control channel on/off", add_help=False, exit_on_error=False)
channel_parser.add_argument('ch', type=int, choices=[0,1,2,3], help='Channel number')
on_off = channel_parser.add_mutually_exclusive_group(required=True)
on_off.add_argument('--on', action='store_true')
on_off.add_argument('--off', action='store_true')
channel_parser.set_defaults(func=channel_action)

stop_parser = subparsers.add_parser("stop", description="Stop current command", add_help=False, exit_on_error=False)
stop_parser.set_defaults(func=stop)

exit_parser = subparsers.add_parser("exit", description="Exit terminal", add_help=False, exit_on_error=False)
exit_parser.set_defaults(func=exit_action)


help_parser = subparsers.add_parser("help", 
description='Display help for commands', add_help=False, exit_on_error=False)
help_parser.add_argument('cmd', type=str, choices=subparsers.choices.keys(), help='command name')
help_parser.set_defaults(func=help_action)

parser.print_help()

prompt = '>>> '
while True:
    commands = input(prompt).split()
    if not len(commands):
        continue
    try:
        a = parser.parse_args(commands)
        a.func(a)
    except argparse.ArgumentError as e:
        print(e)
    except SystemExit as e:
        if not e.code:
            exit(0)
        pass