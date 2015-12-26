#! /bin/bash

# This script sends a login request to a AVM Fritz!Box
# (or some other AVM devices) following the steps described in
# http://avm.de/fileadmin/user_upload/Global/Service/Schnittstellen/AVM_Technical_Note_-_Session_ID.pdf
# After that a list of all switches is requested and information about the
# switches is printed to STDOUT
# Please note that currently no error handling is implemented

#################################
# Configuration
#################################
USERNAME="dose"
PASSWORD="123abc"
URL="http://fritz.box"


#################################
# Some other config
#################################
URL_LOGIN="${URL}/login_sid.lua"
URL_HOMEAUTOSWITCH="${URL}/webservices/homeautoswitch.lua"


#################################
# Implementation
#################################

requestChallenge()
{
	local challenge=$(curl ${URL_LOGIN} 2> /dev/null | sed -r 's/.*<Challenge>(.*?)<\/Challenge>.*/\1/')
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
	local checkSum=$(printf "%s-%s" $challenge $password | iconv -f UTF8 -t UTF16LE | sed -r 's/^\xfe\xff(.*)/\1/' | md5sum | awk '{print $1}')
	
	echo "${challenge}-${checkSum}"
}

login()
{
	local userName="$1"
	local response="$2"
	local sid=$(curl "${URL_LOGIN}?username=${userName}&response=${response}" 2> /dev/null | sed -r 's/.*<SID>(.*?)<\/SID>.*/\1/')
	echo "$sid"
}

logout()
{
	local sid="$1"	
	local sid=$(curl "${URL_LOGIN}?logout=yes&sid=${sid}" 2> /dev/null)
}


requestSID()
{
	local challenge=$(requestChallenge)
	local response=$(calculateResponse "$challenge" "$PASSWORD");	
	local sid=$(login "$USERNAME" "$response")
	
	echo "$sid"
}


# http://avm.de/fileadmin/user_upload/Global/Service/Schnittstellen/AHA-HTTP-Interface.pdf
has_exec()
{
	local sid="$1"
	local ain="$2"
	local cmd="$3"
	
	local result=$(curl "${URL_HOMEAUTOSWITCH}?ain=${ain}&switchcmd=${cmd}&sid=${sid}" 2> /dev/null)
	echo "$result"
}

# http://avm.de/fileadmin/user_upload/Global/Service/Schnittstellen/AHA-HTTP-Interface.pdf
has_list()
{
	local sid="$1"
	
	local cmd="getswitchlist"
	local list=$(curl "${URL_HOMEAUTOSWITCH}?switchcmd=${cmd}&sid=${sid}" 2> /dev/null)
	echo "$list"
}

sid=$(requestSID)

switchList=$(has_list $sid)

while read ain
do
	name=$(has_exec	   "$sid" "$ain" "getswitchname")
	state=$(has_exec   "$sid" "$ain" "getswitchstate")
	present=$(has_exec "$sid" "$ain" "getswitchpresent")
	power=$(has_exec   "$sid" "$ain" "getswitchpower")
	echo "$ain - $name: state=$state; present=$present; power=$power"
done < <(echo "$switchList")


logout $sid

exit 0

