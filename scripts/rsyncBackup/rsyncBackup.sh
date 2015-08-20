#! /bin/bash



getCommmonRsyncArgs()
{    
    local backupDir="$backupName/$(date +'%Y-%m-%d---%H_%M_%S')/"

    local includeParams=""
    
    for ((i = 0; i < ${#includeConfig[@]}; i++))
    do
        includeParams="${includeParams} --include=\"${includeConfig[$i]}\""
    done

    # note: file must be writable by the user, otherwise rsync will fail with the next run
    #       in case of changed files as the file cannot be modified
    local result="--verbose --recursive --links --times --devices --specials --backup --backup-dir=\"$backupDir\" --delete --chmod=u=rwX,g=,o= $additionalParams $includeParams --progress --partial --append-verify"

    if [ $remoteBackup == 0 ]
    then
        # local backup
        result="$result $localBackupRoot $destinationDirectory"
    else
        # remote backup
        result="$result --rsh=\"ssh -p $sshPort\" $localBackupRoot ${username}@${remoteHost}${module}/${destinationDirectory}"
    fi
    
    echo $result;
}

listChanges()
{
    commonArgs=$(getCommmonRsyncArgs)

    # See http://andreafrancia.it/2010/03/understanding-the-output-of-rsync-itemize-changes.html
    cmd="rsync --dry-run --itemize-changes $commonArgs | perl -e 'while (<STDIN>){if (\$_ =~ /<f.*?\s(.*)/){print \$1, \"\n\";}}'"
    eval $cmd
}

exportChanges()
{
    local exportDir="$1"

    mkdir -p "$exportDir"
    
    changedFiles=$(listChanges);
    
    local numChanged=$(echo "$changedFiles" | wc -l)
    local counter=1

    echo "$changedFiles" | while read line; do
        local exportedFile="$exportDir/$line"
        
        
        # use "rsync" instead of "cp" to show progress
        printf "[%i/%i]\n" $counter $numChanged
        rsync  --progress --archive --relative --chmod=ugo=rwX "/$line" "$exportDir"
        echo ""
        
        counter=$(expr $counter + 1)
    done
}


checkDir()
{
    local varName="$1"
    local varValue=$(eval "echo \"\$$varName\"")

    echo ${localBackupRoot}${varValue}
    if [ -d "${localBackupRoot}${varValue}" ]
    then
        echo "################# ERROR ##################" >&2
        echo "# '${localBackupRoot}${varValue}'" >&2
        echo "# is a local directory. Please change value" >&2
        echo "# of variable '\$$varName'" >&2
        echo "##########################################" >&2
        exit 1;
    fi
}

makeBackup()
{
#    checkDir "backupName" || exit 1

    commonArgs=$(getCommmonRsyncArgs)

    cmd="rsync  $commonArgs "
    echo "#############"
    echo "$cmd"
    echo "================"
    eval "$cmd" || exit
}



################################################################
# Related to "makeRules"
################################################################
makeIncludeFilter()
{
    echo "+ ""$1" 
}

getParents()
{
    local d="$1"
    
    makeIncludeFilter "$d"
    
    while [ "$d" != "/" ]
    do    
        d=$(dirname $d)
        makeIncludeFilter "$d"
    done
}

makeIncRules()
{
    for d in "$@"
    do
        if [ -d "$d" ]
        then
            d=$(realpath "$d")
            getParents "$d"
            makeIncludeFilter "$d/**"
        else
            echo "not a directory: $d" >&2
        fi
    done
}

makeRules()
{
    makeIncRules "$@" | sort -u
    echo "- *"
}
################################################################

printConfigTmpl()
{
echo '###########################################'
echo '# Settings'
echo '###########################################'
echo ''
echo '# 0: create backup on local drive'
echo '# 1: create backup on server'
echo 'remoteBackup=1'
echo ''
echo '###########################################'
echo '# Settings for remote ($remoteBackup == 1) backups'
echo '###########################################'
echo ''
echo '# username for ssh login'
echo 'username="homer"'
echo ''
echo '# Server address'
echo 'remoteHost="omv"'
echo ''
echo '# rsync modul in which the backups will be created'
echo 'module="::omv_homes/${username}";'
echo ''
echo '# port for ssh connection'
echo 'sshPort=22'
echo ''
echo '###########################################'
echo '# Common settings'
echo '###########################################'
echo ''
echo '# destination directory for the backup'
echo 'destinationDirectory="backup";'
echo ''
echo '# The starting directory for backups (all files and directories'
echo '# which should end up in the backup need to be inside this directory'
echo 'localBackupRoot="/home/foo"'
echo ''
echo '# Changed or deleted files will be backed up in this directory.'
echo '# Note: this directory needs to be outside $destinationDirectory!'
echo '#   Use something like "../.rsyncBackup" for local backups (($remoteBackup == 0)'
echo '#   In case of remote backups (($remoteBackup != 0) an absolute path inside the'
echo '#   module is suggested (something like "/$username/.rsyncBackup")'
echo 'if [ $remoteBackup == 0 ]'
echo 'then'
echo '    backupName="../.rsyncBackup"'
echo 'else'
echo '    backupName="/$username/.rsyncBackup"'
echo 'fi'
echo ''
echo '# Each item will end up as a "--include" parameter for rsync'
echo '# See also:'
echo '#   http://masstransmit.com/garage_blog/rsync-quirks/'
echo '#   http://www.thegeekstuff.com/2011/01/rsync-exclude-files-and-folders/'
echo 'includeConfig=(\'
echo '"+ **" \'
echo '"- *"'
echo ')'
echo ''
echo '# Additional parameters given to rsync'
echo 'additionalParams=""'
echo ''
}

printUsage()
{
    echo "#############################################################"
    echo ""
    echo "Usage:"
    echo "  >$0 [command] [params] (configFile)"
    echo ""
    echo "Commands:"
    echo ""
    echo "configTmpl"
    echo " Print a config template" 
    echo ""
    echo "makeRules [dir] [dir] ..."
    echo "  Takes a list of directories and creates entries"
    echo "  to be stored in \"\$includeConfig\""
    echo "  Note: It is assumed that \$localBackupRoot is set"
    echo "        to root directory (/)"
    echo ""
    echo "diff (configFile)"
    echo "  Get list of changed local files compared to latest snapshot"
    echo ""
    echo "export (configFile) [exportDir={exported_{date_time}]"
    echo "  Copy changed local files to a directory"
    echo ""
    echo "backup (configFile)"
    echo "  Create backup according to config file"
    echo ""
    echo "#############################################################"
}

#set -x

case $1 in
    
    "diff")
        source "./$2" || exit 1
        listChanges
    ;;
    
    "export")
        source "./$2" || exit 1
        exportDir=$3;
        
        if [ -z $exportDir ]
        then
            dateString=$(date +'%Y-%m-%d---%H_%M_%S')
            exportDir="exported_$dateString"
        fi
        
        exportChanges "$exportDir"
    ;;
    
    makeRules)
        shift
        makeRules "$@"
    ;;
    
    configTmpl)
        printConfigTmpl
    ;;
    
    backup)
        source "./$2" || exit 1
        makeBackup
    ;;
    
    *)
        printUsage
        exit 1
    ;;
    
esac


## Lade Konfigdatei
#if [ -f $1 && -n $2 ]
#then
#    source "./$1"
#elif [ $1 == "configTmpl" ]
#then
#    printConfigTmpl
#    exit 0
#else
#    printUsage
#    exit 1
#fi
#
#case $2 in
#    
#    "diff")
#        listChanges
#    ;;
#    
#    "export")
#        exportDir=$3;
#        
#        if [ -z $exportDir ]
#        then
#            dateString=$(date +'%Y-%m-%d---%H_%M_%S')
#            exportDir="exported_$dateString"
#        fi
#        
#        exportChanges "$exportDir"
#    ;;
#    
#    makeRules)
#        shift
#        shift
#        makeRules "$@"
#    ;;
#    
#    configTmpl)
#        printConfigTmpl
#    ;;
#    
#    backup)
#        makeBackup
#    ;;
#    
#    *)
#        printUsage
#        exit 1
#    ;;
#    
#esac
