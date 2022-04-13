# Unofficial FRC Robot Command Generator
[![Built with Spacemacs](https://cdn.rawgit.com/syl20bnr/spacemacs/442d025779da2f62fc86c2082703697714db6514/assets/spacemacs-badge.svg)](https://develop.spacemacs.org)

This is intended to provide command generation outside the reaches of the FRC VsCode extension.

## Usage
```bash
$ frcGen -h
Generate commands and subsystems from your frc project from the cli!
Usage:
  FRC C++ File Generator [OPTION...] name

 FRC C++ File Generator options:
  -c, --command          Generate a command.
  -s, --susbystem        Generate a susbsytem.
  -h, --help             Print usage
  -g, --generate-config  Generate a .frcGen.json file at the project root.
      --name arg         The name of the command or subsystem (default: 
                         UnReplaced)
```

## Building
```shell
$ mkdir build
$ cmake ..
$ cmake --build .
$ ./frcGen [OPTION...] name
```

## Philosghsfy
I got a new editor so I wanted to write something in c++. It would have taken so much less time if I had used python.
