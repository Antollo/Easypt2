Import-Module BitsTransfer

$flexUrl = "https://netix.dl.sourceforge.net/project/gnuwin32/flex/2.5.4a-1/flex-2.5.4a-1-bin.zip"
$flexZip = "$env:TEMP\flex.zip"
$flexPath = "$env:TEMP\flex"

$LLnextgenUrl = "https://os.ghalkes.nl/LLnextgen/releases/LLnextgen-w64-0.5.5.zip"
$LLnextgenZip = "$env:TEMP\LLnextgen.zip"
$LLnextgenPath = "$env:TEMP\LLnextgen"

$glewUrl = "https://netcologne.dl.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0-win32.zip"
$glewZip = "$env:TEMP\glew.zip"
$glewPath = "$env:TEMP\glew"

$glfwUrl = "https://github.com/glfw/glfw/releases/download/3.3.3/glfw-3.3.3.bin.WIN64.zip"
$glfwZip = "$env:TEMP\glfw.zip"
$glfwPath = "$env:TEMP\glfw"

Remove-Item $flexPath -Recurse -ErrorAction Ignore
Start-BitsTransfer -Source $flexUrl -Destination $flexZip
Expand-Archive $flexZip -DestinationPath $flexPath

Remove-Item $LLnextgenPath -Recurse -ErrorAction Ignore
Start-BitsTransfer -Source $LLnextgenUrl -Destination $LLnextgenZip
Expand-Archive $LLnextgenZip -DestinationPath $LLnextgenPath

Remove-Item $glewPath -Recurse -ErrorAction Ignore
Start-BitsTransfer -Source $glewUrl -Destination $glewZip
Expand-Archive $glewZip -DestinationPath $glewPath

Remove-Item $glfwPath -Recurse -ErrorAction Ignore
Start-BitsTransfer -Source $glfwUrl -Destination $glfwZip
Expand-Archive $glfwZip -DestinationPath $glfwPath

Invoke-Expression "& '$env:TEMP\flex\bin\flex' --version"
Invoke-Expression "& '$env:TEMP\LLnextgen\LLnextgen\LLnextgen' --version"