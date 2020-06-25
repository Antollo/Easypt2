$flexUrl = "https://netix.dl.sourceforge.net/project/gnuwin32/flex/2.5.4a-1/flex-2.5.4a-1-bin.zip"
$flexZip = "$env:TEMP\flex.zip"
$flexPath = "$env:TEMP\flex"

$LLnextgenUrl = "https://os.ghalkes.nl/LLnextgen/releases/LLnextgen-w64-0.5.5.zip"
$LLnextgenZip = "$env:TEMP\LLnextgen.zip"
$LLnextgenPath = "$env:TEMP\LLnextgen"

Remove-Item $flexPath -Recurse -ErrorAction Ignore
Invoke-WebRequest -Uri $flexUrl -OutFile $flexZip
Expand-Archive $flexZip -DestinationPath $flexPath

Remove-Item $LLnextgenPath -Recurse -ErrorAction Ignore
Invoke-WebRequest -Uri $LLnextgenUrl -OutFile $LLnextgenZip
Expand-Archive $LLnextgenZip -DestinationPath $LLnextgenPath

Invoke-Expression "& '$env:TEMP\flex\bin\flex' --version"
Invoke-Expression "& '$env:TEMP\LLnextgen\LLnextgen\LLnextgen' --version"