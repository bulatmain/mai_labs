#!/bin/bash

suffix=""
days_left=0
dir_path="./"
rm_flags=""

alert_wrong_syntacs="Wrong syntacs. Please try to use this command with such template:\n\
\n\tcrm [ -s <file_suffix> ] [ -d <days_left> ] [ -p <directory_path> ] [-r <rm_command_flags> ]\n\
\nOr read short manual using: crm --help\n"

help_page="This is the short manual of usage command crm - custom remove.\n\n\
crm command has following syntacs:\n\n\
\tcrm [ -s <file_suffix> ] [ -d <days_left> ] [ -p <directory_path> ] [-r <rm_command_flags> ]\n\n\
and those following flags:\n\n\
\t-s, --suffix\t\tremoves only those files that end up with given suffix.\n\
\t\t\t\tSuffix can not contain not allowed characters: /\\!@#$^&*():;'\"<>,?|~\`[]{}+ \n\
\t-d, --days_left\t\tremoves only those files that wasn't accessed <days_left> days.\n\
\t-p, --directory_path\t\tremoves only those files in directory with given path.\n\
\t-r, --rm_flags\t\tsets given flags to remove command such as: -f, -i, -I, -r, -d, -v.\n\
\t--help\t\t\tShows this supporting page.\n"

n=$#

if [[ $n == 1 && [ ${!1} == "-h" || ${!1} == "--help" || ${!1} == "?"]]]; then
	echo -e $help_page
	exit 0
else
	echo -e $alert_wrong_syntacs
	exit 0
fi

for (( i=1; i<=n; i++ )); do
	if [[ ${!i} == "-s" || ${!i} == "--suffix" ]]; then
		j=$((i+1))
		s_=${!j}
		not_allowed_symbols="/\\!@#$^&*():;'\"<>,?|~\`[]{}+"
		wrong=0
		for (( k=0; k<${#s_}; k++ )); do
			if [[ "$not_allowed_symbols" ==  *"${s_:k:1}"* ]]; then
				if [[ $wrong == 0 ]]; then
					echo -e "\nCan not execute command with suffix contain now allowed symbols:"
					wrong=1
				fi
				if [[ $wrong == 1 ]]; then
					echo -n ${s_:k:1} "_"
				fi
			fi
		done
		if [[ $wrong == 1 ]]; then
				echo -e "\n"
				exit 0
		fi
		suffix=$s_
		i++
	elif [[ ${!i} == "-d" || ${!i} == "--days_left" ]]; then
		j=$((i+1))
		d_=${!j}
		if [[ "$d_" =~ "^[0-9]+$" ]]; then
			echo -e "Wrong format of <days_left>. \
It should be an integer amount.\n\
May be you should read a short manual using:\t crm --help\n"
			exit 0
		fi
		days_left=$d_
		i++
	elif [[ ${!i} == "-p" || ${!i} == "--directory_path" ]]; then
		j=$((i+1))
		p_=${!j}
		if [[ $p_ == /* && -d "$p_" ]]; then
			dir_path=$p_
		elif [[ -d "`pwd`/$p_" ]]; then
			dir_path=`pwd`/$p_
		else
			echo -e "Choosen directory do not exist\n"
			exit 0
		fi
		i++
	elif [[ ${!i} == "-r" || ${!i} == "--rm_flags" ]]; then
		j=$((i+1))
		r_=${!j}
		allowed_flags="fiIrdv"
		for (( k=1;k<${#r_}; k++ )); do
			if [[ "$allowed_flags" != *"${r_:k:1}"* ]]; then
				echo -e "You have given not allowed <rm_flags>. Please, check the description using:\tcrm --help\n"
				exit 0
			fi
		done
		rm_flags=$r_
		i++
	elif [[ ${!i} == "-h" || ${!i} == "--help" || ${!i} == "?"]]; then
		echo -e $help_page
		exit 0
	else
		j=$((i-1))
		if [[ $i == 1 || ${!j} != -* ]]; then
			echo -e $alert_wrong_syntacs
			exit 0
		fi
	fi
done

files=`find $dir_path -iname "*$suffix" -atime $days_left`

for i in $files; do
	rm $rm_flags $i
	echo -e "Deleted file: $i\n"
done
