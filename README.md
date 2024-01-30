# QuantScript

## Description

Domain-specific language for describing financial cash flows

## Pending implementation

1- Pending implementation of variable definition nodes.
	- Problem to be solved: when defining a macro, for example S=100, "S" would be replaced by macroReplacer by 100, remaining constant.
	- The idea is to implement an indexer that replaces the "variable" nodes with "definition" nodes. To do this, it is necessary to use a special iterator.

2- There must be a temporal node, or idexar the time to the AADNumbers, because it is necessary to be able to calculate sensitivities to ageing [theta].

3- Implement vectors

4- Implement interest rate models

5- Implement multiple currencies


## Tasks
1- Migrate the project to github and change the names /OK
	- The folder has been created, the files have been migrated and it has been compiled, hallelujah, but we still need to upload it. /OK

2- Before uploading it, you have to make sure that the compiled files are ignored, and see how you can reference quantlib and boost.
quantlib and boost to the versions on github.
	- It seems to be possible with quantlib, but boost is not on github.

3- There is a problem with the "parse.h" file, because it is confused with a standard library file. You need to check how the 
directories are changed.

