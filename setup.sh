#!/bin/bash

# Olivia Pereira
# COMP 206, Assignment 2

# covering base case
if [[ $# = 0 ]]
then
	echo "Error: Task must be specified. Supported tasks: backup, archive, sortedcopy."
	echo "Usage: ./setup.sh <task> <additional_arguments>"
	exit 3
fi

# checking inputs
case $1 in
	"backup")
		echo $(pwd)
		ls *.txt 

		mkdir backup
		cd backup
		echo "Moved to backup directory"
		echo $(pwd)

		cp ../*.txt .
		echo "Copied all text files to backup directory"

		echo "Current backup:" > date.txt
		date >> date.txt
		cat date.txt
		exit 0
		;;

	"archive")
		if [[ $# -ne 2 ]] 
		then
			echo "Error: Archive tasks requires file format"
			echo "Usage: ./setup.sh archive <file format>"
			exit 4
		fi

		name="archive-$(date +'%Y-%m-%d').tgz"
		tar -cvzf $name *.$2
		echo "Created archive $name"
		ls -l $name
		exit 0
		;;
	
	"sortedcopy")
		# checking if the script is invoked with 2 additional arguments
		if [[ $# -ne 3 ]] 
		then
			echo "Error: Expected two additional input parameters."
			echo "Usage: ./setup.sh sortedcopy <sourcedirectory> <targetdirectory>"
			exit 1
		fi

		# verifying that the second input parameter is a directory
		if [[ ! -d $2 ]] 
		then
			echo "Error: Input parameter #2 '$2' is not a directory."
			echo "Usage: ./setup.sh sortedcopy <source directory> <target directory>"
			exit 2
		fi

		# creating target directory and copying files from source directory into it in reverse alpha order with # added to names
		
		# first, checking whether we need to overwrite an existing directory
		if [[ -d "$3" ]] 
		then 
			echo "Directory '$3' already exists. Overwrite? (y/n)"
			read -r response
			if [[ "$response" != "y" ]]
			then
				exit 3
			else 
				rm -rf "$3"
				mkdir "$3"
			fi
		else 
			mkdir "$3"
		fi

		# copying files - we set an index, and we list all the files in reverse order using the -r option
		index=1
		for file in $(ls -r "$2") 
		do
			if [ -f "$2/$file" ] 
			then
				cp "$2/$file" "$3/$((index++)).$file"
			fi 
		done
		exit 0
		;;

	*)
		echo "Error: Task must be specified. Supported tasks: backup, archive, sortedcopy."
        echo "Usage: ./setup.sh <task> <additional_arguments>"
		exit 3
esac
