#! /bin/bash

# This script sends a login request to a AVM Fritz!Box
# (or some other AVM devices) following the steps described in
# http://avm.de/fileadmin/user_upload/Global/Service/Schnittstellen/AVM_Technical_Note_-_Session_ID.pdf

PATH_LOGIN="login_sid.lua"
PATH_HOMEAUTOSWITCH="webservices/homeautoswitch.lua"

URL_DEBUG=0


printUsage()
{
	echo "This script interacts with a AVM switch (like"
	echo "Fritz!Powerline 546e)"
	echo "There are two modes. The simple one always"
	echo "needs a username and password, the extended"
	echo "requests a session id once which can be reused"
	echo "for later calls which speeds things up."
	echo ""
	echo "Login (print session id SID to stdout):"
	echo "	$0 -u USERNAME -p PASSWORD ADDRESS" 
	echo ""
	echo "Logout (makes SID requested above invalid):"
	echo "	$0 -s SID ADDRESS"
	echo ""
	echo "List all devices (print AIN = id and some info)":
	echo "	$0 -u USERNAME -p PASSWORD -l ADDRESS"
	echo "	$0 -s SID -l ADDRESS"
	echo ""
	echo "Switch a device on (a = activate)"
	echo "	$0 -u USERNAME -p PASSWORD -a AIN ADDRESS"
	echo "	$0 -s SID -a AIN ADDRESS"
	echo ""
	echo "Switch a device off (d = deactivate)"
	echo "	$0 -u USERNAME -p PASSWORD -d AIN ADDRESS"
	echo "	$0 -s SID -d AIN ADDRESS"
	echo ""
	echo "Query state of a device"
	echo "	$0 -u USERNAME -p PASSWORD -q AIN ADDRESS"
	echo "	$0 -s SID -q AIN ADDRESS"
	echo ""
	echo "Note:"
	echo "All calls with -s SID above logout automatically."
	echo "To prevent logout add -n (=nologout) as additional parameter."
	echo ""
	echo "Other important note:"
	echo "If username is empty pass it with empty string: -u \"\""
	echo ""
	echo "Debug options:"
	echo "-U: print all URL requests to STDERR"
}

logMessage()
{
	local key="$1"
	local message="$2"
	#echo "$key = $message" >&2
}

requestWebsite()
{
	local address="$1"
	
	local result=$(curl "${address}" 2> /dev/null)
	
	if [[ URL_DEBUG -ne 0 ]]
	then
		echo "${address}" >&2
		echo "$result" >&2
	fi
	
	echo "$result";
	
}

runCommand()
{
	local cmd="$1"
	local result=$(eval "$cmd")
	logMessage "$cmd" "$result"
	echo $result
}


requestChallenge()
{
	local url="$1"
	
	
	local response=$(requestWebsite ${url}/${PATH_LOGIN});
	logMessage "requestChallenge_response" "$response"
	
	local challenge=$(echo "$response" | sed -r 's/.*<Challenge>(.*?)<\/Challenge>.*/\1/')
	logMessage "requestChallenge_challenge" "$challenge"
	
	echo "$challenge"
}


# http://avm.de/fileadmin/user_upload/Global/Service/Schnittstellen/AVM_Technical_Note_-_Session_ID.pdf
# Ermittlung des Response-Wertes
# Beim neuen Login-Verfahren wird also das Klartextpasswort
#  login:command/password=<klartextpassword>
# ersetzt durch
#  login:command/response=<response>
# Der Response-Wert wird wie folgt gebildet:
#  <response> = <challenge>-<md5>
# Der Wert <challenge> kann aus der Datei login_sid.xml bzw. login_sid.lua ausgelesen werden
# und <md5> ist der MD5 (32 Hexzeichen mit Kleinbuchstaben) von
#  <challenge>-<klartextpassword>
#
# Der MD5-Hash wird über die Bytefolge der UTF-16LE-Codierung dieses
# Strings gebildet (ohne BOM und ohne abschließende 0-Bytes).
# Aus Kompatibilitätsgründen muss für jedes Zeichen, dessen Unicode Codepoint > 255 ist, die
# Codierung des "."-Zeichens benutzt werden (0x2e 0x00 in UTF-16LE). Dies betrifft also alle
# Zeichen, die nicht in ISO-8859-1 dargestellt werden können, z. B. das Euro-Zeichen.
# Abschließend ein Beispiel mit deutschem Umlaut:
# Die Challenge
#  <challenge> = "1234567z"
# kombiniert mit dem Kennwort
#  <klartextpassword> = "äbc"
# ergibt den Wert
#  <response> = "1234567z-9e224a41eeefa284df7bb0f26c2913e2"
#
# @param challenge
# @param password
# @return
calculateResponse()
{
	local challenge="$1"
	local password="$2"
	
	# TODO:
	# Replace characters with unicode value > 255 by '.'
	
	
	# printf "%s-%s" $challenge $password	=> create input string
	# iconv -f UTF8 -t UTF16LE				=> convert input string to UTF16LE format
	# sed -r 's/^\xfe\xff(.*)/\1/'			=> remove BOM (byte order mask)
	# md5sum								=> calculate checksum
	# awk '{print $1}')						=> remove stuff after checksum
	local checkSum=$(printf "%s-%s" $challenge $password | iconv -f UTF-8 -t UTF-16LE | sed -r 's/^\xfe\xff(.*)/\1/' | md5sum | awk '{print $1}')
	
	echo "${challenge}-${checkSum}"
}

login()
{
	local url="$1"
	local userName="$2"
	local response="$3"
	local sid=$(requestWebsite "${url}/${PATH_LOGIN}?username=${userName}&response=${response}" | sed -r 's/.*<SID>(.*?)<\/SID>.*/\1/')
	echo "$sid"
}

logout()
{
	local url="$1"
	local sid="$2"
	
	local sid=$(requestWebsite "${url}/${PATH_LOGIN}?logout=yes&sid=${sid}")
}


requestSID()
{
	local url="$1"
	local user="$2"
	local pass="$3"
	
	local challenge=$(requestChallenge "$url")
	logMessage "requestSID_challenge" "$challenge"
	
	local response=$(calculateResponse "$challenge" "$pass");
	logMessage "requestSID_response" "$response"
	
	local sid=$(login "$url" "$user" "$response")
	logMessage "requestSID_sid" "$sid"
	
	echo "$sid"
}


# http://avm.de/fileadmin/user_upload/Global/Service/Schnittstellen/AHA-HTTP-Interface.pdf
has_exec()
{
	local url="$1"
	local sid="$2"
	local ain="$3"
	local cmd="$4"
	
	local result=$(requestWebsite "${url}/${PATH_HOMEAUTOSWITCH}?ain=${ain}&switchcmd=${cmd}&sid=${sid}")
	echo "$result"
}

# http://avm.de/fileadmin/user_upload/Global/Service/Schnittstellen/AHA-HTTP-Interface.pdf
has_list()
{
	local url="$1"
	local sid="$2"
	
	local cmd="getswitchlist"
	local list=$(requestWebsite "$url/${PATH_HOMEAUTOSWITCH}?switchcmd=${cmd}&sid=${sid}")
	echo "$list"
}

user=""
pass=""
sid=""
ain=""
enable=0
disable=0
query=0
URL=""
doLogout=1
list=0

while getopts ":u:p:s:nla:d:q:hU" opt; do
	case $opt in
		u)
			user=$OPTARG;
		;;
		
		p)
			pass=$OPTARG;
		;;
		
		n)
			doLogout=0
		;;
		
		s)
			sid=$OPTARG;
		;;
		
		
		a)
			enable=1;
			ain=$OPTARG;
		;;
		
		d)
			disable=1;
			ain=$OPTARG;
		;;
		
		l)
			list=1
		;;
		
		q)
			query=1
			ain=$OPTARG;
		;;
		
		h)
			printUsage
			exit 0
		;;
		
		U)
			URL_DEBUG=1;
		;;
		
		\?)
			echo "Invalid option: -$OPTARG" >&2
			printUsage >&2
			exit 1
		;;
		
		:)
			echo "Missing argument for -$OPTARG" >&2
			printUsage >&2
			exit 1
		;;
	esac
done

shift $(expr $OPTIND - 1 )

if [ -z $1 ]
then
	echo "address missing" >&2
	exit 1;
fi


URL="http://$1"
#URL_LOGIN="${URL}/login_sid.lua"




if [[ -n $user && -n $pass && -n $sid ]]
then
	echo "Cannot handle username and password and sid" >&2
	exit 1;
fi

if [[  -z $user && -z $pass ]] && [[ -z $sid ]]
then
	echo "Neither username/password nor sid specified" >&2
	exit 1;
fi

if [[ -n $pass ]]
then
	sid=$(requestSID "$URL" "$user" "$pass")
	if [[ $OPTIND -eq 5 ]]
	then
		# just username and password specified => print sid, prevent logout
		doLogout=0
		echo $sid
	fi
fi



if [[ $list -ne 0 ]]
then
	switchList=$(has_list "$URL" $sid)
	
	while read ain
	do
		name=$(has_exec	   "$URL" "$sid" "$ain" "getswitchname")
		state=$(has_exec   "$URL" "$sid" "$ain" "getswitchstate")
		present=$(has_exec "$URL" "$sid" "$ain" "getswitchpresent")
		power=$(has_exec   "$URL" "$sid" "$ain" "getswitchpower")
		
		echo "$ain - $name: state=$state; present=$present; power=$power"
	done < <(echo "$switchList")
fi

# echo "user=$user"
# echo "pass=$pass"
# echo "sid=$sid"
# echo "ain=$ain"
# echo "enable=$enable"
# echo "disable=$disable"
# echo "query=$query"
# echo "URL=$URL"
# echo "doLogout=$doLogout"
# echo "list=$list"


if [[ $query -ne 0 ]]
then
	if [[ -z $ain ]]
	then
		echo "cannot query, no ain specified" >&2;
		exit 1;
	fi
	
	state=$(has_exec "$URL" "$sid" "$ain" "getswitchstate")
	echo $state;
fi


if [[ $enable -ne 0 ]]
then
	if [[ -z $ain ]]
	then
		echo "cannot enable, no ain specified" >&2;
		exit 1;
	fi
	
	has_exec "$URL" "$sid"	"$ain" "setswitchon"
fi

if [[ $disable -ne 0 ]]
then
	if [[ -z $ain ]]
	then
		echo "cannot disable, no ain specified" >&2;
		exit 1;
	fi
	
	has_exec "$URL" "$sid"	"$ain" "setswitchoff"
fi

if [[ $doLogout -ne 0 ]]
then
	logout "$URL" $sid
fi


exit 0;



