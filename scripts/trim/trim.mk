# Search JPG files in current directory
# and use convert from imagemagick to trim the images.
# Afterwards use exiftool to delete thumbnailimage from
# trimmed image

fuzzPercentage=50
trimmedDir=trimmed_$(fuzzPercentage)
input=$(shell ls *.JPG) ;
output=$(input:%.JPG=$(trimmedDir)/%.JPG)


all: trimDir $(output)
	echo "done"
	
trimDir:
	mkdir -p $(trimmedDir)
	
$(trimmedDir)/%.JPG: $(input)
	convert -trim +repage -fuzz $(fuzzPercentage)% $(@:$(trimmedDir)/%.JPG=%).JPG $@
	exiftool -overwrite_original -thumbnailimage= $@
	
clean:
	rm -rf $(trimmedDir)
	