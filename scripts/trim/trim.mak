# Search JPG files in current directory
# and use convert from imagemagick to trim the images.
# Afterwards use exiftool to delete thumbnailimage from
# trimmed image

fuzzPercentage ?= 25
trimmedDir=trimmed_$(fuzzPercentage)
#filePattern ?= *.JPG *.jpg
#input ?= $(shell ls $(filePattern)) ;
input ?= $(shell find . -maxdepth 1 -iname "*.jpg") ;
output=$(input:%=$(trimmedDir)/%)


all: trimDir $(output)
	echo "done"
	
trimDir:
	mkdir -p $(trimmedDir)
	
$(trimmedDir)/%: $(input)
	convert -trim +repage -fuzz $(fuzzPercentage)% $(@:$(trimmedDir)/%=%) $@
	exiftool -overwrite_original -thumbnailimage= $@
	
clean:
	rm -rf $(trimmedDir)
	